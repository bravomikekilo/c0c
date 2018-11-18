//
// Created by baomingkun on 18-11-16.
//

#ifndef C0_IRNODE_H
#define C0_IRNODE_H

#include "common.h"
namespace C0 {


class IRNode {

};

enum class IROp {
    Add,
    Sub,
    Mul,
    Div,
    Copy,
    If,
    Return,
    Select,
    Compose,
    Const,
    Region,
    Start,
};


struct IRInst {
    IROp op;
    explicit IRInst(IROp op) : op(op) {}
};


struct If : public IRInst {
    IRInst *control;
    IRInst *pred;
    If(IRInst *p, IRInst *c)
        :control(c), pred(p), IRInst(IROp::If) {}
};

struct Const : public IRInst {
    const int val;
    IRInst *control;

    explicit Const(int con) : val(con), IRInst(IROp::Const) {}
};

struct Copy : public IRInst {
    IRInst *control;
    IRInst *src;

    explicit Copy(IRInst *src) : src(src), IRInst(IROp::Copy) {}
};

struct Add : public IRInst {
    IRInst *control;
    IRInst *src0;
    IRInst *src1;

    Add(IRInst *src0, IRInst *src1, IRInst *c)
            : src0(src0), src1(src1), control(c), IRInst(IROp::Add) {}
};


struct Sub : public IRInst {
    IRInst *control;
    IRInst *src0;
    IRInst *src1;

    Sub(IRInst *src0, IRInst *src1, IRInst *c)
            : src0(src0), src1(src1), control(c), IRInst(IROp::Sub) {}
};

struct Mul : public IRInst {
    IRInst *control;
    IRInst *src0;
    IRInst *src1;

    Mul(IRInst *src0, IRInst *src1, IRInst *c)
            : src0(src0), src1(src1), control(c), IRInst(IROp::Mul) {}
};

struct Div : public IRInst {
    IRInst *control;
    IRInst *src0;
    IRInst *src1;

    Div(IRInst *src0, IRInst *src1, IRInst *c)
            : src0(src0), src1(src1), control(c), IRInst(IROp::Div) {}
};

struct Region : public IRInst {
    vector<IRInst *> srcs;
    explicit Region(vector<IRInst *>&& srcs)
        : srcs(std::move(srcs)), IRInst(IROp::Region) {}
};

struct Return : public IRInst {
    IRInst *r;
    IRInst *control;
    Return(IRInst *r, IRInst *c)
        : r(r), control(c), IRInst(IROp::Return) {}
};


struct Phi : public IRInst {

};

struct ProjTrue : public IRInst {

};

struct Start : public IRInst {

};

class Load : public IRNode {

};

class LoadInt : public Load {

};

class LoadChar : public Load {

};

class Store : public IRNode {

};

class StoreChar : public Store {

};

class StoreInt : public Store {

};



} // end namespace C0
#endif //C0_IRNODE_H
