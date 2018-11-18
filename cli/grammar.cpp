#include <string>
#include <memory>
#include <cstring>
#include <iostream>
#include "AST/ASTDrawer.h"
#include "Parser.h"
#include "util.h"
#include "AST/ExprAST.h"

using namespace std;

int main(int argc, char **argv) {
    cout << "please input path to source file:";
    string source_name;
    cin >> source_name;
    
    string source = C0::getFileContents(source_name.c_str());

    auto parser = C0::Parser::fromStr(source);
    auto [funcs, sym] = parser.parseProg();

    std::cout << C0::ASTDrawer::drawProgram(funcs);

    // std::cout << C0::ASTDrawer::dot(*func) << std::endl;

    for(const auto& err: parser.getError()) {
        std::cout << err << std::endl;
    }

    return 0;

}


