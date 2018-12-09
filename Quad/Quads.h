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
#include <functional>

namespace C0 {


class BasicBlock;

enum class QuadOp {
    Add, Sub, Mul, Div,
    Copy, Call, Ret, Read, PrintInt, PrintChar, GetInt, GetChar,
    SetInt, SetChar, B,
    Beq, Bne, Bgt, Bge, Blt, Ble
};

inline bool isBranchOp(QuadOp op) {
    return op >= QuadOp::B;
}

inline bool isGetOp(QuadOp op) {
    return op >= QuadOp::GetInt && op <= QuadOp::GetChar;
}

inline bool isSetOp(QuadOp op) {
    return op >= QuadOp::SetInt && op <= QuadOp::SetChar;
}

string opToString(QuadOp op);


struct QuadVal {
    bool isConst;
    int val;
    QuadVal(): isConst(false), val(0) {}
    explicit QuadVal(int val, bool c=false): val(val), isConst(c) {}
    bool operator==(const QuadVal &other) const {
        return isConst == other.isConst && other.val == val;
    }

    optional<int> constVal(const SymTable& table) const ;

};



// using QuadVal = std::variant<VarID, int>;

string varToString(QuadVal v, shared_ptr<SymTable> table);


struct Quad {


    QuadOp op;

    QuadVal src0;
    QuadVal src1;
    QuadVal dst;

    BasicBlock *jmp;
    int str_id = -1;
    using FuncExtT = pair<string, vector<QuadVal>>;

    Quad(): op(QuadOp::Ret), jmp(nullptr), call_ext(nullptr) {}


    explicit Quad(QuadVal ret)
        :op(QuadOp::Ret), src0(ret), jmp(nullptr), call_ext(nullptr){

    }

    Quad(int str_id, QuadVal exp, bool is_char=false)
        :op(is_char ? QuadOp::PrintChar : QuadOp::PrintInt),
         jmp(nullptr), call_ext(nullptr), src0(exp) {
        this->str_id = str_id;
    }

    explicit Quad(vector<QuadVal>&& arg)
    :op(QuadOp::Read), jmp(nullptr), call_ext(make_unique<FuncExtT>("", std::move(arg))){}


    Quad(BasicBlock *target)
             :op(QuadOp::B), call_ext(nullptr), jmp(target){}


    Quad(string name, vector<QuadVal>&& arg, QuadVal dst)
    :call_ext(make_unique<FuncExtT>(std::move(name), std::move(arg)))
    , op(QuadOp::Call), jmp(nullptr) {}

    Quad(QuadOp cond, QuadVal lhs, QuadVal rhs, BasicBlock *target)
        :op(cond), src0(lhs), src1(rhs), jmp(target), call_ext(nullptr) {}

    Quad(QuadVal dst, QuadOp op, QuadVal src0, QuadVal src1)
            : src0(src0), src1(src1), op(op), dst(dst),
              jmp(nullptr), call_ext(nullptr) {}


    unique_ptr<pair<string, vector<QuadVal>>> call_ext; /// args for call op

    string toString(shared_ptr<SymTable> table) const ;

};


}

namespace std {

template<>
struct hash<C0::QuadVal> {
    typedef C0::QuadVal argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& s) const {
        return hash<int>{}(s.val);
    }
};

}

#endif //C0_QUADS_H
