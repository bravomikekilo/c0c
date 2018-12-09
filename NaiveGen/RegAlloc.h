//
// Created by baomingkun on 18-11-26.
//

#ifndef C0_REGALLOC_H
#define C0_REGALLOC_H

#include "BfsCFGWalker.h"
#include "Asm/Reg.h"


namespace C0 {

using RegTable = unordered_map<QuadVal, unique_ptr<Reg>>;

class RegAlloc : public BfsCFGWalker {
private:
    const bool verbose;
    shared_ptr<FuncAST> curr_func;
    vector<RegTable *> tables;
    const size_t saved_num; // num of available saved register

    bool checkVal(QuadVal &val, RegTable *table, size_t *reg);

protected:
    void visit(BasicBlock *) override;

public:

    explicit RegAlloc(size_t saved_num, bool verbose=false)
        : saved_num(saved_num), verbose(verbose) {}

    vector<RegTable *> alloc(BasicBlock *start, shared_ptr<FuncAST> func) {
        curr_func = func;
        walk(start);
        vector<RegTable *> ret;
        std::swap(ret, tables);
        return ret;
    }



};

}

#endif //C0_REGALLOC_H
