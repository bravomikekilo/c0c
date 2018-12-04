//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_ARITHN_H
#define C0_ARITHN_H

#include "Node.h"

namespace C0 {

class ArithN : public Node {

public:
    ArithN(Nop op, UseE region, UseE lhs, UseE rhs) :Node(op, 3) {
        uses[0] = region;
        uses[1] = lhs;
        uses[2] = rhs;
    }
};


class AddN: public ArithN {
public:
    AddN(UseE region, UseE lhs, UseE rhs): ArithN(Nop::Add, region, lhs, rhs) {}
};

class SubN: public ArithN {
public:
    SubN(UseE region, UseE lhs, UseE rhs): ArithN(Nop::Sub, region, lhs, rhs) {}
};

class MulN: public ArithN {
public:
    MulN(UseE region, UseE lhs, UseE rhs): ArithN(Nop::Mul, region, lhs, rhs) {}
};

class DivN: public ArithN {
public:
    DivN(UseE region, UseE lhs, UseE rhs): ArithN(Nop::Div, region, lhs, rhs) {}
};




}


#endif //C0_ARITHN_H
