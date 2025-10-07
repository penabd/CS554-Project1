#ifndef SCAN_HPP
#define SCAN_HPP

#include "tokens.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>  
#include <cctype>     

inline bool isInteger(const std::string& str) {
    if (str.empty()) {
        return false;
    } else {
        return std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return std::isdigit(c);
        });
    }
}

inline int scan(const std::string &path, std::vector<Token> &tokens) {
    std::ifstream file(path);

    if (!file) {
        std::cerr << "Could not open file.\n";
        return 1;
    }

    std::string line;
    bool isComment = false;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char ch;
        std::string word;

        while (iss.get(ch)) {
            if (std::isspace(static_cast<unsigned char>(ch)) || ch == ';') {
                if (!word.empty()) {
                    if (isComment && word != "-}") {
                        tokens.push_back(T_COM{word});
                    } else if(word == "-}") {
                        tokens.push_back(T_RLCOM{});
                        isComment = false;
                    }else {
                        if (std::isalpha(static_cast<unsigned char>(word[0]))) {
                            if (std::find(KEYWORDS.begin(), KEYWORDS.end(), word) != KEYWORDS.end()) {
                                tokens.push_back(KW_TO_TOKENS.at(word));
                            } else {
                                tokens.push_back(T_VAR{word});
                            }
                        } else if (isInteger(word)) {
                            tokens.push_back(T_INT{std::stoi(word)});
                        } else if (BINARY_OPS.count(word)) {
                            tokens.push_back(BINARY_OPS.at(word));
                        } else if (word == ")") {
                            tokens.push_back(T_RPAR{});
                        } else if (word == "(") {
                            tokens.push_back(T_LPAR{});
                        } else if (word == "[") {
                            tokens.push_back(T_LBRAC{});
                        } else if (word == "]") {
                            tokens.push_back(T_RBRAC{});
                        } else if (word == ":=") {
                            tokens.push_back(T_ASMT{});
                        } else if (word == "--") {
                            tokens.push_back(T_SCOM{});

                            std::string rest;
                            std::getline(iss, rest);

                            if (!rest.empty()) {
                                tokens.push_back(T_COM{rest});
                            }

                            break;
                        } else if (word == "{-") {
                            tokens.push_back(T_LLCOM{});
                            isComment = true;
                         } else {
                            std::cout << "Unrecognized token: " << word << "\n";
                            // std::cerr << "Unrecognized token: " << word << "\n";
                            // return 1;
                        }
                    }

                    word.clear();
                }

                if (ch == ';') {
                    tokens.push_back(T_SCOL{});
                }
            } 
            else {
                word += ch;
                if (isComment && word.size() >= 2 && word.substr(word.size() - 2) == "-}") {
                    tokens.push_back(T_RLCOM{});
                    isComment = false;
                    word.clear();
                    continue;
                }

            }
        }

        if (!word.empty()) {
            if (isComment) {
                tokens.push_back(T_COM{word});
            }else if(word == "-}") {
                tokens.push_back(T_RLCOM{});
                isComment = false;
            } else {
                if (std::isalpha(static_cast<unsigned char>(word[0]))) {
                    if (std::find(KEYWORDS.begin(), KEYWORDS.end(), word) != KEYWORDS.end()) {
                        tokens.push_back(KW_TO_TOKENS.at(word));
                    } else {
                        tokens.push_back(T_VAR{word});
                    }
                } else if (isInteger(word)) {
                    tokens.push_back(T_INT{std::stoi(word)});
                } else if (BINARY_OPS.count(word)) {
                    tokens.push_back(BINARY_OPS.at(word));
                } else if (word == ")") {
                    tokens.push_back(T_RPAR{});
                } else if (word == "(") {
                    tokens.push_back(T_LPAR{});
                } else if (word == "[") {
                    tokens.push_back(T_LBRAC{});
                } else if (word == "]") {
                    tokens.push_back(T_RBRAC{});
                } else if (word == ":=") {
                    tokens.push_back(T_ASMT{});
                } else if (word == "--") {
                    tokens.push_back(T_SCOM{});
                } else if (word == "{-") {
                    tokens.push_back(T_LLCOM{});
                    isComment = true;
                }else {
                    std::cout << "Unrecognized token: " << word << "\n";
                    // std::cerr << "Unrecognized token: " << word << "\n";
                    // return 1;
                }
            }

            word.clear();
        }
    }

    return 0;
}

#endif
