#pragma once

#include "InstBase.h"
#include "Reg.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class ArithInst : public InstBase {
protected:
    Reg lhs;
    Reg rhs;
    Reg dst;
    virtual string op() = 0;

public:
    ArithInst(Reg lhs, Reg rhs, Reg dst)
        :lhs(lhs), rhs(rhs), dst(dst) {}

    // Inherited via Inst
    string toString(const unordered_map<string, int> &funcs) override {
        if(lhs.getK() == Reg::RegK::I) {
            return fmt::format("{} {}, {}, {}", op(), dst.toStr(), rhs.toStr(), lhs.toStr());
        } else {
            return fmt::format("{} {}, {}, {}", op(), dst.toStr(), lhs.toStr(), rhs.toStr());
        }
    }
};

class AddI : public ArithInst {
protected:
    string op() override {
        return "add";
    }

public:
    AddI(Reg dst, Reg lhs, Reg rhs)
        :ArithInst(lhs, rhs, dst) {}
};

class SubI : public ArithInst {
protected:
    string op() override {
        return "sub";
    }


public:
    SubI(Reg dst, Reg lhs, Reg rhs)
        :ArithInst(lhs, rhs, dst) {}

    string toString(const unordered_map<string, int> &funcs) override {
        if(lhs.getK() == Reg::RegK::I) {
            return fmt::format("{0} {1}, {2}, {3}\nneg {1}, {1}", op(), dst.toStr(), rhs.toStr(), lhs.toStr());
        } else {
            return fmt::format("{} {}, {}, {}", op(), dst.toStr(), lhs.toStr(), rhs.toStr());
        }
    }


};

class MulI : public ArithInst {
protected:
    string op() override {
        return "mul";
    }



public:
    MulI(Reg dst, Reg lhs, Reg rhs)
        :ArithInst(lhs, rhs, dst) {}


};

class DivI : public ArithInst {
protected:
    string op() override {
        return "div";
    }

public:
    DivI(Reg dst, Reg lhs, Reg rhs)
        :ArithInst(lhs, rhs, dst) {}

    string toString(const unordered_map<string, int> &funcs) override {
        if(lhs.getK() == Reg::RegK::I) {
            return fmt::format("li $at {2}\n{0} {1}, $at, {3}", op(), dst.toStr(), lhs.toStr(), rhs.toStr());
        } else {
            return fmt::format("{} {}, {}, {}", op(), dst.toStr(), lhs.toStr(), rhs.toStr());
        }

    }


};





}

