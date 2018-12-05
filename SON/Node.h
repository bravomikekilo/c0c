//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_NODE_H
#define C0_NODE_H

#include "common.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

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
public:
    using User = unordered_set<UseE>;

private:
    void *_payload = nullptr;
    int visit_flag = 0;

protected:
    Nop op;   // Node Op for the node
    UseE *uses = nullptr; // pointer to array of user
    size_t num_uses; // number of use

    unique_ptr<User> user = nullptr;

public:

    void setPayload(void *payload) {
        _payload = payload;
    }

    template<typename T>
    T *Payload() {
        return (T *) _payload;
    }

    void clearDefUse() { user = nullptr; }

    void initDefUse() { user = make_unique<User>(); }

    void addUse(UseE use) {
        if(user) {
            user->insert(use);
        }
    }

    void addUse(User &other) {
        if(user) {
            user->insert(other.begin(), other.end());
        }
    }

    size_t getNumUse() {
        if(user == nullptr) return 0;
        else return user->size();
    }

    User &getUser() {
        return *user;
    }

    size_t size() { return num_uses; }

    void push_back(const UseE &use) {
        auto arr = new UseE[num_uses + 1];
        for (int i = 0; i < num_uses; ++i) {
            arr[i] = uses[i];
        }
        arr[num_uses] = use;
        ++num_uses;
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

    bool replace(UseE old, UseE n) {
        for(auto &use: *this) {
            if(use == old) {
                use = n;
                return true;
            }
        }
        return false;
    }


    explicit Node(Nop op, size_t num_uses) : op(op), num_uses(num_uses) {
        uses = new UseE[num_uses];
    }

    virtual string str() {
        return fmt::format("#{} {}", size(), nopToStr(op));
    };

    virtual ~Node() {
        delete[] uses;
    }
};


}


#endif //C0_NODE_H
