cmake_minimum_required(VERSION 3.1)

project(mary)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

find_package(RE2C REQUIRED)
find_package(BISON REQUIRED)

RE2C_TARGET(
	NAME MARY_LEX

	INPUT "${CMAKE_CURRENT_SOURCE_DIR}/lang/parser.re.yy"
	OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/lang/parser.yy"

	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/lang/parse.h"
)

BISON_TARGET(
	MARY_PARSE

	"${CMAKE_CURRENT_SOURCE_DIR}/lang/parser.yy"
	"${CMAKE_CURRENT_SOURCE_DIR}/lang/parser.cpp"
)

set(SOURCE_LIST
    "main.cpp"

    "core/types.h"

    "core/riff.h"
    "core/riff.cpp"

    "core/bytecode.h"
    "core/bytecode.cpp"

    "ast/scr-statement.h"
    "ast/scr-expression.h"

    "ast/scr-pattern.h"
    "ast/scr-pattern.cpp"

    "ast/scr-patterns.cpp"

    "ast/function.h"

	"lang/parse.h"

	"lang/parser.cpp"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
