//
// Created by baomingkun on 18-11-25.
//

#include <cstring>
#include <iostream>
#include "Parser.h"
#include "AST/ASTDrawer.h"
#include "util.h"
#include "Asm/InstList.h"
#include "Quad/CFGConverter.h"
#include "Quad/CFGDrawer.h"

#include "NaiveGen/frameBuilder.h"
#include "NaiveGen/CFGSerializer.h"
#include "NaiveGen/RegAlloc.h"

#include <unordered_map>
#include <memory>
#include <vector>

#ifdef _WIN32
#include <cstdio>
#endif

using std::string;
using std::unordered_map;
using std::pair;
using std::shared_ptr;
using std::vector;

int main(int argc, char **argv) {

    if (argc < 2) return -1;
    bool only_asm = false;
    bool escape_str = true;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--asm") == 0) {
            only_asm = true;
        }

        if (strcmp(argv[i], "--raw") == 0) {
            escape_str = false;
        }
    }

    std::string source = C0::getFileContents(argv[1]);

    auto parser = C0::Parser::fromStr(source);

    auto[funcs, sym] = parser.parseProg();

    if(!only_asm) {
        std::cout << C0::ASTDrawer::drawProgram(funcs) << std::endl;
    }

    unordered_map<string, C0::BasicBlock *> cfg_map;
    unordered_map<string, std::shared_ptr<C0::frameTable>> frames;
    unordered_map<string, int> offsets;
    vector<C0::RegTable *> regs;
    C0::BasicBlockBuilder builder;

    for (auto &func: funcs) {
        C0::CFGConverter converter(builder);

        func->accept(converter);

        auto start_block = converter.getResult();
        cfg_map.insert(pair(func->name, start_block));

        if (!only_asm) {
            C0::CFGDrawer cfg_drawer;
            cfg_drawer.draw(start_block, func->table);
            std::cout << cfg_drawer.getDot(func->name) << std::endl;
        }


        C0::frameBuilder frame_builder;
        frame_builder.buildFrame(start_block, func);

        auto frame = shared_ptr<C0::frameTable>(frame_builder.getTable());

        frames.insert(pair(func->name, frame));
        offsets.insert(pair(func->name, frame->getWholeSize()));

        if (!only_asm) {
            std::cout << "frame size of function: " << func->name << " is " << frame->getWholeSize() << std::endl;
        }

        C0::RegAlloc alloc(6);
        auto reg_tables = alloc.alloc(start_block, func);
        regs.insert(regs.end(), reg_tables.begin(), reg_tables.end());
    }

    if (!only_asm) {
        std::cout << std::endl;
        std::cout << "begin generate code" << std::endl;
    }

    C0::InstList list(escape_str);
    list.addGlobal(sym->getGlobalList());
    list.addString(sym->getStringList());

    for (auto &func: funcs) {
        C0::CFGSerializer writer(list, offsets, !only_asm);
        writer.serialize(func, cfg_map[func->name], frames[func->name]);
    }

    std::cout << list.toString() << std::endl;

#ifdef _WIN32
    std::cout << "press any key to exit" << std::endl;
    getchar();
#endif

}
