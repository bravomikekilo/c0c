//
// Created by baomingkun on 18-11-23.
//

#ifndef C0_DEFTABLE_H
#define C0_DEFTABLE_H

#include "common.h"
#include "SymTable.h"
#include "SSA.h"

namespace C0 {


class BuildContext {
    unordered_map<VarID, ValueE> table;
    bool filled = false;
    bool sealed = false;

public:

    bool hasVar(VarID id) { return table.count(id) == 1; }

    ValueE getVar(VarID id) {
        if(table.count(id) == 0) {
            return nullptr;
        } else {
            return table[id];
        }
    }

    void setVar(VarID id, ValueE val) {
        table[id] = val;
    }

};


}


#endif //C0_DEFTABLE_H
