#ifndef MARY_SCR_EXPRESSION_H
#define MARY_SCR_EXPRESSION_H

#include "core/types.h"

#include <memory>
#include <vector>

namespace mary {

enum
{
	// Expression type identifiers

	// TODO: add other forms of expressions such as references to variables/constants

	SCR_EXPR_INVALID,

	// No children
	SCR_EXPR_LITERAL,
	SCR_EXPR_NAMED,

	// 1 child (unary operators)
	SCR_EXPR_DEREF,
	SCR_EXPR_NEG,
	SCR_EXPR_NOT,

	// 2 children (binary operators)
	SCR_EXPR_EQU,
	SCR_EXPR_ADDEQU,
	SCR_EXPR_SUBEQU,
	SCR_EXPR_MULEQU,
	SCR_EXPR_DIVEQU,
	SCR_EXPR_MODEQU,
	SCR_EXPR_ADD,
	SCR_EXPR_SUB,
	SCR_EXPR_MUL,
	SCR_EXPR_DIV,
	SCR_EXPR_MOD,
	SCR_EXPR_AND,
	SCR_EXPR_OR,
	// SCR_EXPR_CMP,

	// Unknown amount of children
	SCR_EXPR_FUNC,

	SCR_EXPR_MAX,
};

enum
{
	SCR_VALTYPE_ANY,
	SCR_VALTYPE_MEMADDR,
	SCR_VALTYPE_LABEL,
	SCR_VALTYPE_STRID,
	SCR_VALTYPE_FUNCID,
	SCR_VALTYPE_VOID,
};

struct ScrValue
{
	std::int32_t value;
	unsigned     typeClass;

	bool operator < (const ScrValue& other) const
	{
		if (typeClass < other.typeClass)
			return true;

		return value < other.value;
	}
};

struct ScrExpr
{
	ScrExpr(unsigned exprClass)
		: exprClass(exprClass) {}

	inline
	bool is_literal() const
	{
		return exprClass == SCR_EXPR_LITERAL;
	}

	inline
	bool is_named() const
	{
		return exprClass == SCR_EXPR_NAMED;
	}

	inline
	bool is_compound() const
	{
		return exprClass != SCR_EXPR_LITERAL && exprClass != SCR_EXPR_NAMED;
	}

	inline
	unsigned child_count() const
	{
		return is_compound() ? compound.children.size() : 0;
	}

	// TODO: embed expression source location? (either bytecode offset or file:line:col)
	// TODO (C++17): use std::variant

	// Literal
	ScrValue literal;

	// Named
	std::string named;

	// Compound
	struct
	{
		std::vector<std::unique_ptr<ScrExpr>> children;
	}
	compound;

	unsigned exprClass;
};

using ExpressionSpan = Span<std::unique_ptr<ScrExpr>>;

inline
std::unique_ptr<ScrExpr> make_compound_expr(unsigned exprClass, Span<std::unique_ptr<ScrExpr>>&& children)
{
	auto result = std::make_unique<ScrExpr>(exprClass);

	result->compound.children.assign(std::make_move_iterator(children.begin()), std::make_move_iterator(children.end()));

	return result;
}

inline
std::unique_ptr<ScrExpr> make_literal_expr(std::size_t value, unsigned type = SCR_VALTYPE_ANY)
{
	auto result = std::make_unique<ScrExpr>(SCR_EXPR_LITERAL);

	result->literal.value     = value;
	result->literal.typeClass = type;

	return result;
}

inline
std::unique_ptr<ScrExpr> make_named_expr(std::string&& value)
{
	auto result = std::make_unique<ScrExpr>(SCR_EXPR_NAMED);

	result->named = std::move(value);

	return result;
}

} // namespace mary

#endif // MARY_SCR_EXPRESSION_H
