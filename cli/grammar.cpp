#include <string>
#include <memory>
#include <cstring>
#include <iostream>
#include "AST/ASTDrawer.h"
#include "TypeChecker.h"
#include "Parser.h"
#include "util.h"
#include "AST/ExprAST.h"
#ifdef _WIN32
#include <cstdio>
#endif


using namespace std;


int main(int argc, char **argv) {

    string source_name;

    if(argc == 2) {
        source_name = std::string(argv[1]);
    } else {
        cout << "please input path to source file:";
        cin >> source_name;
    }


    string source = C0::getFileContents(source_name.c_str());
    std::cout << "source:" << std::endl << source << std::endl;

    auto parser = C0::Parser::fromStr(source);
    auto [funcs, sym] = parser.parseProg();
    
    std::cout << std::endl;
    std::cout << "----------graphviz dot for AST-------------" << std::endl;
    std::cout << C0::ASTDrawer::drawProgram(funcs);

    // std::cout << C0::ASTDrawer::dot(*func) << std::endl;
    std::cout << "----------other output------------" << std::endl;
    if (!parser.getError().empty()) {
        std::cout << "syntax error" << std::endl;
    }
    for(const auto& err: parser.getError()) {
        std::cout << err << std::endl;
    }
    
    
    for (auto &func : funcs) {
        C0::TypeChecker checker;
        func->accept(checker);
        if (!checker.getErrors().empty()) {
            std::cout << "sementic error in function " << func->name << std::endl;
            for (const auto &err : checker.getErrors()) {
                std::cout << err << std::endl;
            }
            std::cout << endl;
        }
    }

#ifdef _WIN32
    std::cout << "press any key to exit" << std::endl;
    getchar();
    getchar();
#endif


    return 0;

}


