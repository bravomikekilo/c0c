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
    std::list<Quad> insts;
    BasicBlock *next;

    explicit BasicBlock(int bid, BasicBlock *next = nullptr)
    :bid(bid), next(next) {}

    int getBid() { return bid; }
private:
    int bid;
};



class BasicBlockBuilder {

public:
    BasicBlock *create() {
        auto ret = new BasicBlock(popBid());
        built.push_back(ret);
        return ret;
    }

    ~BasicBlockBuilder() {
        for(auto b: built) {
            delete b;
        }
    }


private:
    int popBid() {
        return next_bid++;
    }
    int next_bid = 1;
    vector<BasicBlock *> built;
    
};



}
#endif //C0_BASICBLOCK_H
