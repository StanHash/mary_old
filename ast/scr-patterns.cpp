
#include "scr-pattern.h"

// This file contains standard pattern data for mary (de)compilation

namespace mary {
namespace pattern {

namespace patterns {

// expression list: (A)

template<unsigned Match>
constexpr ExprPattern expr_wildcard[] = {
	ExprPattern(Match)
};

// expression list: (A), (B)

template<unsigned MatchA, unsigned MatchB>
constexpr ExprPattern expr_binop_inner[]
{
	ExprPattern(MatchA), // A
	ExprPattern(MatchB), // B
};

// expression list: (A <op> B)

template<unsigned ExprClass, unsigned MatchA, unsigned MatchB>
constexpr ExprPattern expr_binop[]
{
	ExprPattern(ExprClass, expr_binop_inner<MatchA, MatchB>)
};

// expression list: ([A]), (B)

template<unsigned MatchA, unsigned MatchB>
constexpr ExprPattern expr_equ_inner[]
{
	ExprPattern(SCR_EXPR_DEREF, expr_wildcard<MatchA>), // [A]
	ExprPattern(MatchB), // B
};

// expression list: ([A] <op> B)

template<unsigned ExprClass, unsigned MatchA, unsigned MatchB>
constexpr ExprPattern expr_equ[]
{
	ExprPattern(ExprClass, expr_equ_inner<MatchA, MatchB>)
};

// expression list: (<op> A)

template<unsigned ExprClass, unsigned Match>
constexpr ExprPattern expr_unop[]
{
	ExprPattern(ExprClass, expr_wildcard<Match>)
};

// statement list: push A; push B; <stmt>;

template<unsigned StmtClass, unsigned MatchA, unsigned MatchB>
constexpr StmtPattern push_push_op[]
{
	StmtPattern(SCR_OPCODE_PUSH, expr_wildcard<MatchA>),
	StmtPattern(SCR_OPCODE_PUSH, expr_wildcard<MatchB>),
	StmtPattern(StmtClass, {}),
};

// statement list: push A; <stmt>;

template<unsigned StmtClass, unsigned Match>
constexpr StmtPattern push_op[]
{
	StmtPattern(SCR_OPCODE_PUSH, expr_wildcard<Match>),
	StmtPattern(StmtClass, {}),
};

// statement list: push (A <op> B);

template<unsigned ExprClass, unsigned MatchA, unsigned MatchB>
constexpr StmtPattern push_binop[]
{
	StmtPattern(SCR_OPCODE_PUSH, expr_binop<ExprClass, MatchA, MatchB>)
};

// statement list: push ([A] <op> B);

template<unsigned ExprClass, unsigned MatchA, unsigned MatchB>
constexpr StmtPattern push_equ[]
{
	StmtPattern(SCR_OPCODE_PUSH, expr_equ<ExprClass, MatchA, MatchB>)
};

// statement list: push A; pop;

template<unsigned Match>
constexpr StmtPattern push_pop[]
{
	StmtPattern(SCR_OPCODE_PUSH, expr_wildcard<Match>),
	StmtPattern(SCR_OPCODE_POP, {}),
};

// statement list: A;

template<unsigned Match>
constexpr StmtPattern expr_stmt[]
{
	StmtPattern(SCR_STMT_EXPR, expr_wildcard<Match>),
};

// statement list: <stmt> A;

template<unsigned StmtClass, unsigned Match>
constexpr StmtPattern op_imm_stmt[]
{
	StmtPattern(StmtClass, expr_wildcard<Match>),
};

// statement list: push (<op> A);

template<unsigned ExprClass, unsigned Match>
constexpr StmtPattern push_unop[]
{
	StmtPattern(SCR_OPCODE_PUSH, expr_unop<ExprClass, Match>),
};

} // namespace patterns

template<typename RuleFunc>
static std::vector<PatternRule> get_rules(RuleFunc r)
{
	using namespace patterns;

	return
	{
		r(Pattern(op_imm_stmt<SCR_OPCODE_PUSHM, 0>), Pattern(push_unop<SCR_EXPR_DEREF, 0>)),

		r(Pattern(push_push_op<SCR_OPCODE_EQU,    0, 1>), Pattern(push_equ<SCR_EXPR_EQU,    0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_ADDEQU, 0, 1>), Pattern(push_equ<SCR_EXPR_ADDEQU, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_SUBEQU, 0, 1>), Pattern(push_equ<SCR_EXPR_SUBEQU, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_MULEQU, 0, 1>), Pattern(push_equ<SCR_EXPR_MULEQU, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_DIVEQU, 0, 1>), Pattern(push_equ<SCR_EXPR_DIVEQU, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_MODEQU, 0, 1>), Pattern(push_equ<SCR_EXPR_MODEQU, 0, 1>)),

		r(Pattern(push_push_op<SCR_OPCODE_ADD, 0, 1>), Pattern(push_binop<SCR_EXPR_ADD, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_SUB, 0, 1>), Pattern(push_binop<SCR_EXPR_SUB, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_MUL, 0, 1>), Pattern(push_binop<SCR_EXPR_MUL, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_DIV, 0, 1>), Pattern(push_binop<SCR_EXPR_DIV, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_MOD, 0, 1>), Pattern(push_binop<SCR_EXPR_MOD, 0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_OR,  0, 1>), Pattern(push_binop<SCR_EXPR_OR,  0, 1>)),
		r(Pattern(push_push_op<SCR_OPCODE_AND, 0, 1>), Pattern(push_binop<SCR_EXPR_AND, 0, 1>)),

		r(Pattern(push_op<SCR_OPCODE_NEG, 0>), Pattern(push_unop<SCR_EXPR_NEG, 0>)),
		r(Pattern(push_op<SCR_OPCODE_NOT, 0>), Pattern(push_unop<SCR_EXPR_NOT, 0>)),

		r(Pattern(push_pop<0>), Pattern(expr_stmt<0>)),
	};
}

std::vector<PatternRule> get_decompile_rules()
{
	return get_rules([] (auto&& from, auto&& to)
	{
		return PatternRule { from, to };
	});
}

std::vector<PatternRule> get_compile_rules()
{
	return get_rules([] (auto&& from, auto&& to)
	{
		return PatternRule { to, from };
	});
}

std::vector<FunctionPatterns> get_function_patterns(const std::vector<Function>& functions)
{
	std::vector<FunctionPatterns> result(functions.size());

	for (unsigned i = 0; i < functions.size(); ++i)
		result[i].rebuild(functions[i]);

	return result;
}

std::vector<PatternRule> get_decompile_rules(const std::vector<FunctionPatterns>& functions)
{
	std::vector<PatternRule> result { get_decompile_rules() };
	result.reserve(result.size() + functions.size());

	for (unsigned i = 0; i < functions.size(); ++i)
	{
		auto rule = functions[i].decompile_rule();
		result.push_back(rule);
	}

	return result;
}

/* What follows is the previous draft of the pattern system (it's much worse)

enum
{
	 * MARY PATTERN BYTECODE LANGUAGE pseudo-ebnf
	 *
	 * PATTERN = { $PATTERN_OP_STMT, STMT }, $PATTERN_OP_END ;
	 * EXPRS   = { ( $PATTERN_OP_EXPR, EXPR ) | ( $PATTERN_OP_WILDCARD, WILD ) }, $PATTERN_OP_END ;
	 * STMT    = $StmtClass, EXPRS ;
	 * EXPR    = $ExprClass, EXPRS ;
	 * WILD    = $MatchId ;
	 *

	PATTERN_OP_STMT,
	PATTERN_OP_EXPR,
	PATTERN_OP_WILDCARD,
	PATTERN_OP_END,
};

struct PatternRef
{
	template<unsigned N>
	constexpr PatternRef(const byte_type (&patterns) [N])
		: begin(patterns), end(patterns + N) { mIsValid = validate(); }

	bool is_valid() const { return mIsValid; }
	unsigned statement_count() const { return mStatementCount; }

	const byte_type* begin;
	const byte_type* end;

private:
	bool validate();

	unsigned mStatementCount = 0;
	bool mIsValid = false;
};

struct RuleRef
{
	PatternRef from;
	PatternRef to;
};

#define PATTERN_STMT(type)        PATTERN_OP_STMT, (type)
#define PATTERN_EXPR(type)        PATTERN_OP_EXPR, (type)
#define PATTERN_WILDCARD(matchId) PATTERN_OP_WILDCARD, (matchId)
#define PATTERN_END               PATTERN_OP_END

// push <*>;
#define PATTERN_PUSH_ANY_STMT(exprMatchId) \
	PATTERN_STMT(SCR_OPCODE_PUSH32), \
		PATTERN_WILDCARD(exprMatchId), \
		PATTERN_END

// <opcode>;
#define PATTERN_OPCODE_STMT(opcode) \
	PATTERN_STMT(opcode), \
		PATTERN_END

// push (<*> <op> <*>);
#define PATTERN_PUSH_BINOP_STMT(exprClass, lMatchId, rMatchId) \
	PATTERN_STMT(SCR_OPCODE_PUSH32), \
		PATTERN_EXPR(exprClass), \
			PATTERN_WILDCARD(lMatchId), \
			PATTERN_WILDCARD(rMatchId), \
			PATTERN_END, \
		PATTERN_END

#define PATTERN_PUSH_EQU_STMT(exprClass, lMatchId, rMatchId) \
	PATTERN_STMT(SCR_OPCODE_PUSH32), \
		PATTERN_EXPR(exprClass), \
			PATTERN_EXPR(SCR_EXPR_DEREF), \
				PATTERN_WILDCARD(lMatchId), \
				PATTERN_END, \
			PATTERN_WILDCARD(rMatchId), \
			PATTERN_END, \
		PATTERN_END

// push <A>; push <B>; <opcode>; <=> push (<A> <op> <B>);
#define DECL_BINOP_CONVERT_PATTERNS(inName, outName, inOpcodeClass, outExprClass) \
	static const byte_type inName[] = { \
		PATTERN_PUSH_ANY_STMT(0), \
		PATTERN_PUSH_ANY_STMT(1), \
		PATTERN_OPCODE_STMT(inOpcodeClass), \
		PATTERN_END, \
	}; \
	static const byte_type outName[] = { \
		PATTERN_PUSH_BINOP_STMT(SCR_EXPR_ADD, 0, 1), \
		PATTERN_END, \
	};

// push <A>; push <B>; <opcode>; <=> push ([<A>] <op> <B>);
#define DECL_EQU_CONVERT_PATTERNS(inName, outName, inOpcodeClass, outExprClass) \
	static const byte_type inName[] = { \
		PATTERN_PUSH_ANY_STMT(0), \
		PATTERN_PUSH_ANY_STMT(1), \
		PATTERN_OPCODE_STMT(inOpcodeClass), \
		PATTERN_END, \
	}; \
	static const byte_type outName[] = { \
		PATTERN_PUSH_EQU_STMT(SCR_EXPR_ADD, 0, 1), \
		PATTERN_END, \
	};

DECL_EQU_CONVERT_PATTERNS(sPatternStmtPushEquIn,    sPatternStmtPushEquOut,    SCR_OPCODE_EQU,    SCR_EXPR_EQU)
DECL_EQU_CONVERT_PATTERNS(sPatternStmtPushAddEquIn, sPatternStmtPushAddEquOut, SCR_OPCODE_ADDEQU, SCR_EXPR_ADDEQU)
DECL_EQU_CONVERT_PATTERNS(sPatternStmtPushSubEquIn, sPatternStmtPushSubEquOut, SCR_OPCODE_SUBEQU, SCR_EXPR_SUBEQU)
DECL_EQU_CONVERT_PATTERNS(sPatternStmtPushMulEquIn, sPatternStmtPushMulEquOut, SCR_OPCODE_MULEQU, SCR_EXPR_MULEQU)
DECL_EQU_CONVERT_PATTERNS(sPatternStmtPushDivEquIn, sPatternStmtPushDivEquOut, SCR_OPCODE_DIVEQU, SCR_EXPR_DIVEQU)
DECL_EQU_CONVERT_PATTERNS(sPatternStmtPushModEquIn, sPatternStmtPushModEquOut, SCR_OPCODE_MODEQU, SCR_EXPR_MODEQU)

DECL_BINOP_CONVERT_PATTERNS(sPatternStmtPushAddIn, sPatternStmtPushAddOut, SCR_OPCODE_ADD, SCR_EXPR_ADD)
DECL_BINOP_CONVERT_PATTERNS(sPatternStmtPushSubIn, sPatternStmtPushSubOut, SCR_OPCODE_SUB, SCR_EXPR_SUB)
DECL_BINOP_CONVERT_PATTERNS(sPatternStmtPushMulIn, sPatternStmtPushMulOut, SCR_OPCODE_MUL, SCR_EXPR_MUL)
DECL_BINOP_CONVERT_PATTERNS(sPatternStmtPushDivIn, sPatternStmtPushDivOut, SCR_OPCODE_DIV, SCR_EXPR_DIV)
DECL_BINOP_CONVERT_PATTERNS(sPatternStmtPushModIn, sPatternStmtPushModOut, SCR_OPCODE_MOD, SCR_EXPR_MOD)
DECL_BINOP_CONVERT_PATTERNS(sPatternStmtPushOrIn,  sPatternStmtPushOrOut,  SCR_OPCODE_OR,  SCR_EXPR_OR)
DECL_BINOP_CONVERT_PATTERNS(sPatternStmtPushAndIn, sPatternStmtPushAndOut, SCR_OPCODE_AND, SCR_EXPR_AND)

static const byte_type sPatternStmtPushPop[] = {
	PATTERN_PUSH_ANY_STMT(0),
	PATTERN_OPCODE_STMT(SCR_OPCODE_POP),
	PATTERN_END,
};

static const byte_type sPatternStmtAnyExpr[] = {
	PATTERN_STMT(SCR_STMT_EXPR),
		PATTERN_WILDCARD(0),
		PATTERN_END,
	PATTERN_END
};

std::vector<RuleRef> get_standard_rules()
{
	return
	{
		{ PatternRef(sPatternStmtPushEquIn),    PatternRef(sPatternStmtPushEquOut)    },
		{ PatternRef(sPatternStmtPushAddEquIn), PatternRef(sPatternStmtPushAddEquOut) },
		{ PatternRef(sPatternStmtPushSubEquIn), PatternRef(sPatternStmtPushSubEquOut) },
		{ PatternRef(sPatternStmtPushMulEquIn), PatternRef(sPatternStmtPushMulEquOut) },
		{ PatternRef(sPatternStmtPushDivEquIn), PatternRef(sPatternStmtPushDivEquOut) },
		{ PatternRef(sPatternStmtPushModEquIn), PatternRef(sPatternStmtPushModEquOut) },

		{ PatternRef(sPatternStmtPushAddIn), PatternRef(sPatternStmtPushAddOut) },
		{ PatternRef(sPatternStmtPushSubIn), PatternRef(sPatternStmtPushSubOut) },
		{ PatternRef(sPatternStmtPushMulIn), PatternRef(sPatternStmtPushMulOut) },
		{ PatternRef(sPatternStmtPushDivIn), PatternRef(sPatternStmtPushDivOut) },
		{ PatternRef(sPatternStmtPushModIn), PatternRef(sPatternStmtPushModOut) },
		{ PatternRef(sPatternStmtPushOrIn),  PatternRef(sPatternStmtPushOrOut) },
		{ PatternRef(sPatternStmtPushAndIn), PatternRef(sPatternStmtPushAndOut) },

		{ PatternRef(sPatternStmtPushPop),   PatternRef(sPatternStmtAnyExpr) },
	};
}

struct PatternScanner
{
	PatternScanner(const PatternRef& ref)
		: it(ref.begin), end(ref.end) {}

	inline
	unsigned consume_byte()
	{
		if (it >= end)
		{
			// Invalid pattern
			throw false;
		}

		return *it++;
	}

	inline
	unsigned peek_byte() const
	{
		if (it >= end)
		{
			// Invalid pattern
			throw false;
		}

		return *it;
	}

private:
	const byte_type* it;
	const byte_type* end;
};

static bool scan_expressions(PatternScanner& scanner)
{
	while (true)
	{
		switch (scanner.consume_byte())
		{

		case PATTERN_OP_WILDCARD:
			scanner.consume_byte();
			break;

		case PATTERN_OP_EXPR:
			scanner.consume_byte();

			if (!scan_expressions(scanner))
				return false;

			break;

		case PATTERN_OP_END:
			return true;

		default:
			// Invalid pattern
			return false;

		}
	}
}

bool PatternRef::validate()
{
	PatternScanner scanner(*this);

	try
	{
		while (true)
		{
			switch (scanner.consume_byte())
			{

			case PATTERN_OP_STMT:
				scanner.consume_byte();

				if (!scan_expressions(scanner))
					return false;

				mStatementCount++;

				break;

			case PATTERN_OP_END:
				return true;

			default:
				return false;

			} // switch (scanner.consume_byte())
		}
	}
	catch (bool)
	{
		return false;
	}
}

static bool match_expressions(PatternScanner& scanner, const std::vector<std::unique_ptr<ScrExpr>>& expressions);
static bool match_expression(PatternScanner& scanner, const ScrExpr& expression);
static bool match_statement(PatternScanner& scanner, const ScrStatement& statement);

static bool match_expressions(PatternScanner& scanner, const std::vector<std::unique_ptr<ScrExpr>>& expressions)
{
	auto it = expressions.begin();

	while (true)
	{
		switch (scanner.consume_byte())
		{

		case PATTERN_OP_WILDCARD:
		{
			// Wildcard aka any expression

			if (it++ == expressions.end())
				return false;

			scanner.consume_byte(); // consume match id byte

			break;
		}

		case PATTERN_OP_EXPR:
		{
			if (it == expressions.end())
				return false;

			if (!match_expression(scanner, **it++))
				return false;

			break;
		}

		case PATTERN_OP_END:
		{
			return true;
		}

		default:
		{
			// Invalid pattern
			throw false;
		}

		} // switch (consume_byte())
	}
}

static bool match_expression(PatternScanner& scanner, const ScrExpr& expression)
{
	if (scanner.consume_byte() != expression.exprClass)
		return false;

	if (scanner.peek_byte() != PATTERN_OP_END)
	{
		if (!expression.is_compound())
			return false;

		if (!match_expressions(scanner, expression.compound.children))
			return false;
	}

	return true;
}

static bool match_statement(PatternScanner& scanner, const ScrStatement& statement)
{
	if (scanner.consume_byte() != statement.stmtClass)
		return false;

	return match_expressions(scanner, statement.children);
}

static bool match_statements(const PatternRef& pattern, const ScrStatement* begin, const ScrStatement* end)
{
	if (std::distance(begin, end) != pattern.statement_count())
		return false;

	PatternScanner scanner(pattern);

	auto it = begin;

	while (true)
	{
		switch (scanner.consume_byte())
		{

		case PATTERN_OP_STMT:
		{
			if (!match_statement(scanner, *it++))
				return false;

			break;
		}

		case PATTERN_OP_END:
		{
			return true;
		}

		default:
		{
			// Invalid pattern
			throw false;
		}

		} // switch (scanner.consume_byte())
	}

	return false;
}

std::vector<ScrStatement> reduce_statements(std::vector<ScrStatement>&& statements, const std::vector<RuleRef>& rules)
{
	std::vector<ScrStatement> result;

	for (auto& statement : statements)
	{
		result.push_back(std::move(statement));

		for (auto& rule : rules)
		{
			unsigned statementCount = rule.from.statement_count();

			if (statementCount > result.size())
				continue;

			if (match_statements(rule.from, result.data() + result.size() - statementCount, result.data() + result.size()))
			{

			}
		}
	}

	return result;
}

// */

} // namespace pattern
} // namespace mary
