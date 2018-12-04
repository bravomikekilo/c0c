//
// Created by baomingkun on 18-12-4.
//

#include <iostream>
#include "SON/SON.h"
#include "SON/Sea.h"
#include "SON/SONBuilder.h"
#include "SON/SONDrawer.h"
#include "AST/ASTDrawer.h"
#include "Parser.h"
#include "util.h"

#ifdef _WIN32
#include <cstdio>
#endif


int main(int argc, char **argv) {
    std::cout << "hello world" << std::endl;
    if (argc != 2) {
        return -1;
    }

    std::string source = C0::getFileContents(argv[1]);
    std::cout << source << std::endl;
    auto parser = C0::Parser::fromStr(source);

    auto[funcs, sym] = parser.parseProg();

    std::cout << C0::ASTDrawer::drawProgram(funcs);

    for(const auto& err: parser.getError()) {
        std::cout << err << std::endl;
    }

    C0::Sea ocean(4096);
    for(auto &func: funcs) {
        C0::SONBuilder builder(ocean);
        func->accept(builder);

        auto [start, stop] = builder.getResult();

        C0::SONDrawer drawer;
        drawer.draw(stop);

        std::cout << drawer.toDot(func->name) << std::endl;


    }

#ifdef _WIN32
    std::cout << "press any key to exit" << std::endl;
    getchar();
#endif
    return 0;
}


