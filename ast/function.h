#ifndef MARY_FUNCTION_H
#define MARY_FUNCTION_H

#include "core/types.h"
#include "ast/scr-expression.h"

#include <string>
#include <vector>

namespace mary {

struct TypedName
{
	unsigned typeClass;
	std::string name;
};

struct Function
{
	unsigned resultType;
	std::vector<TypedName> parameters;

	std::string name;
	unsigned identifier;
};

inline
unsigned get_named_type(const std::string& name)
{
	if (name == "void")
		return SCR_VALTYPE_VOID;

	if (name == "string")
		return SCR_VALTYPE_STRID;

	if (name == "memaddr")
		return SCR_VALTYPE_MEMADDR;

	if (name == "label")
		return SCR_VALTYPE_LABEL;

	return SCR_VALTYPE_ANY;
}

} // namespace mary

#endif // MARY_FUNCTION_H
