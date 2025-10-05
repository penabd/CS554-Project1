#ifndef SCAN_HPP
#define SCAN_HPP

#include "tokens.hpp"
#include <vector>

Token determineType(std::string &strToken){
    switch (strToken){
        case "while":
        case "do":
        case "od":
        case "if":
        case "then":
        case "else":
        case "fi":
        case "skip":
           return T_KW(strToken);
        case 

    }
}

int scan(const std::string &path, std::vector<Token> &tokens){
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Could not open file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {  
        std::istringstream iss(line);    
        std::string word;
        while (iss >> word) {            
            
        }
    }
  
    return 0;
}
#endif