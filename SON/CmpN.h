//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_CMPN_H
#define C0_CMPN_H

#include "Node.h"

namespace C0 {

class CmpN : public Node {

public:
    CmpN(Nop op, UseE region, UseE lhs, UseE rhs) : Node(op, 3) {
        uses[0] = region;
        uses[1] = lhs;
        uses[2] = rhs;
    }
};


class EqN : public CmpN {
public:
    EqN(UseE region, UseE lhs, UseE rhs) : CmpN(Nop::Eq, region, lhs, rhs) {}
};

class NeN : public CmpN {
public:
    NeN(UseE region, UseE lhs, UseE rhs) : CmpN(Nop::Ne, region, lhs, rhs) {}
};

class LtN : public CmpN {
public:
    LtN(UseE region, UseE lhs, UseE rhs) : CmpN(Nop::Lt, region, lhs, rhs) {}
};

class LeN : public CmpN {
public:
    LeN(UseE region, UseE lhs, UseE rhs) : CmpN(Nop::Le, region, lhs, rhs) {}
};

class GtN : public CmpN {
public:
    GtN(UseE region, UseE lhs, UseE rhs) : CmpN(Nop::Gt, region, lhs, rhs) {}
};

class GeN : public CmpN {
public:
    GeN(UseE region, UseE lhs, UseE rhs) : CmpN(Nop::Ge, region, lhs, rhs) {}
};


}


#endif //C0_CMPN_H
