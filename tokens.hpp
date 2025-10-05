#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <variant>

struct T_KW { std::string value; }; // reserved keyword
struct T_VAR { std::string value; };
struct T_INT { int64_t value; }; // enforce signed 64 bit?
struct T_BOOL { bool value; }; // boolean
struct T_BOP { std::string value; }; // infix binary operator
struct T_UOP { std::string value; }; // unitary operator
struct T_LPAR {}; // left parents
struct T_RPAR {};
struct T_LBRAC {}; // left bracket
struct T_RBRAC {};
struct T_ASMT {}; // assignment
struct T_SCOL {}; // semicolon to end command
struct T_RLCOM {}; // right bracket for long comment
struct T_LLCOM {}; // left bracket for long comment
struct T_COM { std::string value; } // actual comment?
struct T_SCOM {}; // -- to begin short, end of line comment 

using Token = std::variant<
    T_KW, T_VAR, T_INT, T_BOP, T_UOP, T_LPAR, T_RPAR, T_LBRAC,
    T_BRAC, T_ASMT, T_SCOL, T_RLCOM, T_LLCOM, T_COM, T_SCOM
>;

#endif