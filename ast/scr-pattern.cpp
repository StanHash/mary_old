
#include "scr-pattern.h"

namespace mary {
namespace pattern {

// wildcard-visiting template functions
// for matching and wildcard extraction

template<typename Visitor>
static bool visit_expressions(const ExprPatternSpan& patterns, const ExpressionSpan& expressions, Visitor v)
{
	if (patterns.size() != expressions.size())
		return false;

	for (unsigned i = 0; i < patterns.size(); ++i)
	{
		switch (patterns[i].exprClass)
		{

		case ExprPattern::wildcard_class:
		{
			v(patterns[i].wildcardMatch, expressions[i]);
			continue;
		}

		case ExprPattern::literal_class:
		{
			if (!expressions[i]->is_literal())
				return false;

			if (expressions[i]->literal.value != patterns[i].literal.value)
				return false;

			continue;
		}

		default:
		{
			if (!expressions[i]->is_compound())
				return false;

			if (patterns[i].children.size() != expressions[i]->child_count())
				return false;

			if (!visit_expressions(patterns[i].children, expressions[i]->compound.children, v))
				return false;

			continue;
		}

		} // switch (patterns[i].exprClass)
	}

	return true;
}

template<typename Visitor>
static bool visit_statements(const Pattern& pattern, const StatementSpan& statements, Visitor v)
{
	if (pattern.statement_count() != statements.size())
		return false;

	for (unsigned i = 0; i < pattern.size(); ++i)
	{
		if (statements[i].stmtClass != pattern[i].stmtClass)
			return false;

		if (statements[i].children.size() != pattern[i].expressions.size())
			return false;

		if (!visit_expressions(pattern[i].expressions, statements[i].children, v))
			return false;
	}

	return true;
}

static bool match(const Pattern& pattern, const StatementSpan& statements)
{
	return visit_statements(pattern, statements, [] (unsigned, std::unique_ptr<ScrExpr>&) {});
}

static std::vector<std::unique_ptr<ScrExpr>> extract_wildcards(const Pattern& pattern, const StatementSpan& statements)
{
	std::vector<std::unique_ptr<ScrExpr>> result;

	visit_statements(pattern, statements, [&result] (unsigned i, std::unique_ptr<ScrExpr>& uptr) {
		if (i >= result.size())
			result.resize(i+1);

		result[i] = std::move(uptr);
	});

	return result;
}

// construction functions
// for constructing from pattern+wildcard matches

static std::vector<std::unique_ptr<ScrExpr>> construct_expressions(
	const ExprPatternSpan& patterns, const Span<std::unique_ptr<ScrExpr>>& wildcardExpressions)
{
	std::vector<std::unique_ptr<ScrExpr>> result;
	result.reserve(patterns.size());

	for (auto& exprPattern : patterns)
	{
		switch (exprPattern.exprClass)
		{

		case ExprPattern::wildcard_class:
		{
			result.push_back(
				std::move(wildcardExpressions[exprPattern.wildcardMatch]));

			continue;
		}

		case ExprPattern::literal_class:
		{
			result.push_back(
				make_literal_expr(exprPattern.literal.value, exprPattern.literal.typeClass));

			continue;
		}

		default:
		{
			auto inner = construct_expressions(exprPattern.children, wildcardExpressions);

			result.push_back(
				make_compound_expr(exprPattern.exprClass, inner));

			continue;
		}

		} // switch (exprPattern.exprClass)
	}

	return result;
}

template<typename OutIteratorType>
static void construct(OutIteratorType it, const Pattern& pattern, const Span<std::unique_ptr<ScrExpr>>& wildcardExpressions)
{
	for (auto& stmtPattern : pattern)
	{
		auto exprs = construct_expressions(stmtPattern.expressions, wildcardExpressions);
		*it++ = ScrStatement(stmtPattern.stmtClass, exprs);
	}
}

// the rule applying function
// this is where everything comes together

std::vector<ScrStatement> reduce_statements(const Span<pattern::PatternRule>& rules, std::vector<ScrStatement>&& statements)
{
	std::vector<ScrStatement> result;
	result.reserve(statements.size()); // does this matter?

	for (auto& statement : statements)
	{
		result.push_back(std::move(statement));

		for (auto& rule : rules)
		{
			unsigned stmtCount = rule.from.statement_count();

			if (stmtCount > result.size())
				continue;

			auto inSpan = StatementSpan(result.data() + result.size() - stmtCount, stmtCount);

			if (pattern::match(rule.from, inSpan))
			{
				auto exprs = pattern::extract_wildcards(rule.from, inSpan);

				result.resize(result.size() - stmtCount + rule.to.statement_count());
				pattern::construct(std::prev(result.end(), rule.to.statement_count()), rule.to, exprs);

				break;
			}
		}
	}

	return result;
}

} // namespace pattern
} // namespace mary
