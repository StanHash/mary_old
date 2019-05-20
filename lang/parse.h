#ifndef MARY_PARSER_H
#define MARY_PARSER_H

#include "core/types.h"

#include "ast/function.h"

namespace mary {

struct ParseResult
{
	std::vector<Function> functions;
};

struct ParseConfig
{
	ParseConfig()
		: noScript(false) {}

	bool noScript : 1; //!< only parse function & type declarations
};

ParseResult parse_string(const std::string& string, const ParseConfig& config = {});
ParseResult parse_file(const std::string& filename, const ParseConfig& config = {});

} // namespace mary

#endif // MARY_PARSER_H
