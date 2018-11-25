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
    UnEq,

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
    Read,
    Print,

    // Phi instruction
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

    Undef,
};

class Region;

class ValueNode: NodeBase {
protected:
    Region *region;
    Vop op;
public:
    ValueNode(Region *region, Vop op): region(region), op(op) {}
};

using ValueE = ValueNode *;











} // end namespace C0




