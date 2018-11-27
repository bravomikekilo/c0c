
#pragma once

#include "Inst.h"
#include "Reg.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class LiInst : public Inst {
private:
    unique_ptr<Reg> reg;
    int val;

public:
    LiInst(unique_ptr<Reg>&& reg, int v) : reg(std::move(reg)), val(v) {}
    string toString() override {
        return fmt::format("li {} {}", reg->toString(), val);
    }
};

class MoveInst : public Inst {
private:
    unique_ptr<Reg> lhs;
    unique_ptr<Reg> rhs;

public:
    MoveInst(unique_ptr<Reg>&& lhs, unique_ptr<Reg>&& rhs)
        :lhs(std::move(lhs)), rhs(std::move(rhs)) {}
    
    string toString() override {
        return fmt::format("move {} {}", lhs->toString(), rhs->toString());
    }
};

class SysCallInst : public Inst {
public:

    string toString() override {
        return "syscall";
    }
};


}





