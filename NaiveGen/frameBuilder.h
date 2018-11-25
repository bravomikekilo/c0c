//
// Created by B on 2018/11/25.
//

#ifndef C0_FRAMEBUILDER_H
#define C0_FRAMEBUILDER_H

#include "BfsCFGWalker.h"

namespace C0 {

class frameBuilder : public BfsCFGWalker {
protected:
    void visit(BasicBlock *) override;







};
}


#endif //C0_FRAMEBUILDER_H
