//
// Created by baomingkun on 18-11-18.
//

#ifndef C0_BASICBLOCK_H
#define C0_BASICBLOCK_H

#include "common.h"
#include "Quads.h"
#include "Pos.h"
#include <list>

namespace C0 {


class BasicBlock {
public:
    Pos pos;
    std::list<Quad> insts;
    BasicBlock *next;

    explicit BasicBlock(Pos pos, int bid, BasicBlock *next = nullptr)
    :bid(bid), next(next), pos(pos) {}

    int getBid() { return bid; }

    void setPayload(void *payload) { other = payload; }
    void cleanPayload() { other = nullptr; }

    template <typename T>
    T *payload() { return (T *)other; }

    void constFolding(const C0::SymTable &table);
    void deadClean();

private:
    int bid;
    void *other = nullptr;
};



class BasicBlockBuilder {

public:
    BasicBlock *create(Pos pos) {
        auto ret = new BasicBlock(pos, popBid());
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
