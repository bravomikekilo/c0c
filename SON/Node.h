//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_NODE_H
#define C0_NODE_H

#include "common.h"
#include "Arch/Reg.h"

#define FMT_HEADER_ONLY

#include "fmt/format.h"
#include "NLoad.h"

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
    ProjGlobal,
    InitWorld,
    InitGlobal,
    Phi,
    Undef,
    Copy

};

string nopToStr(Nop op);

class Node;

using UseE = Node *;

class Sea;

class ProjN;

class Node {
public:
    using User = unordered_set<UseE>;

private:
    NLoad *_payload = nullptr;
    int visit_flag = 0;

protected:
    Nop op;   // Node Op for the node
    UseE *uses = nullptr; // pointer to array of user
    size_t num_uses; // number of use

    unique_ptr<User> user = nullptr;

public:

    int serial = -1;

    virtual bool asCode() {
        return true;
    }

    virtual string exprAsUse();

    virtual string asText() {
        string base = fmt::format("v{} = {}", std::to_string(serial), nopToStr(op));
        auto sz = this->size();
        for(size_t i = 1; i < sz; ++i) {
            base += " ";
            base += this->at(i)->exprAsUse();
        }
        return base;
    };


    void setPayload(NLoad *payload) {
        delete _payload;
        _payload = payload;
    }

    void freePayload() {
        delete _payload;
        _payload = nullptr;
    }

    template<typename T>
    T *Payload() {
        return (T *) _payload;
    }

    void clearDefUse() { user = nullptr; }

    void initDefUse() { user = make_unique<User>(); }

    void addUser(UseE use) {
        if (user) {
            user->insert(use);
        }
    }

    void addUser(User &other) {
        if (user) {
            user->insert(other.begin(), other.end());
        }
    }

    size_t getNumUse() {
        if (user == nullptr) return 0;
        else return user->size();
    }

    User &getUser() {
        return *user;
    }

    size_t size() const { return num_uses; }

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

    Nop getOp() const { return op; }

    UseE at(size_t index) const {
        return uses[index];
    }

    UseE operator[](size_t index) const {
        return uses[index];
    }

    UseE *begin() { return uses; }

    UseE front() { return *begin(); }

    const UseE *cbegin() { return uses; }

    UseE *end() { return uses + num_uses; }

    UseE back() { return *(end() - 1); }

    const UseE *cend() { return uses + num_uses; }

    bool replace(UseE old, UseE n) {
        bool has = false;
        for (auto &use: *this) {
            if (use == old) {
                use = n;
                has = true;
            }
        }
        return has;
    }


    explicit Node(Nop op, size_t num_uses) : op(op), num_uses(num_uses) {
        uses = new UseE[num_uses];
    }


    virtual string str() {
        if (_payload) {
            return fmt::format("$:{2} #{0} {1} ", size(), nopToStr(op), _payload->toStr());
        } else {
            return fmt::format("#{} {}", size(), nopToStr(op));
        }
    };

    virtual ~Node() {
        delete[] uses;
    }

    // methods for optimization

    virtual void SCCPType() {};

    virtual void SCCPType(ProjN *n) { exit(-2); }

    virtual UseE SCCPIdentity(Sea &sea);

    virtual UseE SCCPIdentity(Sea &sea, ProjN *projection);

    virtual bool needReg() {
        return true;
    };

    virtual bool same(const Node &other) {
        if(other.size() != size() || op != other.op) return false;
        for(auto i = 0; i < size(); ++i) {
            if(uses[i] != other.uses[i]) {
                return false;
            }
        }
        return true;
    }

    unique_ptr<Reg> reg = nullptr;

};


}


#endif //C0_NODE_H
