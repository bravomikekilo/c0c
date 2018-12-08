//
// Created by baomingkun on 18-12-6.
//

#ifndef C0_UTIL_H
#define C0_UTIL_H

#include "SON.h"
#include "SymTable.h"

namespace C0 {

void buildDefUse(StopN *stop);

void cleanDefUse(StopN *stop);

void mergeLinearRegion(StopN *stop);

unordered_map<VarID, int> getGlobalOffset(const vector<SymTerm> &terms);


}

#endif //C0_UTIL_H
