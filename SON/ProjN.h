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
    UseE up;
    ProjN(int n, UseE up, Nop op, size_t num_uses):Node(op, num_uses), field(n), up(up) {}

    void SCCPType() override {
        up->SCCPType(this);
    }


};

/// project world from a Read node
class ProjWorldN : public ProjN {
public:
    ProjWorldN(UseE region, UseE up): ProjN(-1, up, Nop::ProjWorld, 2) {
        uses[0] = region;
        uses[1] = up;
    }
};

/// project return value from Read node
class ProjRetN : public ProjN {

public:
    ProjRetN(UseE region, UseE up): ProjN(-1, up, Nop::ProjRet, 2) {
        uses[0] = region;
        uses[1] = up;
    }

    void SCCPType() override;

};

/// projection global value from call node
class ProjGlobalN: public ProjN {
private:
    VarID id;
public:
    ProjGlobalN(UseE region, UseE up, VarID id)
        : ProjN(id, up, Nop::ProjGlobal, 2), id(id) {
        uses[0] = region;
        uses[1] = up;
    }

};

class IfProjN : public ProjN {

public:
    explicit IfProjN(UseE up, bool branch) :ProjN(branch ? 1 : 0, up, Nop::IfProj, 1) {
        uses[0] = up;
    }

    string str() override {
        return Node::str() + ":" + (field ? "true" : "false");
    }
};

}

#endif //C0_PROJN_H
