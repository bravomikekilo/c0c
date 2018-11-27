//
// Created by baomingkun on 18-11-26.
//

#include "RegAlloc.h"
#include <unordered_set>
#include "AST/FuncAST.h"

namespace C0 {

void C0::RegAlloc::visit(C0::BasicBlock *block) {
    auto* curr_table = new RegTable;
    block->setPayload(curr_table);
    tables.push_back(curr_table);
    std::unordered_set<QuadVal> alloced_val;
    size_t next_reg = 0;
    auto &insts = block->insts;
    for(auto iter=insts.begin();
        next_reg < saved_num && iter != insts.end(); ++iter) {

        if(iter->dst.val != 0
            && alloced_val.count(iter->dst) == 0
            && checkVal(iter->dst, curr_table, &next_reg)) {
            alloced_val.insert(iter->dst);
        }

        if(next_reg >= saved_num) break;

        if(iter->src0.val != 0
            && alloced_val.count(iter->src0) == 0
            && checkVal(iter->src0, curr_table, &next_reg)) {
            alloced_val.insert(iter->src0);
        }

        if(next_reg >= saved_num) break;

        if(iter->src1.val != 0
            && alloced_val.count(iter->src1) == 0
            && checkVal(iter->src1, curr_table, &next_reg)) {
            alloced_val.insert(iter->src1);
        }

        if (iter->call_ext != nullptr) {
            auto &args = iter->call_ext->second;
            for (auto &arg : args) {
                if (arg.val != 0
                    && alloced_val.count(arg) == 0
                    && checkVal(arg, curr_table, &next_reg)) {
                    alloced_val.insert(arg);
                }
                if (next_reg >= saved_num) return;
            }
        }

    }


}

bool RegAlloc::checkVal(QuadVal &val, RegTable *table, size_t *reg) {
    auto *term = curr_func->table->findVarByID(val.val);
    if(term != nullptr && (term->isGlobal || val.isConst || term->isConst())) {
        return false;
    } else {
        table->insert(pair(val, make_unique<SReg>(*reg)));
        ++*reg;
    }
    return false;
}

} // end namespace C0
