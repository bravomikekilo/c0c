//
// Created by baomingkun on 18-11-18.
//

#ifndef C0_BASICBLOCK_H
#define C0_BASICBLOCK_H

#include "common.h"
#include "Quads.h"
#include <list>

namespace C0 {


class BasicBlock {
public:
    std::list<unique_ptr<Quad>> insts;
    vector<BasicBlock *> succ;
    BasicBlock *next;

    int getBid() { return bid; }
private:
    int bid;
};



class BasicBlockBuilder {

    BasicBlock *create() {
        auto ret = new BasicBlock();
        built.push_back(ret);
        return ret;
    }

    ~BasicBlockBuilder() {
        for(auto b: built) {
            delete b;
        }
    }


private:
    vector<BasicBlock *> built;
};



}
#endif //C0_BASICBLOCK_H
