//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_NODE_H
#define C0_NODE_H

#include "common.h"

namespace C0 {


enum class Nop {
    Region,
    Stop,
    If,
    IfProj,

    Add,
    Sub,
    Mul,
    Div,
    Call,

    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,

    PrintInt,
    PrintChar,

    ReadChar,
    ReadInt,

    GlobalAddr,
    StackSlot,

    Int,
    Char,

    GetInt,
    GetChar,

    SetInt,
    SetChar,

    ProjRet,
    ProjArg,
    ProjWorld,
    InitWorld,
    InitGlobal,
    Phi,
    Undef

};

string nopToStr(Nop op);

class Node;

using UseE = Node *;

class Node {
private:
    void *_payload = nullptr;

protected:
    Nop op;
    UseE *uses;
    size_t num_uses;

public:

    void setPayload(void *payload) {
        _payload = payload;
    }

    template<typename T>
    T *Payload() {
        return (T *) _payload;
    }

    size_t numUse() { return num_uses; }

    void push_back(const UseE &use) {
        auto arr = new UseE[num_uses + 1];
        for (int i = 0; i < num_uses; ++i) {
            arr[i] = uses[i];
        }
        ++num_uses;
        arr[num_uses] = use;
        delete[] uses;
        uses = arr;
    }

    Nop getOp() {return op;}

    UseE at(size_t index) {
        return uses[index];
    }

    UseE operator[](size_t index) {
        return uses[index];
    }

    UseE *begin() { return uses; }

    UseE front() { return *begin(); }

    const UseE *cbegin() { return uses; }

    UseE *end() { return uses + num_uses; }

    UseE back() { return *(end() - 1); }

    const UseE *cend() { return uses + num_uses; }


    explicit Node(Nop op, size_t num_uses) : op(op), num_uses(num_uses) {
        uses = new UseE[num_uses];
    }

    virtual string str() {
        return nopToStr(op);
    };

    virtual ~Node() {
        delete[] uses;
    }
};


}


#endif //C0_NODE_H
