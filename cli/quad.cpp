//
// Created by baomingkun on 18-11-19.
//

#include "Quad/CFGConverter.h"
#include "Quad/CFGDrawer.h"
#include "AST/ASTDrawer.h"
#include "Parser.h"
#include "util.h"


int main(int argc, char **argv) {
    if(argc != 2) {
        return -1;
    }

    std::string source = C0::getFileContents(argv[1]);
    std::cout << source << std::endl;
    auto parser = C0::Parser::fromStr(source);

    auto [funcs, sym] = parser.parseProg();

    std::cout << C0::ASTDrawer::drawProgram(funcs);

    for(const auto& err: parser.getError()) {
        std::cout << err << std::endl;
    }

    for(auto &func: funcs) {


        C0::BasicBlockBuilder builder;

        C0::CFGConverter converter(builder);

        func->accept(converter);

        auto start_block = converter.getResult();

        C0::CFGDrawer cfg_drawer;

        cfg_drawer.draw(start_block, func->table);

        std::cout << cfg_drawer.getDot(func->name) << std::endl;

    }


    return 0;

}