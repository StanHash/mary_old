#ifndef MARY_SCR_STATEMENT_H
#define MARY_SCR_STATEMENT_H

#include "core/bytecode.h"
#include "ast/scr-expression.h"

namespace mary {

enum
{
	// Statement type identifiers
	// All raw script instructions also work as statements

	SCR_STMT_EXPR = SCR_OPCODE_MAX,
	SCR_STMT_BRANCHIF,

	SCR_STMT_MAX,
};

struct ScrStatement
{
	ScrStatement()
		: stmtClass(SCR_STMT_MAX) {}

	ScrStatement(unsigned id)
		: stmtClass(id) {}

	ScrStatement(unsigned id, std::unique_ptr<ScrExpr>&& child)
		: stmtClass(id) { children.push_back(std::move(child)); }

	ScrStatement(unsigned id, Span<std::unique_ptr<ScrExpr>>&& child)
		: children(std::make_move_iterator(child.begin()), std::make_move_iterator(child.end())), stmtClass(id) {}

	std::vector<std::unique_ptr<ScrExpr>> children;

	unsigned stmtClass;
};

using StatementSpan = Span<ScrStatement>;

std::vector<ScrStatement> statements_from_instructions(Span<ScrIns> instructions);

} // namespace mary

#endif // MARY_SCR_STATEMENT_H
