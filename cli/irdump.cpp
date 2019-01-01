//
// Created by baomingkun on 18-12-31.
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
#include "SON/IRGraph.h"
#include "AST/ASTDrawer.h"

#include "Parser.h"
#include "util.h"

#ifdef _WIN32
#include <cstdio>
#endif


int main(int argc, char **argv) {
    if(argc < 2) {
        return -1;
    }

    std::string source = C0::getFileContents(argv[1]);
    std::cout << "##$$ source.c0" << std::endl;
    auto parser = C0::Parser::fromStr(source);

    auto [funcs, sym] = parser.parseProg();

    std::cout << "##$$ AST.dot" << std::endl;

    std::cout << C0::ASTDrawer::drawProgram(funcs);

    for(const auto &err: parser.getError()) {
        std::cout << err << std::endl;
    }

    auto global_offsets = C0::getGlobalOffset(sym->getVarInScope());

    for (auto &func: funcs) {

        C0::Sea ocean(4096);
        C0::PhiCleaner phi_cleaner(ocean);

        C0::RegionN *start;
        C0::StopN *stop;

        {
            C0::SONBuilder builder(ocean, global_offsets);
            func->accept(builder);
            auto graph = builder.getResult();
            start = graph.first;
            stop = graph.second;
        }



        C0::IRGraph graph(start, stop);

        C0::buildDefUse(stop);

        std::cout << "##$$ " << func->name << "/raw.ir" << std::endl;
        graph.buildPostOrder();
        graph.buildIndex();

        std::cout << graph.irDump(true) << std::endl;

        phi_cleaner.optimize(stop);

        graph.buildPostOrder();
        graph.buildIndex();

        std::cout << "##$$ " << func->name << "/origin.ir" << std::endl;
        std::cout << graph.irDump(true) << std::endl;


        C0::mergeLinearRegion(stop);

        graph.buildPostOrder();
        graph.buildIndex();

        std::cout << "##$$ " << func->name << "/after_merge.ir" << std::endl;
        std::cout << graph.irDump(true) << std::endl;

        graph.buildPostOrder();
        graph.buildIndex();

        C0::SCCPOptimizer sccp;
        sccp.analysis(std::pair(start, stop));



        std::cout << "##$$ " << func->name << "/sccp_label.ir" << std::endl;

        std::cout << graph.irDump(true) << std::endl;

        sccp.transform(std::pair(start, stop), ocean);


        std::cout << "##$$ " << func->name << "/sccp_transform.ir" << std::endl;

        graph.buildPostOrder();
        graph.buildIndex();


        std::cout << graph.irDump(true) << std::endl;


        std::cout << "##$$ " << func->name << "/control_flow.dot" << std::endl;

        C0::FineDrawer fine_drawer;
        fine_drawer.draw(stop);
        std::cout << fine_drawer.toDot(func->name) << std::endl;


        graph.buildPostOrder();
        graph.buildIndex();

        std::cout << "##$$ " << func->name << "/ir_dump.ir" << std::endl;
        std::cout << graph.irDump(true) << std::endl;

    }


#ifdef _WIN32
    std::cout << "press any key to exit" << std::endl;
    getchar();
#endif
    return 0;
}