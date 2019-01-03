//
// Created by baomingkun on 19-1-3.
//

#ifndef C0_CFGWRITER_H
#define C0_CFGWRITER_H

#include "common.h"
#include "BasicBlock.h"
#include <stack>
#include <sstream>

namespace C0 {


string writeCFG(BasicBlock *start, const shared_ptr<SymTable> &table);


}


#endif //C0_CFGWRITER_H
