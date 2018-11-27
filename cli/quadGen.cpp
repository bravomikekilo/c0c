//
// Created by baomingkun on 18-11-25.
//

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

using std::string;
using std::unordered_map;
using std::pair;
using std::shared_ptr;
using std::vector;

int main(int argc, char **argv) {
    std::cout << "hello world from quad gen" << std::endl;

    if(argc != 2) return -1;

    std::string source = C0::getFileContents(argv[1]);
    std::cout << source << std::endl;

    auto parser = C0::Parser::fromStr(source);

    auto [funcs, sym] = parser.parseProg();

    unordered_map<string, C0::BasicBlock *> cfg_map;
    unordered_map<string, std::shared_ptr<C0::frameTable>> frames;
    unordered_map<string, int> offsets;
    vector<C0::RegTable *> regs;
    C0::BasicBlockBuilder builder;
    for(auto &func: funcs) {
        C0::CFGConverter converter(builder);

        func->accept(converter);

        auto start_block = converter.getResult();
        cfg_map.insert(pair(func->name, start_block));

        C0::CFGDrawer cfg_drawer;

        cfg_drawer.draw(start_block, func->table);

        std::cout << cfg_drawer.getDot(func->name) << std::endl;

        C0::frameBuilder frame_builder;
        frame_builder.buildFrame(start_block, func);

        auto frame = shared_ptr<C0::frameTable>(frame_builder.getTable());

        frames.insert(pair(func->name, frame));
        offsets.insert(pair(func->name, frame->getWholeSize()));

        C0::RegAlloc alloc(6);
        auto reg_tables = alloc.alloc(start_block, func);
        regs.insert(regs.end(), reg_tables.begin(), reg_tables.end());
    }

    std::cout << std::endl;
    std::cout << "begin generate code" << std::endl;
    C0::InstList list;

    for(auto &func: funcs) {
        C0::CFGSerializer writer(list, offsets);
        writer.serialize(func, cfg_map[func->name], frames[func->name]);
    }

    std::cout << list.toString() << std::endl;


}
