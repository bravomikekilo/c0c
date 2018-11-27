//
// Created by B on 2018/11/25.
//

#ifndef C0_FRAMEBUILDER_H
#define C0_FRAMEBUILDER_H

#include "BfsCFGWalker.h"
#include "frameTable.h"
#include "AST/FuncAST.h"

namespace C0 {

class frameBuilder : public BfsCFGWalker {
private:
    frameTable *table;
    size_t temp_count = 0;

    void checkVal(QuadVal &val);

protected:
    void visit(BasicBlock *) override;

public:

    void buildFrame(BasicBlock *start, shared_ptr<FuncAST> func) {
        // add function argument and local variable
        auto sym_table = func->table;
        const auto arg_num = func->args.size();
        const auto &all_vars = sym_table->getVarInScope();

        for(const auto &var: all_vars) {
            if(!var.isConst()) {
                table->addVal(QuadVal(var.id), var.type.sizeOf());
            }
        }

        temp_count = 0;
        walk(start);

        table->addTemp(temp_count);
    }

    frameBuilder() {
        table = new frameTable;
    }

    ~frameBuilder() {
        delete table;
    }

    frameTable *getTable() {
        auto ret = table;
        table = nullptr;
        return ret;
    }


};
}


#endif //C0_FRAMEBUILDER_H
