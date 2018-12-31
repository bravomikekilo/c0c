#pragma once

#include "InstBase.h"
#include "Reg.h"
#include "labelGen.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {


class LInst : public InstBase {
    string label;

public:
    string toString(const unordered_map<string, int> &funcs) override {
        return label + ":";
    }
    explicit LInst(string str): label(std::move(str)) {}
};


class BInst : public InstBase {
    // Inherited via Inst
private:
    string label;
public:
    string toString(const unordered_map<string, int> &funcs) override;

    explicit BInst(string label): label(std::move(label)) {}
};


class RetInst : public InstBase {
private:
    string func_name;

public:
    string toString(const unordered_map<string, int> &funcs) override {
        return fmt::format("sub $sp $sp {}\njal {}", funcs.at(func_name), genFuncLabel(func_name));
    }

    explicit RetInst(string name): func_name(std::move(name)) {}
};

class CallInst : public InstBase {
private:
    string func_name;

public:
    string toString(const unordered_map<string, int> &funcs) override {
        return fmt::format("sub $sp $sp {}\njal {}", funcs.at(func_name), genFuncLabel(func_name));
    }

    explicit CallInst(string name): func_name(std::move(name)) {}
};


class BranchInst : public InstBase {
protected:
    string label;
    Reg lhs;
    Reg rhs;
    virtual string cond() const = 0;
    virtual string invert() const = 0;
public:
    BranchInst(Reg lhs, Reg rhs, string label)
        :lhs(lhs), rhs(rhs), label(std::move(label)) {}

    string toString(const unordered_map<string, int> &funcs) override {
        if(lhs.getK() == Reg::RegK::I) {
            return fmt::format("{} {}, {}, {}", invert(), rhs.toStr(), lhs.toStr(), label);
        } else {
            return fmt::format("{} {}, {}, {}", cond(), lhs.toStr(), rhs.toStr(), label);
        }
    }
};

class BeqInst : public BranchInst {
    // Inherited via Inst
    // Inherited via BranchInst
public:

    string cond() const override {
        return "beq";
    }

    string invert() const override {
        return "bne";
    }

    BeqInst(Reg lhs, Reg rhs, string label)
        :BranchInst(lhs, rhs, std::move(label)) {}


};

class BneInst : public BranchInst {
    
public:

    string cond() const override {
        return "bne";
    }

    string invert() const override {
        return "beq";
    }

    BneInst(Reg lhs, Reg rhs, string label)
        :BranchInst(lhs, rhs, std::move(label)) {}


};

class BgtInst : public BranchInst {


public:

    string cond() const override {
        return "bgt";
    }

    string invert() const override {
        return "blt";
    }

    BgtInst(Reg lhs, Reg rhs, string label)
        :BranchInst(lhs, rhs, std::move(label)) {}


};

class BgeInst : public BranchInst {

public:

    string cond() const override {
        return "bge";
    }

    string invert() const override {
        return "ble";
    }

    BgeInst(Reg lhs, Reg rhs, string label)
        :BranchInst(lhs, rhs, std::move(label)) {}


};

class BltInst : public BranchInst {

public:

    string cond() const override {
        return "blt";
    }

    string invert() const override {
        return "bgt";
    }

    BltInst(Reg lhs, Reg rhs, string label)
        :BranchInst(lhs, rhs, std::move(label)) {}


};

class BleInst : public BranchInst {

public:

    string cond() const override {
        return "ble";
    }

    string invert() const override {
        return "bge";
    }

    BleInst(Reg lhs, Reg rhs, string label)
        :BranchInst(lhs, rhs, std::move(label)) {}


};











}
