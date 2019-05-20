#ifndef MARY_SCR_PATTERN_H
#define MARY_SCR_PATTERN_H

#include "core/types.h"

#include "ast/scr-expression.h"
#include "ast/scr-statement.h"
#include "ast/function.h"

#include <limits>

namespace mary {
namespace pattern {

struct ExprPattern;
struct StmtPattern;

using ExprPatternSpan = Span<const ExprPattern>;
using StmtPatternSpan = Span<const StmtPattern>;

struct StmtPattern
{
	constexpr StmtPattern()
		: stmtClass(SCR_STMT_MAX) {}

	constexpr StmtPattern(unsigned short cls, ExprPatternSpan span)
		: expressions(span), stmtClass(cls) {}

	ExprPatternSpan expressions;
	unsigned short stmtClass;
};

struct ExprPattern
{
	constexpr ExprPattern()
		: exprClass(SCR_EXPR_MAX), wildcardMatch(0) {}

	constexpr ExprPattern(unsigned short match)
		: exprClass(wildcard_class), wildcardMatch(match) {}

	constexpr ExprPattern(std::int32_t value, unsigned type)
		: exprClass(literal_class), literal({ value, type }) {}

	constexpr ExprPattern(unsigned short cls, ExprPatternSpan span)
		: exprClass(cls), children(span) {}

	unsigned short exprClass;

	union
	{
		unsigned short  wildcardMatch;
		ScrValue        literal;
		ExprPatternSpan children;
	};

	static constexpr unsigned short literal_class  = SCR_EXPR_LITERAL;
	static constexpr unsigned short wildcard_class = std::numeric_limits<unsigned short>::max();
};

struct Pattern : public Span<const StmtPattern>
{
	using Span<const StmtPattern>::Span;

	constexpr unsigned statement_count() const { return size(); }
};

struct PatternRule
{
	Pattern from, to;
};

struct FunctionPatterns
{
	FunctionPatterns() {}

	FunctionPatterns(const Function& function) { rebuild(function); }

	FunctionPatterns(const FunctionPatterns& other) = delete;
	FunctionPatterns& operator = (const FunctionPatterns& other) = delete;

	FunctionPatterns(FunctionPatterns&& other)
	{
		rebuild(std::move(other));
	}

	FunctionPatterns& operator = (FunctionPatterns&& other)
	{
		rebuild(std::move(other));
		return *this;
	}

	PatternRule decompile_rule() const
	{
		return { from(), to() };
	}

	PatternRule compile_rule() const
	{
		return { to(), from() };
	}

	Pattern from() const
	{
		return Pattern(stmt.data() + 1, stmt.size() - 1);
	}

	Pattern to() const
	{
		return Pattern(stmt.data(), 1);
	}

	void rebuild(const Function& function)
	{
		stmt.resize(function.parameters.size() + 2);
		expr.resize(function.parameters.size() + 2);

		for (unsigned i = 0; i < function.parameters.size(); ++i)
		{
			// expr: (wildcard i)
			expr[i + 1] = ExprPattern(i);

			// stmt: push (wildcard i);
			stmt[i + 1] = StmtPattern(SCR_OPCODE_PUSH, ExprPatternSpan(expr.data() + (i + 1), 1));
		}

		// expr: (func id)
		expr.front() = ExprPattern(function.identifier, SCR_VALTYPE_FUNCID);

		// expr: (func call)
		expr.back() = ExprPattern(SCR_EXPR_FUNC, ExprPatternSpan(expr.data(), expr.size()-1));

		// stmt: either push (func(...)); or (func(...));
		stmt.front() = StmtPattern(
			(function.resultType == SCR_VALTYPE_VOID)
				? (unsigned) SCR_STMT_EXPR : (unsigned) SCR_OPCODE_PUSH,
			ExprPatternSpan(expr.data() + expr.size()-1, 1));

		// stmt: call (func);
		stmt.back() = StmtPattern(SCR_OPCODE_CALL, ExprPatternSpan(expr.data(), 1));
	}

	void rebuild(const FunctionPatterns& other)
	{
		// copy

		stmt = other.stmt;
		expr = other.expr;

		// fix spans

		for (unsigned i = 0; i < expr.size()-1; ++i)
			stmt[i + 1].expressions = ExprPatternSpan(expr.data() + (i + 1), 1);

		expr.back().children = ExprPatternSpan(expr.data(), expr.size()-1);
		stmt.front().expressions = ExprPatternSpan(expr.data() + expr.size()-1, 1);
		stmt.back().expressions = ExprPatternSpan(expr.data(), 1);
	}

	void rebuild(FunctionPatterns&& other)
	{
		// move

		stmt = std::move(other.stmt);
		expr = std::move(other.expr);

		// fix spans

		for (unsigned i = 0; i < expr.size()-1; ++i)
			stmt[i + 1].expressions = ExprPatternSpan(expr.data() + (i + 1), 1);

		expr.back().children = ExprPatternSpan(expr.data(), expr.size()-1);
		stmt.front().expressions = ExprPatternSpan(expr.data() + expr.size()-1, 1);
		stmt.back().expressions = ExprPatternSpan(expr.data(), 1);
	}

protected:
	std::vector<StmtPattern> stmt; /* order: func; push...; call; */
	std::vector<ExprPattern> expr; /* order: id; wild... */
};

std::vector<ScrStatement> reduce_statements(const Span<PatternRule>& rules, std::vector<ScrStatement>&& statements);

std::vector<FunctionPatterns> get_function_patterns(const std::vector<Function>& functions);
std::vector<PatternRule> get_decompile_rules(const std::vector<FunctionPatterns>& functions);

std::vector<PatternRule> get_decompile_rules();
std::vector<PatternRule> get_compile_rules();

} // namespace pattern
} // namespace mary

#endif // MARY_SCR_PATTERN_H
