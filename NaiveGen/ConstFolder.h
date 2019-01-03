//
// Created by baomingkun on 19-1-3.
//

#ifndef C0_CONSTFOLDER_H
#define C0_CONSTFOLDER_H

#include "common.h"
#include "BfsCFGWalker.h"

namespace C0 {


class ConstFolder : public BfsCFGWalker {
private:
    const shared_ptr<SymTable> table;
protected:
    void visit(BasicBlock *block) override;

public:
    explicit ConstFolder(shared_ptr<SymTable> table)
        :table(std::move(table)) {}

    void foldConst(BasicBlock *start) {
        walk(start);
    }
};



}


#endif //C0_CONSTFOLDER_H
