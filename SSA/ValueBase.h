#pragma once

#include "NodeBase.h"

namespace C0 {

enum class Vop {
    // arithmetic operation

    Add,
    Sub,
    Mul,
    Div,

    // cmp instruction

    Gt,
    Ge,
    Lt,
    Le,
    Eq,
    Ne,

    // copy instruction

    Copy,

    // load instruction

    GetInt,
    GetChar,

    // store instruction

    SetInt,
    SetChar,

    // Call function

    Call,
    ReadInt,
    ReadChar,

    PrintInt,
    PrintChar,

    // Phi instruction
    Phi,
    Select,
    Compose,

    // return instruction

    Return,

    // special values

    Fp, // frame_pointer

    // four kind of const value

    ConstInt,
    StackSlot,
    GlobalAddr,


    // projection node

    InitWorld,
    ProjWorld,
    ProjArg,
    ProjRet,

    Undef,
};

string vopToString(Vop op) {
    switch (op) {
        case Vop::Add:
            return "Add";
        case Vop::Sub:
            return "Sub";
        case Vop::Mul:
            return "Mul";
        case Vop::Div:
            return "Div";
        case Vop::Gt:
            return "Gt";
        case Vop::Ge:
            return "Ge";
        case Vop::Lt:
            return "Lt";
        case Vop::Le:
            return "Le";
        case Vop::Eq:
            return "Eq";
        case Vop::Ne:
            return "Ne";
        case Vop::Copy:
            return "Copy";
        case Vop::GetInt:
            return "GetInt";
        case Vop::GetChar:
            return "GetChar";
        case Vop::SetInt:
            return "SetInt";
        case Vop::SetChar:
            return "SetChar";
        case Vop::Call:
            return "Call";
        case Vop::ReadInt:
            return "ReadInt";
        case Vop::ReadChar:
            return "ReadChar";
        case Vop::PrintInt:
            return "PrintInt";
        case Vop::PrintChar:
            return "PrintChar";
        case Vop::Select:
            return "Select";
        case Vop::Compose:
            return "Compose";
        case Vop::Phi:
            return "Phi";
        case Vop::Return:
            return "Return";
        case Vop::Fp:
            return "Fp";
        case Vop::ConstInt:
            return "ConstInt";
        case Vop::StackSlot:
            return "StackSlot";
        case Vop::GlobalAddr:
            return "GlobalAddr";
        case Vop::ProjWorld:
            return "ProjWorld";
        case Vop::ProjArg:
            return "ProjArg";
        case Vop::ProjRet:
            return "ProjRetN";
        case Vop::Undef:
            return "Undef";
        case Vop::InitWorld:
            return "InitWorld";
            break;
    }
}

class Region;

class ValueNode : public NodeBase {
protected:
    Region *region;
    Vop op;

public:
    ValueNode(Region *region, Vop op) : region(region), op(op) {}

    size_t num_in() override {
        return num_vin() + 1;
    }

    NodeBase *in(size_t index) override {
        if (index == 0) return (NodeBase *) region;
        else {
            return vin(index - 1);
        }
    }

    Vop getOp() {
        return op;
    }

    Region *&control() {
        return region;
    }

    virtual size_t num_vin() = 0;
    virtual bool needReg() = 0;

    virtual ValueNode *&vin(size_t index) = 0;
    virtual string str() = 0;

};

using ValueE = ValueNode *;


} // end namespace C0




