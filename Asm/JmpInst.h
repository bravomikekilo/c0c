#pragma once

#include "Inst.h"
#include "Reg.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class BInst : public Inst {
    // Inherited via Inst
    virtual string toString() override;
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
        return fmt::format("{} {} {} {}", cond(), lhs->toString(), rhs->toString(), label);
    }
};

class BeqInst : public BranchInst {
    // Inherited via Inst
    // Inherited via BranchInst
public:
    BeqInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), label) {}


};

class BneInst : public BranchInst {
    
public:
    BneInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), label) {}


};

class BgtInst : public BranchInst {


public:
    BgtInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), label) {}


};

class BgeInst : public BranchInst {

public:
    BgeInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), label) {}


};

class BltInst : public BranchInst {

public:
    BltInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), label) {}


};

class BleInst : public BranchInst {

public:
    BleInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, string label)
        :BranchInst(std::move(lhs), std::move(rhs), label) {}


};











}
