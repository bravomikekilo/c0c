
#pragma once

#include "InstBase.h"
#include "Reg.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class LiInst : public InstBase {
private:
    Reg reg;
    int val;

public:
    LiInst(Reg reg, int v) : reg(reg), val(v) {}
    string toString(const unordered_map<string, int> &funcs) override {
        return fmt::format("li {}, {}", reg.toStr(), val);
    }
};

class MoveInst : public InstBase {
private:
    Reg lhs;
    Reg rhs;

public:
    MoveInst(Reg lhs, Reg rhs)
        :lhs(lhs), rhs(rhs) {}
    
    string toString(const unordered_map<string, int> &funcs) override {
        return fmt::format("move {}, {}", lhs.toStr(), rhs.toStr());
    }
};

class SysCallInst : public InstBase {
public:

    string toString(const unordered_map<string, int> &funcs) override {
        return "syscall";
    }
};


}





