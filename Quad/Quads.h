//
// Created by baomingkun on 18-11-18.
//

#ifndef C0_QUADS_H
#define C0_QUADS_H


#include "common.h"
#include "SymTable.h"

#define FMT_HEADER_ONLY

#include "fmt/format.h"
#include <variant>

namespace C0 {


class BasicBlock;

enum class QuadOp {
    Add, Sub, Mul, Div,
    Copy, Call, GetInt, GetChar,
    SetInt, SetChar, B,
    Beq, Bne, Bgt, Bge, Blt, Ble
};

bool isBranchOp(QuadOp op) {
    return op >= QuadOp::B;
}

bool isGetOp(QuadOp op) {
    return op >= QuadOp::GetInt && op <= QuadOp::GetChar;
}

bool isSetOp(QuadOp op) {
    return op >= QuadOp::SetInt && op <= QuadOp::SetChar;
}

string opToString(QuadOp op);


struct QuadVal {
    bool isConst;
    int val;
    QuadVal(): isConst(false), val(0) {}
    explicit QuadVal(int val, bool c=false): val(val), isConst(c) {}
};



// using QuadVal = std::variant<VarID, int>;

string varToString(QuadVal v, shared_ptr<SymTable> table);


struct Quad {


    QuadOp op;

    QuadVal src0;
    QuadVal src1;
    QuadVal dst;

    BasicBlock *jmp;

    using FuncExtT = pair<string, vector<QuadVal>>;

    Quad(string name, vector<QuadVal>&& arg, QuadVal dst)
    :call_ext(make_unique<FuncExtT>(std::move(name), std::move(arg)))
    , op(QuadOp::Call), jmp(nullptr) {}

    Quad(QuadOp cond, QuadVal lhs, QuadVal rhs, BasicBlock *target)
        :op(cond), src0(lhs), src1(rhs), jmp(target) {}

    Quad(QuadVal dst, QuadOp op, QuadVal src0, QuadVal src1)
            : src0(src0), src1(src1), op(op), dst(dst),
              jmp(nullptr), call_ext(nullptr) {}


    unique_ptr<pair<string, vector<QuadVal>>> call_ext; /// args for call op

    string toString(shared_ptr<SymTable> table);


};


}

#endif //C0_QUADS_H
