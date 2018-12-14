//
// Created by baomingkun on 18-12-13.
//

#ifndef C0_PROJN_H
#define C0_PROJN_H

#include "common.h"
#include "Node.h"
#include "SymTable.h"

namespace C0 {

class ProjN : public Node {
public:
    const int field;
    ProjN(int n, Nop op, size_t num_uses):Node(op, num_uses), field(n) {}
};

/// project world from a Read node
class ProjWorldN : public Node {
public:
    ProjWorldN(UseE region, UseE up): Node(Nop::ProjWorld, 2) {
        uses[0] = region;
        uses[1] = up;
    }
};

/// project return value from Read node
class ProjRetN : public Node {

public:
    ProjRetN(UseE region, UseE up): Node(Nop::ProjRet, 2) {
        uses[0] = region;
        uses[1] = up;
    }

    void SCCPType() override;

};

/// projection global value from call node
class ProjGlobalN: public Node {
private:
    VarID id;
public:
    ProjGlobalN(UseE region, UseE up, VarID id)
        : Node(Nop::ProjGlobal, 2), id(id) {
        uses[0] = region;
        uses[1] = up;
    }

};

}

#endif //C0_PROJN_H
