//
// Created by B on 2018/11/25.
//

#include "frameBuilder.h"
#include <utility>

void C0::frameBuilder::visit(C0::BasicBlock *block) {
    for(auto &inst : block->insts) {
        checkVal(inst.src1);
        checkVal(inst.src0);
        checkVal(inst.dst);
    }
}

void C0::frameBuilder::checkVal(C0::QuadVal &val) {
    if(val.val < 0) {
        temp_count = std::max(static_cast<size_t>(-val.val), temp_count);
    }
}
