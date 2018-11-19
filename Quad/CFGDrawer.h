//
// Created by baomingkun on 18-11-18.
//

#ifndef C0_CFGDRAWER_H
#define C0_CFGDRAWER_H

#include "common.h"
#include "Quad/BasicBlock.h"
#include "Quad/Quads.h"
#include "SymTable.h"
#include <list>
#include <unordered_set>
#include <tuple>

namespace C0 {

class CFGDrawer {

    shared_ptr<SymTable> sym_table;

    std::unordered_set<BasicBlock *> visited;
    unordered_map<int, std::list<string>> cfg;
    std::list<std::tuple<int, int, string>> edges;

public:
    void draw(BasicBlock *start, shared_ptr<SymTable> sym_table);

    string getDot() const ;

};

}

#endif //C0_CFGDRAWER_H
