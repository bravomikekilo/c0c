//
// Created by baomingkun on 18-12-4.
//

#include <iostream>
#include "SON/sonUtil.h"
#include "SON/SON.h"
#include "SON/Sea.h"
#include "SON/SONBuilder.h"
#include "SON/SONDrawer.h"
#include "SON/SCCP.h"
#include "SON/PhiClean.h"
#include "SON/FineDrawer.h"
#include "SON/dominance.h"
#include "AST/ASTDrawer.h"

#include "Parser.h"
#include "util.h"

#ifdef _WIN32
#include <cstdio>
#endif


int main(int argc, char **argv) {
    if (argc < 2) {
        return -1;
    }

    std::string source = C0::getFileContents(argv[1]);
    std::cout << "##$$ source.c0" << std::endl;
    std::cout << source << std::endl;
    auto parser = C0::Parser::fromStr(source);

    auto[funcs, sym] = parser.parseProg();

    std::cout << "##$$ AST.dot" << std::endl;

    std::cout << C0::ASTDrawer::drawProgram(funcs);

    for (const auto &err: parser.getError()) {
        std::cout << err << std::endl;
    }


    auto global_offsets = C0::getGlobalOffset(sym->getVarInScope());

    C0::Sea ocean(4096);
    C0::PhiCleaner phi_cleaner(ocean);
    for (auto &func: funcs) {
        C0::RegionN *start;
        C0::StopN *stop;

        {
            C0::SONBuilder builder(ocean, global_offsets);
            func->accept(builder);
            auto graph = builder.getResult();
            start = graph.first;
            stop = graph.second;
        }

        C0::buildDefUse(stop);

        phi_cleaner.optimize(stop);
        C0::SONDrawer drawer;

        std::cout << "##$$ " << func->name << "/origin.dot" << std::endl;
        drawer.draw(stop);
        std::cout << drawer.toDot(func->name) << std::endl;


        C0::mergeLinearRegion(stop);


        drawer.clear();
        drawer.draw(stop);


        std::cout << "##$$ " << func->name << "/after_merge.dot" << std::endl;
        std::cout << drawer.toDot(func->name) << std::endl;

        C0::SCCPOptimizer sccp;
        sccp.analysis(std::pair(start, stop));

        drawer.clear();
        drawer.draw(stop);


        std::cout << "##$$ " << func->name << "/sccp_label.dot" << std::endl;

        std::cout << drawer.toDot(func->name) << std::endl;

        sccp.transform(std::pair(start, stop), ocean);


        std::cout << "##$$ " << func->name << "/sccp_transform.dot" << std::endl;

        drawer.clear();
        drawer.draw(stop, true);

        std::cout << drawer.toDot(func->name) << std::endl;


        std::cout << "##$$ " << func->name << "/control_flow.dot" << std::endl;


        auto tree = C0::computeDominanceTree(std::pair(start, stop));

        C0::FineDrawer fine_drawer;
        fine_drawer.draw(stop);
        std::cout << fine_drawer.toDot(func->name) << std::endl;






    }


#ifdef _WIN32
    std::cout << "press any key to exit" << std::endl;
    getchar();
#endif
    return 0;
}


