//
// Created by baomingkun on 18-11-18.
//

#include "BasicBlock.h"

namespace C0 {


void C0::BasicBlock::constFolding(const C0::SymTable &table) {

    auto begin = insts.begin();
    optional<QuadVal> c_val;
    auto start = insts.begin();
    int val = 0;

    for(;begin != insts.end(); ++begin) {
        if(auto c = begin->constFold(table, c_val, val); c) {
            // get a const foldable right side
            if(!c_val.has_value()) {
                c_val = begin->dst;
                start = begin;
                val = c.value();
            } else if(c_val == begin->dst) {
                val = c.value();
            } else { // last val
                insts.erase(start, begin);
                *begin = Quad(begin->dst, QuadOp::Copy, QuadVal(c.value(), true), QuadVal());
                insts.insert(begin, Quad(c_val.value(), QuadOp::Copy, QuadVal(val, true), QuadVal()));
                c_val.reset();
                // begin->op = QuadOp::Copy;
            }
        } else if(c_val.has_value()){
            // begin->constReplace(c_val.value(), val);
            insts.erase(start, begin);
            insts.insert(begin, Quad(c_val.value(), QuadOp::Copy, QuadVal(val, true), QuadVal()));
            c_val.reset();
        } else {
            c_val.reset();
        }
    }

    if(c_val.has_value()) {
        insts.insert(begin, Quad(c_val.value(), QuadOp::Copy, QuadVal(val, true), QuadVal()));
    }
}

void C0::BasicBlock::deadClean() {
    auto iter = insts.begin();
    for(;iter != insts.end(); ++iter) {
        if(iter->op == QuadOp::Ret) {
            break;
        }
    }

    if(iter != insts.end()) {
        insts.erase(++iter, insts.end());
    }

}


}
