
/* TODO: location tracking */

%skeleton "lalr1.cc"

%define api.token.constructor

%define api.value.type variant

%define parse.assert
%define parse.error verbose

%code requires {

#include "lang/parse.h"
#include "ast/function.h"

#include <istream>
#include <sstream>
#include <fstream>

namespace mary {

template<unsigned FillSize>
struct ParseScanner
{
	constexpr static std::size_t buffer_size = 0x1000;

	ParseScanner(std::istream& source)
		: source(source), limit(buffer.begin() + buffer_size), token(limit), cursor(limit), marker(limit), eof(false) {}

	bool fill(std::size_t amount)
	{
		if (eof)
			return false;

		// is free the area before the current token (aka what was already processed)
		const auto free = token - buffer.data();

		if (free < amount)
			return false;

		// effectively clean the processed area

		std::copy(token, limit, buffer.data());

		// update iterators to reflect the change

		cursor -= free;
		marker -= free;
		limit  -= free;
		token  -= free;

		limit += source.read(
			/* dest */ buffer.data() + (limit - buffer.data()),
			/* size */ buffer.data() + buffer.size() - limit).gcount();

		if (limit < (buffer.data() + buffer_size))
		{
			// we read less than we should have, we reached end of stream
			// fill the rest of the buffer with zeroes so that nothing explodes, and set eof

			limit = std::fill_n(
				/* dest  */ buffer.data() + (limit - buffer.data()),
				/* size  */ FillSize,
				/* value */ 0);

			eof = true;
		}

		return true;
	}

	std::array<char, buffer_size + FillSize> buffer;
	std::istream& source; // TODO: use something else instead?

	const char* limit;  // end of current working area
	const char* token;  // start of current token / end of discarded area
	const char* cursor; // last read byte
	const char* marker; // idk

	bool eof;
};

} // namespace mary

/*!max:re2c*/

struct ParseCtx
{
	mary::ParseScanner<YYMAXFILL> scanner;
	mary::ParseResult result;

	const mary::ParseConfig& config;
	bool guardEmitted = false;
};

} // %code requires

%param { ParseCtx& ctx }

%define api.token.prefix {TOKEN_}

%token <std::string>  IDENTIFIER
%token <std::int32_t> INTEGER

%token
	EOF 0 "end of file"

	/* punctuation/operators */
	LPAREN    "("
	RPAREN    ")"
	COMMA     ","
	EQUAL     "="
	SEMICOLON ";"

	/* keywords */
	FUNCTION "function"
	TRUE     "true"
	FALSE    "false"
	SCRIPT   "script"

	/* dummy "guard" token */
	DECLARATIONS_ONLY "declarations only"
;


%type <unsigned>                     typename
%type <mary::TypedName>              function_decl_param
%type <std::vector<mary::TypedName>> function_decl_params
%type <mary::Function>               function_decl

%code {

namespace yy { parser::symbol_type yylex(ParseCtx& ctx); }

}

%%

file:
    declarations /* TODO */
  | DECLARATIONS_ONLY declarations
;

declarations:
    %empty
  | declarations function_decl { ctx.result.functions.push_back(std::move($2)); }
;

function_decl:
    "function" typename IDENTIFIER "(" function_decl_params ")" "=" INTEGER ";"
    {
        $$ = {
            /* return type */ (unsigned) ($2),
            /* parameters  */ std::move($5),
            /* name        */ std::move($3),
            /* identifier  */ (unsigned) ($8)
        };
    }
;

function_decl_params:
    %empty                                       { $$ = {}; }
  | function_decl_param                          { $$ = { std::move($1) }; }
  | function_decl_params "," function_decl_param { $$ = std::move($1); $$.push_back(std::move($3)); }
;

function_decl_param:
    typename            { $$ = { $1, {} }; }
  | typename IDENTIFIER { $$ = { $1, $2 }; }
;

typename:
    IDENTIFIER { $$ = mary::get_named_type($1); }
;

%%

template<unsigned Base>
static std::int32_t lex_int_simple(const char* start, const char* end)
{
	std::int32_t result { 0 };

	while (start != end)
		result = ((*start++) - '0') + (result * Base);

	return result;
}

static std::int32_t lex_int_hex(const char* start, const char* end)
{
	std::int32_t result { 0 };

	while (start != end)
	{
		/*!re2c
			re2c:define:YYCTYPE = char;

			re2c:define:YYCURSOR = start;
			re2c:define:YYLIMIT = end;

			re2c:yyfill:enable = 0;

			*     { result = (result * 16) + (start[-1] - '0'); continue; }
			[a-f] { result = (result * 16) + 10 + (start[-1] - 'a'); continue; }
			[A-F] { result = (result * 16) + 10 + (start[-1] - 'A'); continue; }
		*/
	}

	return result;
}

yy::parser::symbol_type yy::yylex(ParseCtx& ctx)
{
	if (!ctx.guardEmitted)
	{
		ctx.guardEmitted = true;

		if (ctx.config.noScript)
			return yy::parser::make_DECLARATIONS_ONLY();
	}

	ctx.scanner.token = ctx.scanner.cursor;

	/*!re2c
		re2c:define:YYCTYPE = char;

		re2c:define:YYCURSOR = ctx.scanner.cursor;
		re2c:define:YYMARKER = ctx.scanner.marker;
		re2c:define:YYLIMIT  = ctx.scanner.limit;

		re2c:yyfill:enable = 1;

		re2c:define:YYFILL = "if (!ctx.scanner.fill(@@)) throw false;"; // FIXME
		re2c:define:YYFILL:naked = 1;

		end = "\x00";

		*   { throw std::runtime_error("ugh"); } // TODO: better error
		end { return yy::parser::make_EOF(); }

		// whitespaces & comments
		// TODO: maybe handle comments separately

		mcm = "/""*" ([^*] | ("*" [^/]))* "*""/";
		scm = "//" [^\n]* "\n";
		whitespace = ([ \t\v\n\r] | scm | mcm)+;

		whitespace { return yylex(ctx); } // ignore

		// integer literals

		int_bin_prefix = '0b' [01]+;
		int_oct_prefix = '0o' [0-7]+;
		int_hex_cstyle = '0x' [0-9a-fA-F]+;
		int_dec = [0-9]+;

		int_bin_prefix
		{
			return yy::parser::make_INTEGER(lex_int_simple<2>(ctx.scanner.token + 2, ctx.scanner.cursor));
		}

		int_oct_prefix
		{
			return yy::parser::make_INTEGER(lex_int_simple<8>(ctx.scanner.token + 2, ctx.scanner.cursor));
		}

		int_hex_cstyle
		{
			return yy::parser::make_INTEGER(lex_int_hex(ctx.scanner.token + 2, ctx.scanner.cursor));
		}

		int_dec
		{
			return yy::parser::make_INTEGER(lex_int_simple<10>(ctx.scanner.token, ctx.scanner.cursor));
		}

		// (unused integer literals)
		// int_bin_postfx = [01]+ 'b';
		// int_hex_dollar = "$" [0-9a-fA-F]+;

		// keywords

		"function" { return yy::parser::make_FUNCTION(); }
		"true"     { return yy::parser::make_TRUE(); }
		"false"    { return yy::parser::make_FALSE(); }
		"script"   { return yy::parser::make_SCRIPT(); }

		// operators & punctuation

		"(" { return yy::parser::make_LPAREN(); }
		")" { return yy::parser::make_RPAREN(); }
		"," { return yy::parser::make_COMMA(); }
		";" { return yy::parser::make_SEMICOLON(); }
		"=" { return yy::parser::make_EQUAL(); }

		// identifiers

		identifier = [a-zA-Z_][a-zA-Z0-9_]*;

		identifier
		{
			return yy::parser::make_IDENTIFIER(std::string(ctx.scanner.token, ctx.scanner.cursor));
		}
	*/
}

void yy::parser::error(const std::string& msg)
{
    std::cerr << msg << std::endl;
}

namespace mary {

ParseResult parse_string(const std::string& string, const ParseConfig& config)
{
	std::istringstream ss(string);
	ParseCtx ctx { ss, {}, config };

	yy::parser parser(ctx);
	parser.parse();

	return std::move(ctx.result);
}

ParseResult parse_file(const std::string& filename, const ParseConfig& config)
{
	std::ifstream fs(filename);

	if (!fs.is_open())
		throw std::runtime_error(std::string("failed to open file: ") + filename); // TODO: better error

	ParseCtx ctx { fs, {}, config };

	yy::parser parser(ctx);
	parser.parse();

	return std::move(ctx.result);
}

} // namespace mary
