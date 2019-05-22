
# `mary`

Decompiles and compiles (eventually) event scripts for Harvest Moon: Friends of Mineral Town and Harvest Moon: More Friends of Mineral Town.

This version is very work in progress and is not ready for use. It can only do basic decompilation for now.

## CLI usage (for now):

### Script compilation

Not yet!

### Script decompilation

    mary anal <rom path> <script offset> <functions.mary path>

Will print the script to stdout.

[You can find the version of `functions.mary` (for MFoMT(U)) I use for research here](https://github.com/StanHash/FOMT-DOC/blob/master/tools/functions.mary). [(Read the doc!)](https://github.com/StanHash/FOMT-DOC)

## The script language

Supported comment syntaxes are C-style `/* comment */` and C++-style `// comment <newline>`.

Right now the only working part of the actual language is function declarations (for `functions.mary`):

    function <return type> <name>([<parameter type> [parameter name] [, ...]]) = <numerical identifier>;

- Function name is an identifier (all alphanumeric characters plus underscores allowed, except when a number is the first character)
- Typenames can also be any identifier (typenames `void`, `string`, `label` and `memaddr` have (or will have) special meaning (only `void` really has right now: it means the function doesn't push any value on the stack))
- The function's numerical identifier is a number literal. `0b` (binary), `0o` (octal) and `0x` (hex) prefixes are supported.
- Parameters are for defining what the function reads from the stack. *They are in the order in which they are pushed*.

## Compiling `mary`

Having `cmake`, `bison`, `re2c` and a working C++14 compiler:

    mkdir build
    cd build
    cmake ..
    cmake --build .
