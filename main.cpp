
#include <iostream>
#include <cstring>
#include <vector>

#include <memory>
#include <set>
#include <list>
#include <array>
#include <iomanip>

#include "core/riff.h"
#include "core/bytecode.h"
#include "ast/scr-pattern.h"
#include "lang/parse.h"

namespace mary {

void print_expression(std::ostream& out, const ScrExpr& expr)
{
	switch (expr.exprClass)
	{

	case SCR_EXPR_LITERAL:
		out << expr.literal.value;
		break;

	case SCR_EXPR_DEREF:
		out << "[";
		print_expression(out, *expr.compound.children[0]);
		out << "]";
		break;

	case SCR_EXPR_ADD:
		out << "(";
		print_expression(out, *expr.compound.children[0]);
		out << " + ";
		print_expression(out, *expr.compound.children[1]);
		out << ")";
		break;

	case SCR_EXPR_EQU:
		print_expression(out, *expr.compound.children[0]);
		out << " = ";
		print_expression(out, *expr.compound.children[1]);
		break;

	case SCR_EXPR_FUNC:
		print_expression(out, *expr.compound.children[0]);
		out << "(";

		for (unsigned i = 1; i < expr.compound.children.size(); ++i)
		{
			if (i > 1)
				out << ", ";
			print_expression(out, *expr.compound.children[i]);
		}

		out << ")";
		break;

	} // switch (expr.exprClass)
}

void print_opcode_statement(std::ostream& out, const ScrStatement& statement)
{
	if (statement.stmtClass >= SCR_OPCODE_MAX)
		throw std::runtime_error("tried to print a complex statement as a core instruction"); // TODO: better error

	out << get_opcode_info(statement.stmtClass).mnemonic;

	for (auto& expr : statement.children)
	{
		out << " ";
		print_expression(out, *expr);
	}
}

void print_statement(std::ostream& out, const ScrStatement& statement)
{
	if (statement.stmtClass < SCR_OPCODE_MAX)
	{
		print_opcode_statement(out, statement);
	}
	else
	{
		switch (statement.stmtClass)
		{

		case SCR_STMT_EXPR:
			print_expression(out, *statement.children[0]);
			break;

		} // switch (statement.stmtClass)
	}

	out << ";" << std::endl; // /* " << statement.stmtClass << " */" << std::endl;
}

ScrStatement make_statement(const ScrIns& ins)
{
	switch (ins.opcode)
	{

	case SCR_OPCODE_PUSH8:
	case SCR_OPCODE_PUSH16:
		return ScrStatement(SCR_OPCODE_PUSH, make_literal_expr(ins.operand));

	default:
		if (ins.has_operand())
			return ScrStatement(ins.opcode, make_literal_expr(ins.operand));

		return ScrStatement(ins.opcode);

	} // switch (ins.opcode)
}

std::vector<ScrStatement> make_statements(const Span<ScrIns>& script)
{
	std::vector<ScrStatement> result;

	for (auto& ins : script)
		result.push_back(make_statement(ins));

	return result;
}

} // namespace mary

struct ProgramOptions
{
	std::vector<std::string> positionals;

	bool isHelp = false;
};

ProgramOptions parse_arguments(const char* const* begin, const char* const* end)
{
	ProgramOptions result;

	for (auto it = begin; it != end; ++it)
	{
		if (strlen(*it) == 0)
			continue;

		if ((*it)[0] == '-')
		{
			std::string flag = (*it) + 1;

			if (flag == "h" || flag == "-help")
			{
				result.isHelp = true;
			}
		}
		else
		{
			result.positionals.emplace_back(*it);
		}
	}

	return result;
}

void print_usage(std::ostream& out, const std::string& programName)
{
	out << "usage:" << std::endl
		<< "  " << programName << " anal <rom> <offset> <functions.mary>" << std::endl;
}

int main(int argc, char** argv)
{
	auto options = parse_arguments(argv + 1, argv + argc);

	auto error = [argv] ()
	{
		print_usage(std::cerr, argv[0]);
		return 1;
	};

	if (options.isHelp)
	{
		print_usage(std::cout, argv[0]);
		return 0;
	}

	if (options.positionals.size() == 0)
		return error();

	if (options.positionals[0] == "anal")
	{
		/*
		 * DECOMPILATION STAGES:
		 * - extract chunks from RIFF (CODE, JUMP, STR)
		 * - decode instructions from CODE chunk
		 * - split instructions into linear blocks (split points are jumps and jump targets)
		 *   - TODO: parse JUMP for more accurate splitting
		 *   - TODO: figure out what to do with the bi (indirect branch) instruction
		 * - convert instructions to statements with literal expressions
		 *   - each instruction map to a statement with the corresponding class (exception: push8 and push16 are simplified to generic push)
		 *   - TODO: parse JUMP for more elaborate conversion of the switch instruction
		 *   - TODO: handle X
		 * - combine statements into less statements using cool pattern matching mechanism (ex: push A; push B; add; into push (A + B);)
		 * - TODO: control flow analysis
		 * - TODO: expand literal expressions of certain value types to declared identifiers (functions, strings, variables)
		 *   - TODO: use STR chunk for string declarations
		 */

		if (options.positionals.size() <= 3)
			return error();

		try {
			std::string romPath = options.positionals[1];

			std::uint64_t romOffset = (options.positionals.size() > 2)
				? 0x1FFFFFF & (std::stoll(options.positionals[2], nullptr, 0))
				: 0;

			auto riff = mary::unpack_riff(romPath, romOffset);

			auto funcs = mary::parse_file(options.positionals[3]).functions;
			auto funcPatterns = mary::pattern::get_function_patterns(funcs);
			auto rules = mary::pattern::get_decompile_rules(funcPatterns);

			for (auto& chunk : riff.chunks)
			{
				if (chunk.name == "CODE")
				{
					auto script = mary::decode_script(chunk.data.data() + 4, chunk.data.data() + chunk.data.size());
					auto slices = mary::slice_script(script);

					for (auto& chunk : slices)
					{
						std::cout << "loc_" << mary::get_script_offset(chunk) << ":" << std::endl;

						auto statements = mary::pattern::reduce_statements(
							rules, mary::make_statements(chunk));

						for (auto& stmt : statements)
							mary::print_statement(std::cout, stmt);
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "An error occured: " << e.what() << std::endl;
			return 1;
		}

		return 0;
	}

	return 0;
}
