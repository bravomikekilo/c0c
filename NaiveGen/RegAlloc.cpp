//
// Created by baomingkun on 18-11-26.
//

#include "RegAlloc.h"
#include <unordered_set>
#include "AST/FuncAST.h"

namespace C0 {

void C0::RegAlloc::visit(C0::BasicBlock *block) {
    auto* curr_table = new RegTable;
    std::cout << "begin alloc basic block: " << block->getBid() << std::endl;
    block->setPayload(curr_table);
    tables.push_back(curr_table);
    size_t next_reg = 0;
    auto &insts = block->insts;
    for(auto iter=insts.begin();
        next_reg < saved_num && iter != insts.end(); ++iter) {

        if(iter->dst.val != 0 && !iter->dst.isConst) {
            checkVal(iter->dst, curr_table, &next_reg);
        }

        if(next_reg >= saved_num) break;

        if(iter->src0.val != 0 && !iter->src0.isConst) {
            checkVal(iter->src0, curr_table, &next_reg);
        }

        if(next_reg >= saved_num) break;

        if(iter->src1.val != 0 && !iter->src1.isConst) {
            checkVal(iter->src1, curr_table, &next_reg);
        }

        if(next_reg >= saved_num) break;

        if (iter->call_ext != nullptr) {
            auto &args = iter->call_ext->second;
            for (auto &arg : args) {
                if (arg.val != 0 && !arg.isConst) {
                    checkVal(arg, curr_table, &next_reg);
                }
                if (next_reg >= saved_num) return;
            }
        }

    }


}

bool RegAlloc::checkVal(QuadVal &val, RegTable *table, size_t *reg) {
    if(table->count(val) != 0) return false;

    auto *term = curr_func->table->findVarByID(val.val);
    if(val.isConst || (term != nullptr && (term->isGlobal || term->isConst()))) {
        return false;
    } else {
        std::cout << "alloc: " << varToString(val, curr_func->table)
                  << "->" << *reg << std::endl;
        table->insert(pair(val, make_unique<SReg>(*reg)));
        ++*reg;
    }
    return false;
}

} // end namespace C0
