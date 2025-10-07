#ifndef PARSER_HPP
#define PARSER_HPP

#include "tokens.hpp"
#include "scan.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

inline bool isComment(const Token& token){
    return std::holds_alternative<T_RLCOM>(token)
        || std::holds_alternative<T_SCOM>(token)
        || std::holds_alternative<T_COM>(token)
        || std::holds_alternative<T_LLCOM>(token);
}

inline bool isKeyword(const Token& t, const std::string& kw) {
    return std::holds_alternative<T_KW>(t)
        && std::get<T_KW>(t).value == kw;
}

struct AST{
    std::string type;
    std::string value;
    std::vector<AST> children;
};

AST parseCommand(const std::vector<Token>& tokens, size_t& pos);
AST parseBoolExpression(const std::vector<Token>& tokens, size_t& pos);
AST parseArithExpression(const std::vector<Token>& tokens, size_t& pos);
AST parseTerm(const std::vector<Token>& tokens, size_t& pos);
AST parseFactor(const std::vector<Token>& tokens, size_t& pos);
AST parseProgram(const std::vector<Token>& tokens, size_t& pos);

inline AST parse(const std::vector<Token>& tokens){
    size_t pos = 0;
    return parseProgram(tokens, pos);
}


AST parseProgram(const std::vector<Token>& tokens, size_t& pos) {
    AST programNode;
    programNode.type = "Program";

    while (pos < tokens.size()) {
        if(isComment(tokens[pos])) {
            ++pos;
            continue;
        }
        AST commandNode = parseCommand(tokens, pos);
        programNode.children.push_back(commandNode);

        if (pos < tokens.size() && std::holds_alternative<T_SCOL>(tokens[pos])) {
            ++pos;
        }
    }

    return programNode;
}

AST parseCommandSequence(const std::vector<Token>& tokens, size_t& pos) {
    AST seq;
    seq.type = "command";

    while (pos < tokens.size()) {
        if (isKeyword(tokens[pos], "od") || isKeyword(tokens[pos], "else") || isKeyword(tokens[pos], "fi")) {
            break; 
        }
        AST cmd = parseCommand(tokens, pos);
        seq.children.push_back(cmd);

        if (pos < tokens.size() && std::holds_alternative<T_SCOL>(tokens[pos])) {
            ++pos; 
        } else {
            break; 
        }
    }

    return seq;
}

AST parseCommand(const std::vector<Token>& tokens, size_t& pos) {
    auto& token = tokens[pos];
    AST node;

    // if (isComment(token)) {
    //     node.type = "Comment";
    //     node.value = "<comment>";
    //     ++pos;
    //     return node;
    // }

    if (std::holds_alternative<T_VAR>(token)) {
        std::string varName = std::get<T_VAR>(token).value;
        ++pos;
        if (pos < tokens.size() && std::holds_alternative<T_ASMT>(tokens[pos])) {
            ++pos;
            AST expr = parseArithExpression(tokens, pos);
            node.type = "assignment";
            node.value = varName;
            node.children.push_back(expr);
            return node;
        } else {
            std::cerr << "Expected ':=' after variable in assignment" << std::endl;
        }
    }

    if (isKeyword(token, "if")) {
        ++pos;
        AST cond = parseBoolExpression(tokens, pos);

        if (pos >= tokens.size() || !isKeyword(tokens[pos], "then")) {
            std::cerr << "Expected 'then' after if condition" << std::endl;
        }
        ++pos;

        AST thenCmd = parseCommandSequence(tokens, pos);

        if (pos >= tokens.size() || !isKeyword(tokens[pos], "else")) {
            std::cerr << "Expected 'else' after then" << std::endl;;
        }
        ++pos;
        AST elseCmd = parseCommandSequence(tokens, pos);
        if (pos >= tokens.size() || !isKeyword(tokens[pos], "fi")) {
            std::cerr << "Expected 'fi' after else" << std::endl;
        }
        ++pos;

        node.type = "if";
        node.children = {cond, thenCmd, elseCmd};
        return node;
    }

    if (isKeyword(token, "while")) {
        ++pos;
        AST cond = parseBoolExpression(tokens, pos);

        if (pos >= tokens.size() || !isKeyword(tokens[pos], "do")) {
            std::cerr << "Expected 'do' after while condition" << std::endl;
        }
        ++pos;

        AST body = parseCommandSequence(tokens, pos);

        if (pos >= tokens.size() || !isKeyword(tokens[pos], "od")) {
            std::cerr << "Expected 'od' after while body" << std::endl;
        }
        ++pos;

        node.type = "while";
        node.children = {cond, body};
        return node;
    }

    if (isKeyword(token, "skip")) {
        ++pos;
        node.type = "skip";
        node.value = "skip";
        return node;
    }

    std::cerr << "Unknown command" << std::endl;
}


AST parseBoolExpression(const std::vector<Token>& tokens, size_t& pos){
    auto& token = tokens[pos];
    AST node;

    if (std::holds_alternative<T_BOOL>(token)) {
        node.type = "bool";
        node.value = std::get<T_BOOL>(token).value ? "true" : "false";
        ++pos;
        return node;
    }else if (std::holds_alternative<T_UOP>(token)) {
        const std::string& uop = std::get<T_UOP>(token).value;
        if (uop == "not") {
            ++pos;
            if (pos >= tokens.size() || !std::holds_alternative<T_LBRAC>(tokens[pos])) {
                std::cerr << "'not' requires closed brackers." << std::endl;
            } else {
                ++pos;
                AST child = parseBoolExpression(tokens, pos);
                if (pos >= tokens.size() || !std::holds_alternative<T_RBRAC>(tokens[pos])) {
                    std::cerr << "'not' requires closed brackers."; 
                } else {
                    ++pos;
                    node.type = "not";
                    node.children.push_back(child);
                    return node;
                }
            }
        } else {
            std::cerr << "Unknown unary boolean operator" << std::endl;
        }
    }
    else if (std::holds_alternative<T_LBRAC>(token)) {
        ++pos;
        node = parseBoolExpression(tokens, pos);
        if (pos >= tokens.size() || !std::holds_alternative<T_RBRAC>(tokens[pos])) {
            std::cerr << "Missing closed brackets." << std::endl; 
        }
        ++pos;
        return node;
    }
    else {
        AST leftArith = parseArithExpression(tokens, pos);

        if (pos < tokens.size() && std::holds_alternative<T_BOP>(tokens[pos])) {
            const TokenType opType = std::get<T_BOP>(tokens[pos]).type;
            std::string val = std::get<T_BOP>(tokens[pos]).value;

            if (opType == TokenType::INFBOOLEXPR) {
                ++pos;
                AST rightArith = parseArithExpression(tokens, pos);
                AST node;
                node.type = "infbin";
                node.value = val;
                node.children = {leftArith, rightArith};
                return node;
            }

            if (opType == TokenType::BOOLEXPR) {
                ++pos;
                AST right = parseBoolExpression(tokens, pos);
                AST node;
                node.type = "boolexpr";
                node.value = val;
                node.children = {leftArith, right};
                return node;
            }
        }

        return leftArith;
    }
}


AST parseArithExpression(const std::vector<Token>& tokens, size_t& pos){
    AST left = parseTerm(tokens, pos);
    while (pos < tokens.size() && std::holds_alternative<T_BOP>(tokens[pos])) {
        TokenType opType = std::get<T_BOP>(tokens[pos]).type;
        std::string val = std::get<T_BOP>(tokens[pos]).value;
        if (opType == TokenType::ARITHEXPR) {
            ++pos;
            AST right = parseTerm(tokens, pos);
            AST node;
            node.type = "arithexpr";
            node.value = val;
            node.children = {left, right};
            left = node;
        } else {
            break;
        }
    }
    return left;
}

AST parseTerm(const std::vector<Token>& tokens, size_t& pos) {
    AST left = parseFactor(tokens, pos);
    while (pos < tokens.size() && std::holds_alternative<T_BOP>(tokens[pos])) {
        std::string op = std::get<T_BOP>(tokens[pos]).value;
        if (op == "*") {
            ++pos;
            AST right = parseFactor(tokens, pos);
            AST node;
            node.type = "term";
            node.value = "*";
            node.children = {left, right};
            left = node;
        } else {
            break;
        }
    }
    return left;
}

AST parseFactor(const std::vector<Token>& tokens, size_t& pos) {
    AST node;
    auto& token = tokens[pos];

    if (std::holds_alternative<T_LPAR>(token)) {
        ++pos;
        node = parseArithExpression(tokens, pos);
        if (pos >= tokens.size() || !std::holds_alternative<T_RPAR>(tokens[pos])) {
            std::cerr << "Missing ')' after expression" << std::endl;
        }
        ++pos;
        return node;
    }

    if (std::holds_alternative<T_INT>(token)) {
        node.type = "int";
        node.value = std::to_string(std::get<T_INT>(token).value);
        ++pos;
        return node;
    }

    if (std::holds_alternative<T_VAR>(token)) {
        node.type = "var";
        node.value = std::get<T_VAR>(token).value;
        ++pos;
        return node;
    }

    std::cerr << "Invalid factor" << std::endl;
}

inline void printAST(const AST& node, int indent = 0) {
    for (int i = 0; i < indent; ++i) std::cout << "  ";
    std::cout << node.type;
    if (!node.value.empty()) std::cout << " (" << node.value << ")";
    std::cout << "\n";
    for (const auto& child : node.children) {
        printAST(child, indent + 1);
    }
}

#endif
