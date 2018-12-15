//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_MEMN_H
#define C0_MEMN_H

#include "Node.h"

namespace C0 {


class SetN : public Node {
public:
    SetN(Nop op, UseE region, UseE pointer, UseE lhs, UseE rhs):Node(op, 4) {
        uses[0] = region;
        uses[1] = pointer;
        uses[2] = lhs;
        uses[3] = rhs;
    }

    void SCCPType() override;


};

class SetIntN : public SetN {
public:
    SetIntN(UseE region, UseE pointer, UseE lhs, UseE rhs)
        :SetN(Nop::SetInt, region, pointer, lhs, rhs) {}
};

class SetCharN : public SetN {
public:
    SetCharN(UseE region, UseE pointer, UseE lhs, UseE rhs)
        :SetN(Nop::SetChar, region, pointer, lhs, rhs) {}
};


class GetN : public Node {
public:
    GetN(Nop op, UseE region, UseE pointer, UseE value): Node(op, 3) {
        uses[0] = region;
        uses[1] = pointer;
        uses[2] = value;
    }

    void SCCPType() override;
};

class GetIntN: public GetN {
public:
    GetIntN(UseE region, UseE pointer, UseE value)
        :GetN(Nop::GetInt, region, pointer, value) {}
};

class GetCharN: public GetN {
public:
    GetCharN(UseE region, UseE pointer, UseE value)
        :GetN(Nop::GetChar, region, pointer, value) {}
};



}

#endif //C0_MEMN_H
