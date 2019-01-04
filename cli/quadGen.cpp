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
#include "Quad/CFGWriter.h"
#include "NaiveGen/ConstFolder.h"

#include "NaiveGen/frameBuilder.h"
#include "NaiveGen/CFGSerializer.h"
#include "NaiveGen/RegAlloc.h"
#include "TypeChecker.h"

#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

#ifdef _WIN32
#include <cstdio>
#endif

using std::string;
using std::unordered_map;
using std::pair;
using std::shared_ptr;
using std::vector;


int main(int argc, char **argv) {

    string source_name;
    if (argc < 2) {
        std::cin >> source_name;
    } else {
        source_name = argv[1];
    }
    bool verbose = false;
    bool escape_str = true;
    bool with_opt = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        }

        if (strcmp(argv[i], "--raw") == 0) {
            escape_str = false;
        }

        if (strcmp(argv[i], "--opt") == 0) {
            with_opt = true;
        }
    }

    bool has_error = false;

    std::string source = C0::getFileContents(source_name.c_str());

    auto parser = C0::Parser::fromStr(source);

    auto[funcs, sym] = parser.parseProg();

    if (verbose) {
        std::cout << C0::ASTDrawer::drawProgram(funcs) << std::endl;
    }

    if (!parser.getError().empty()) {
        has_error = true;

        std::cout << "all syntax error" << std::endl;

        for (const auto &err: parser.getError()) {
            std::cout << err << std::endl;
        }

        std::cout << std::endl;
    }

    for (auto &func : funcs) {
        C0::TypeChecker checker;
        func->accept(checker);
        if (!checker.getErrors().empty()) {
            std::cout << "sementic error in function " << func->name << std::endl;
            for (const auto &err: checker.getErrors()) {
                has_error = true;
                std::cout << err << std::endl;
            }
            std::cout << std::endl;
        }
    }

    if (has_error) {
#ifdef _WIN32
        getchar();
        getchar();
#endif
        return -1;
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

        if (verbose) {
            C0::CFGDrawer cfg_drawer;
            cfg_drawer.draw(start_block, func->table);
            std::cout << cfg_drawer.getDot(func->name) << std::endl;
        }

        if (!with_opt) {
            std::cout << func->name << "/pre.cfg" << std::endl;
            std::cout << C0::writeCFG(start_block, func->table) << std::endl;
        }

        if (with_opt) {
            C0::ConstFolder folder(func->table);
            folder.foldConst(start_block);

            std::cout << func->name << "/after.cfg" << std::endl;
            std::cout << C0::writeCFG(start_block, func->table) << std::endl;
        }

        C0::frameBuilder frame_builder;
        frame_builder.buildFrame(start_block, func);

        auto frame = shared_ptr<C0::frameTable>(frame_builder.getTable());

        frames.insert(pair(func->name, frame));
        offsets.insert(pair(func->name, frame->getWholeSize()));

        if (verbose) {
            std::cout << "frame size of function: " << func->name << " is " << frame->getWholeSize() << std::endl;
        }

        C0::RegAlloc alloc(8);
        auto reg_tables = alloc.alloc(start_block, func);
        regs.insert(regs.end(), reg_tables.begin(), reg_tables.end());
    }

    if (verbose) {
        std::cout << std::endl;
        std::cout << "begin generate code" << std::endl;
    }

    C0::InstList list(escape_str);
    list.addGlobal(sym->getGlobalList());
    list.addString(sym->getStringList());

    for (auto &func: funcs) {
        C0::CFGSerializer writer(list, offsets, verbose);
        writer.serialize(func, cfg_map[func->name], frames[func->name]);
    }

    std::cout << list.toString() << std::endl;

#ifdef _WIN32
    getchar();
    getchar();
#endif

}
