
#include "core/riff.h"
#include "core/bytecode.h"
#include "core/offset-map.h"
#include "ast/scr-pattern.h"
#include "lang/parse.h"

#include <iostream>
#include <cstring>
#include <map>

namespace mary {

void print_expression(std::ostream& out, const ScrExpr& expr)
{
	switch (expr.exprClass)
	{

	case SCR_EXPR_LITERAL:
		out << expr.literal.value;
		break;

	case SCR_EXPR_NAMED:
		out << expr.named;
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

	case SCR_EXPR_SUB:
		out << "(";
		print_expression(out, *expr.compound.children[0]);
		out << " - ";
		print_expression(out, *expr.compound.children[1]);
		out << ")";
		break;

	case SCR_EXPR_MUL:
		out << "(";
		print_expression(out, *expr.compound.children[0]);
		out << " * ";
		print_expression(out, *expr.compound.children[1]);
		out << ")";
		break;

	case SCR_EXPR_DIV:
		out << "(";
		print_expression(out, *expr.compound.children[0]);
		out << " / ";
		print_expression(out, *expr.compound.children[1]);
		out << ")";
		break;

	case SCR_EXPR_MOD:
		out << "(";
		print_expression(out, *expr.compound.children[0]);
		out << " % ";
		print_expression(out, *expr.compound.children[1]);
		out << ")";
		break;

	case SCR_EXPR_EQU:
		print_expression(out, *expr.compound.children[0]);
		out << " = ";
		print_expression(out, *expr.compound.children[1]);
		break;

	case SCR_EXPR_ADDEQU:
		print_expression(out, *expr.compound.children[0]);
		out << " += ";
		print_expression(out, *expr.compound.children[1]);
		break;

	case SCR_EXPR_SUBEQU:
		print_expression(out, *expr.compound.children[0]);
		out << " -= ";
		print_expression(out, *expr.compound.children[1]);
		break;

	case SCR_EXPR_MULEQU:
		print_expression(out, *expr.compound.children[0]);
		out << " *= ";
		print_expression(out, *expr.compound.children[1]);
		break;

	case SCR_EXPR_DIVEQU:
		print_expression(out, *expr.compound.children[0]);
		out << " /= ";
		print_expression(out, *expr.compound.children[1]);
		break;

	case SCR_EXPR_MODEQU:
		print_expression(out, *expr.compound.children[0]);
		out << " %= ";
		print_expression(out, *expr.compound.children[1]);
		break;

	case SCR_EXPR_NEG:
		out << "-";
		print_expression(out, *expr.compound.children[0]);
		break;

	case SCR_EXPR_NOT:
		out << "!";
		print_expression(out, *expr.compound.children[0]);
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

	out << get_opcode_class(statement.stmtClass).mnemonic;

	for (auto& expr : statement.children)
	{
		out << " ";
		print_expression(out, *expr);
	}
}

void print_statement(std::ostream& out, const ScrStatement& statement)
{
	if (statement.has_label())
		std::cout << statement.label << ":" << std::endl;

	std::cout << "    ";

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

ScrStatement make_statement(const NameMap& labels, const ScrIns& ins)
{
	switch (ins.opcode)
	{

	case SCR_OPCODE_PUSH8:
	case SCR_OPCODE_PUSH16:
		return ScrStatement(SCR_OPCODE_PUSH, make_literal_expr(ins.operand));

	default:
		if (ins.has_operand())
		{
			if (ins.is_jump())
			{
				// pulling operand from labels instead of using the offset

				ScrStatement result(ins.opcode);

				labels.for_at(ins.operand, [&] (auto& name)
				{
					result.children.push_back(make_named_expr(std::string(name)));
				});

				return result;
			}

			return ScrStatement(ins.opcode, make_literal_expr(ins.operand));
		}

		return ScrStatement(ins.opcode);

	} // switch (ins.opcode)
}

std::vector<ScrStatement> make_statements(const NameMap& labels, Span<const ScrIns> script)
{
	std::vector<ScrStatement> result;

	// TODO: more efficient label check (iterate linearily instead of bsearching for each statement)

	auto makeStatement = [&labels] (auto& ins)
	{
		auto result = make_statement(labels, ins);

		labels.for_at(ins.offset, [&result] (const std::string& name)
		{
			result.label = name;
		});

		return result;
	};

	for (auto& ins : script)
		result.push_back(makeStatement(ins));

	return result;
}

std::map<ScrValue, std::string> functions_to_names(Span<const Function> functions)
{
	std::map<ScrValue, std::string> result;

	for (auto& function : functions)
		result[{ (std::int32_t) function.identifier, SCR_VALTYPE_FUNCID }] = function.name;

	return result;
}

void name_expression_literals(const std::map<ScrValue, std::string>& names, Span<std::unique_ptr<ScrExpr>> exprs)
{
	for (auto& expr : exprs)
	{
		if (expr->is_literal())
		{
			auto it = names.find(expr->literal);

			if (it != names.end())
				expr = make_named_expr(std::string(it->second));
		}
		else if (expr->is_compound())
		{
			name_expression_literals(names, expr->compound.children);
		}
	}
}

void name_literals(const std::map<ScrValue, std::string>& names, Span<ScrStatement> statements)
{
	for (auto& statement : statements)
		name_expression_literals(names, statement.children);
}

using ScrBasicBlock = std::vector<ScrStatement>;

struct ScrFlowGraph
{
	// All elements have the same input (implicit) and output (explicit)
	// The first node is always index 0, and should be the sole common dominator of all other nodes

	struct Node
	{
		enum class Kind
		{
			Subgraph,
			Basic,
		};

		Node(ScrBasicBlock&& basic)
			: kind(Kind::Basic), basic(std::move(basic)) {}

		Node(std::unique_ptr<ScrFlowGraph>&& subgraph)
			: kind(Kind::Subgraph), subgraph(std::move(subgraph)) {}

		// TODO (C++17): use std::variant
		Kind kind;

		std::unique_ptr<ScrFlowGraph> subgraph;
		ScrBasicBlock basic;

		std::vector<unsigned> outNodes;
	};

	std::vector<Node> nodes;

	static constexpr unsigned end_node_ref = std::numeric_limits<unsigned>::max();
};

constexpr unsigned ScrFlowGraph::end_node_ref;

ScrFlowGraph make_flow_graph(const ScrAnalysis& anal)
{
	ScrFlowGraph result;
	OffsetMap<unsigned> nodeMap;

	for (auto& mappedBlock : anal.linearChunks)
	{
		nodeMap.set(mappedBlock.first, result.nodes.size());
		result.nodes.emplace_back(make_statements(anal.labels, mappedBlock.second));
	}

	for (auto& mappedBlock : anal.linearChunks)
	{
		auto& lastIns = mappedBlock.second.back();
		auto& node = result.nodes[nodeMap.get(mappedBlock.first)->second];

		auto add_offset_node = [&] (unsigned offset)
		{
			auto it = nodeMap.get(offset);

			if (it != nodeMap.end())
				node.outNodes.push_back(it->second);
		};

		if (lastIns.has_operand() && lastIns.is_jump())
		{
			// this is a jump with operand, add node referred by operand
			add_offset_node(lastIns.operand);
		}

		if (!lastIns.is_end())
		{
			// this is not an end, add node just after
			add_offset_node(lastIns.offset + lastIns.encoded_size());
		}

		if (lastIns.opcode == SCR_OPCODE_END)
		{
			// this is the end, add end "node"
			node.outNodes.push_back(ScrFlowGraph::end_node_ref);
		}

		// TODO: if (lastIns.opcode == SCR_OPCODE_SWITCH)
	}

	return result;
}

/*
 * On generating subgraphs:
 * Algorithm idea:
 * Take the first node that isn't a subgraph (maybe?)
 * that node and everything between it and its immediate postdominator (excluded) becomes a subgraph
 * but how do I find that immediate postdominator?
 * using http://www.hipersoft.rice.edu/grads/publications/dom14.pdf
 */

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

	auto argError = [argv] ()
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
		return argError();

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
			return argError();

		try
		{
			std::string romPath = options.positionals[1];
			std::uint64_t romOffset = std::stoll(options.positionals[2], nullptr, 0) & 0x1FFFFFF;
			std::string definitionsPath = options.positionals[3];

			auto riff = mary::unpack_riff(romPath, romOffset);
			auto funcs = mary::parse_file(definitionsPath).functions;

			auto funcPatterns = mary::pattern::get_function_patterns(funcs);
			auto rules = mary::pattern::get_decompile_rules(funcPatterns);

			auto names = mary::functions_to_names(funcs);

			for (auto& chunk : riff.chunks)
			{
				if (chunk.name == "CODE")
				{
					auto script = mary::decode_script({ chunk.data.data() + 4, chunk.data.size() - 4});
					auto anal = mary::analyse_script(script);

					auto flow = mary::make_flow_graph(anal);

					for (unsigned i = 0; i < flow.nodes.size(); ++i)
					{
						auto& node = flow.nodes[i];

						switch (node.kind) {

						case mary::ScrFlowGraph::Node::Kind::Basic:
						{
							std::cout << "/* --- BASIC " << i << " --- */" << std::endl;

							node.basic = mary::pattern::reduce_statements(rules, std::move(node.basic));
							mary::name_literals(names, node.basic);

							for (auto& stmt : node.basic)
								mary::print_statement(std::cout, stmt);

							std::cout << "/* --- OUT ";

							for (unsigned j = 0; j < node.outNodes.size(); ++j)
							{
								if (j > 0)
									std::cout << ", ";

								if (node.outNodes[j] == mary::ScrFlowGraph::end_node_ref)
									std::cout << "end";
								else
									std::cout << node.outNodes[j];
							}

							if (node.outNodes.empty())
								std::cout << "-";

							std::cout << " --- */" << std::endl;
							break;
						}

						case mary::ScrFlowGraph::Node::Kind::Subgraph:
						{
							break;
						}

						} // switch switch (node.kind)
					}

					if (false) // yay for dead code
					{
						for (auto& chunk : anal.linearChunks)
						{
							std::cout << "/* ------ " << chunk.first << " ------ */" << std::endl;

							auto statements = mary::pattern::reduce_statements(
								rules, mary::make_statements(anal.labels, chunk.second));

							mary::name_literals(names, statements);

							for (auto& stmt : statements)
								mary::print_statement(std::cout, stmt);
						}
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
