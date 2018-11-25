#pragma once

#include "Inst.h"
#include "Reg.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class ArithInst : public Inst {
protected:
    unique_ptr<Reg> lhs;
    unique_ptr<Reg> rhs;
    unique_ptr<Reg> dst;
    virtual string op() = 0;

public:
    ArithInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, unique_ptr<Reg>&& dst)
        :lhs(std::move(lhs)), rhs(std::move(rhs)), dst(std::move(dst)) {}

    // Inherited via Inst
    virtual string toString() override {
        return fmt::format("{} {}, {}, {}", op(), dst->toString(), lhs->toString(), rhs->toString());
    }
};

class AddI : public ArithInst {
protected:
    string op() override {
        return "add";
    }

public:
    AddI(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, unique_ptr<Reg>&& dst)
        :ArithInst(std::move(lhs), std::move(rhs), std::move(dst)) {}




};

class SubI : public ArithInst {
protected:
    string op() override {
        return "sub";
    }


public:
    SubI(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, unique_ptr<Reg>&& dst)
        :ArithInst(std::move(lhs), std::move(rhs), std::move(dst)) {}


};

class MulI : public ArithInst {
protected:
    string op() override {
        return "mul";
    }



public:
    MulI(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, unique_ptr<Reg>&& dst)
        :ArithInst(std::move(lhs), std::move(rhs), std::move(dst)) {}


};

class DivI : public ArithInst {
protected:
    string op() override {
        return "div";
    }

public:
    DivI(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs, unique_ptr<Reg>&& dst)
        :ArithInst(std::move(lhs), std::move(rhs), std::move(dst)) {}


};





}

