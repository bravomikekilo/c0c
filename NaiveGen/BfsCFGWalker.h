#pragma once

#include "common.h"
#include "Quad/BasicBlock.h"
#include <unordered_set>
#include <queue>


namespace C0 {

class BfsCFGWalker {
protected:
    std::unordered_set<BasicBlock *>  visited;
    virtual void visit(BasicBlock *block) = 0;
    std::queue<BasicBlock *> q;
    BasicBlock *nextBlock();

public:

    void walk(BasicBlock *start);

};










} // end namespace C0
