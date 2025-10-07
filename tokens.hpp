#ifndef TOKENS_HPP
#define TOKENS_HPP

#include <string>
#include <variant>
#include <unordered_map>
#include <vector>
#include <cstdint>

// FIX ME
enum class TokenType {
    COMMAND, ASSIGNMENT, VAR, INT, BOOL, COMMENT, INFBOOLEXPR, ARITHEXPR,
    BOOLEXPR, BRACKET, SCOL
};

struct T_KW   { std::string value; TokenType type = TokenType::COMMAND; }; // reserved keyword
struct T_VAR  { std::string value; TokenType type = TokenType::VAR; }; // variable
struct T_INT  { int64_t value; TokenType type = TokenType::INT; };     // enforce signed 64 bit
struct T_BOOL { bool value; TokenType type = TokenType::BOOL; };
struct T_BOP  { std::string value; TokenType type = TokenType::BOOLEXPR; }; // infix binary operator
struct T_UOP  { std::string value; TokenType type = TokenType::BOOLEXPR; }; // unary operator
struct T_LPAR { TokenType type = TokenType::BRACKET; }; // left parenthesis
struct T_RPAR { TokenType type = TokenType::BRACKET; };
struct T_LBRAC { TokenType type = TokenType::BRACKET; };
struct T_RBRAC { TokenType type = TokenType::BRACKET; };
struct T_ASMT { TokenType type = TokenType::ASSIGNMENT; };
struct T_SCOL { TokenType type = TokenType::SCOL; };
struct T_RLCOM { TokenType type = TokenType::COMMENT; };
struct T_LLCOM { TokenType type = TokenType::COMMENT; };
struct T_COM  { std::string value; TokenType type = TokenType::COMMENT; }; // comment body
struct T_SCOM { TokenType type = TokenType::COMMENT; }; // short comment ("-- ...")

using Token = std::variant<
    T_KW, T_VAR, T_INT, T_BOOL, T_BOP, T_UOP,
    T_LPAR, T_RPAR, T_LBRAC, T_RBRAC,
    T_ASMT, T_SCOL, T_RLCOM, T_LLCOM, T_COM, T_SCOM
>;

inline const std::vector<std::string> KEYWORDS = {
    "while", "do", "od", "true", "false", "not", "and", "or",
    "skip", "if", "then", "else", "fi"
};

inline const std::unordered_map<std::string, Token> KW_TO_TOKENS = {
    {"true",  T_BOOL{true}},
    {"false", T_BOOL{false}},
    {"not",   T_UOP{"not"}},
    {"and",   T_BOP{"and", TokenType::BOOLEXPR}},
    {"or",    T_BOP{"or", TokenType::BOOLEXPR}},
    {"skip",  T_KW{"skip"}},
    {"if",    T_KW{"if"}},
    {"then",  T_KW{"then"}},
    {"else",  T_KW{"else"}},
    {"fi",    T_KW{"fi"}},
    {"while", T_KW{"while"}},
    {"do",    T_KW{"do"}},
    {"od",    T_KW{"od"}}
};

inline const std::unordered_map<std::string, Token> BINARY_OPS = {
    {"+",  T_BOP{"+", TokenType::ARITHEXPR}},
    {"-",  T_BOP{"-", TokenType::ARITHEXPR}},
    {"*",  T_BOP{"*", TokenType::ARITHEXPR}},
    {"=",  T_BOP{"=", TokenType::INFBOOLEXPR}},
    {"<",  T_BOP{"<", TokenType::INFBOOLEXPR}},
    {"<=", T_BOP{"<=",TokenType::INFBOOLEXPR}},
    {">",  T_BOP{">", TokenType::INFBOOLEXPR}},
    {">=", T_BOP{">=", TokenType::INFBOOLEXPR}}
};

#endif
