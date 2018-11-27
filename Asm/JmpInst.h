#pragma once

#include "Inst.h"
#include "Reg.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {


class LInst : public Inst {
    string label;

public:
    string toString() override {
        return label + ":";
    }
    explicit LInst(string str): label(std::move(str)) {}
};

class BInst : public Inst {
    // Inherited via Inst
private:
    string label;
public:
    string toString() override;

    explicit BInst(string label): label(std::move(label)) {}
};


class RetInst : public Inst {
public:
    string toString() override {
        return "jr $ra";
    }
};

class CallInst : public Inst {
private:
    string func_name;

public:
    string toString() override {
        return "jal " + func_name;
    }

    explicit CallInst(string name): func_name(std::move(name)) {}

};


class BranchInst : public Inst {
protected:
    string label;
    unique_ptr<Reg> lhs;
    unique_ptr<Reg> rhs;
    virtual string cond() const = 0;
public:
    BranchInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :lhs(std::move(lhs)), rhs(std::move(rhs)), label(std::move(label)) {} 

    string toString() override {
        return fmt::format("{} {}, {}, {}", cond(), lhs->toString(), rhs->toString(), label);
    }
};

class BeqInst : public BranchInst {
    // Inherited via Inst
    // Inherited via BranchInst
public:

    string cond() const override {
        return "beq";
    }

    BeqInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), std::move(label)) {}


};

class BneInst : public BranchInst {
    
public:

    string cond() const override {
        return "bne";
    }

    BneInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), std::move(label)) {}


};

class BgtInst : public BranchInst {


public:

    string cond() const override {
        return "bgt";
    }

    BgtInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), std::move(label)) {}


};

class BgeInst : public BranchInst {

public:

    string cond() const override {
        return "bge";
    }

    BgeInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), std::move(label)) {}


};

class BltInst : public BranchInst {

public:

    string cond() const override {
        return "blt";
    }

    BltInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), std::move(label)) {}


};

class BleInst : public BranchInst {

public:

    string cond() const override {
        return "ble";
    }

    BleInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), std::move(label)) {}


};











}
