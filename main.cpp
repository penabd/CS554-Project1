#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include "scan.hpp"
#include "parse.hpp"

int main(int argc, char* argv[]) {
    std::string programPath = argv[1];
    std::vector<Token> program;

    if(scan(programPath, program)){
        std::cerr << "Problem reading program." << std::endl;
        return 1;
    }

    for(auto token: program){
        std::visit([](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, T_KW>)
                std::cout << "T_KW(" << arg.value << ")\n";
            else if constexpr (std::is_same_v<T, T_VAR>)
                std::cout << "T_VAR(" << arg.value << ")\n";
            else if constexpr (std::is_same_v<T, T_INT>)
                std::cout << "T_INT(" << arg.value << ")\n";
            else if constexpr (std::is_same_v<T, T_BOOL>)
                std::cout << "T_BOOL(" << (arg.value ? "true" : "false") << ")\n";
            else if constexpr (std::is_same_v<T, T_BOP>)
                std::cout << "T_BOP(" << arg.value << ")\n";
            else if constexpr (std::is_same_v<T, T_UOP>)
                std::cout << "T_UOP(" << arg.value << ")\n";
            else if constexpr (std::is_same_v<T, T_LPAR>)
                std::cout << "T_LPAR\n";
            else if constexpr (std::is_same_v<T, T_RPAR>)
                std::cout << "T_RPAR\n";
            else if constexpr (std::is_same_v<T, T_LBRAC>)
                std::cout << "T_LBRAC\n";
            else if constexpr (std::is_same_v<T, T_RBRAC>)
                std::cout << "T_RBRAC\n";
            else if constexpr (std::is_same_v<T, T_ASMT>)
                std::cout << "T_ASMT\n";
            else if constexpr (std::is_same_v<T, T_SCOL>)
                std::cout << "T_SCOL\n";
            else if constexpr (std::is_same_v<T, T_RLCOM>)
                std::cout << "T_RLCOM\n";
            else if constexpr (std::is_same_v<T, T_LLCOM>)
                std::cout << "T_LLCOM\n";
            else if constexpr (std::is_same_v<T, T_COM>)
                std::cout << "T_COM(" << arg.value << ")\n";
            else if constexpr (std::is_same_v<T, T_SCOM>)
                std::cout << "T_SCOM\n";
        }, token);
    }

    AST ast = parse(program);
    printAST(ast);
    return 0;
}