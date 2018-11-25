//
// Created by baomingkun on 18-11-22.
//

#ifndef C0_VALUENODE_H
#define C0_VALUENODE_H

#include "ValueBase.h"
#include "common.h"

namespace C0 {

class ConstV: public ValueNode {
public:
    ConstV(Region *region, Vop op)
        :ValueNode(region, op) {};
};

class ConstIntV: public ConstV {
protected:
    int val;
public:
    ConstIntV(Region *region, int val)
        :ConstV(region, Vop::ConstInt), val(val) {}
};

class ConstAddrV: public ConstV {
public:
    ConstAddrV(Region *region, Vop op)
        :ConstV(region, op) {}

};

class StackSoltV: public ConstV {
private:
    int var_id;

public:
    StackSoltV(Region *region, int var_id)
        :ConstV(region, Vop::StackSlot), var_id(var_id) {}

};

class GlobalAddr: public ConstV {
private:
    string name;

public:
    GlobalAddr(Region *region, string name)
        :ConstV(region, Vop::GlobalAddr), name(std::move(name)) {}
};

class FpV: public ValueNode {
public:
    explicit FpV(Region *region)
        :ValueNode(region, Vop::Fp) {}
};



class AddV: public ValueNode {
protected:
    ValueE lhs;
    ValueE rhs;
public:
    AddV(Region *region, ValueE lhs, ValueE rhs)
        :ValueNode(region, Vop::Add), lhs(lhs), rhs(rhs) {}
};


class SubV: public ValueNode {
protected:
    ValueE lhs;
    ValueE rhs;
public:
    SubV(Region *region, ValueE lhs, ValueE rhs)
        :ValueNode(region, Vop::Sub), lhs(lhs), rhs(rhs) {}
};


class MulV: public ValueNode {
protected:
    ValueE lhs;
    ValueE rhs;
public:
    MulV(Region *region, ValueE lhs, ValueE rhs)
        :ValueNode(region, Vop::Mul), lhs(lhs), rhs(rhs) {}

};


class DivV: public ValueNode {
protected:
    ValueE lhs;
    ValueE rhs;
public:
    DivV(Region *region, ValueE lhs, ValueE rhs)
        :ValueNode(region, Vop::Div), lhs(lhs), rhs(rhs) {}

};

class CmpV: public ValueNode {
protected:
    ValueE lhs;
    ValueE rhs;

public:
    CmpV(Vop op, Region *region, ValueE lhs, ValueE rhs)
        :ValueNode(region, op), lhs(lhs), rhs(rhs) {}

};

class GtV: public CmpV {
public:
    GtV(Region *region, ValueE lhs, ValueE rhs)
        :CmpV(Vop::Gt, region, lhs, rhs) {}

};

class GeV: public CmpV {
public:
    GeV(Region *region, ValueE lhs, ValueE rhs)
        :CmpV(Vop::Ge, region, lhs, rhs) {}

};

class LtV: public CmpV {
public:
    LtV(Region *region, ValueE lhs, ValueE rhs)
        :CmpV(Vop::Lt, region, lhs, rhs) {}

};

class LeV: public CmpV {
public:
    LeV(Region *region, ValueE lhs, ValueE rhs)
        :CmpV(Vop::Le, region, lhs, rhs) {}

};

class EqV: public CmpV {
public:
    EqV(Region *region, ValueE lhs, ValueE rhs)
        :CmpV(Vop::Eq, region, lhs, rhs) {}

};

class UnEqV: public CmpV {
public:
    UnEqV(Region *region, ValueE lhs, ValueE rhs)
        :CmpV(Vop::UnEq, region, lhs, rhs) {}

};

class CopyV: public ValueNode {
private:
    ValueE src;
public:
    CopyV(Region *region, ValueE src)
        :ValueNode(region, Vop::Copy), src(src) {}


};

class SelectV: public ValueNode {
protected:
    vector<ValueE> values;
public:
    SelectV(Region *region, vector<ValueE>&& vals)
        :ValueNode(region, Vop::Select), values(std::move(vals)) {}
};


class ComposeV: public ValueNode {
protected:
    vector<SelectV *> selects;

public:
    ComposeV(Region *region, vector<SelectV *>&& selects)
        :ValueNode(region, Vop::Compose), selects(std::move(selects)) {}
};


class UndefV: public ValueNode {

public:
    explicit UndefV(Region *region)
        :ValueNode(region, Vop::Undef) {}
};


class SetV: public ValueNode {
protected:
    ValueE index;
    ValueE val;

public:

    SetV(Vop op, Region *region, ValueE index, ValueE val)
        :ValueNode(region, op), index(index), val(val) {}

};


class SetIntV: public SetV {

public:
    SetIntV(Region *region, ValueE index, ValueE val)
        :SetV(Vop::SetInt, region, index, val) {}

};


class SetCharV: public SetV {

public:
    SetCharV(Region *region, ValueE index, ValueE val)
        :SetV(Vop::SetChar, region, index, val) {}

};

class GetV: public ValueNode {
protected:
    ValueE arr;
    ValueE index;

public:

    GetV(Vop op, Region *region, ValueE arr, ValueE index)
        :ValueNode(region, op), arr(arr), index(index) {}

};

class GetIntV: public GetV {

public:

    GetIntV(Region *region, ValueE arr, ValueE index)
        :GetV(Vop::GetInt, region, arr, index) {}

};


class GetCharV: public GetV {

public:
    GetCharV(Region *region, ValueE arr, ValueE index)
        :GetV(Vop::GetChar, region, arr, index) {}

};

class CallV: public ValueNode {
private:
    string func_name;
    vector<ValueE> args;
    ValueE world;

public:

    CallV(Region *region, string func_name, ValueE world, vector<ValueE>&& args)
        :ValueNode(region, Vop::Call) ,func_name(std::move(func_name)), world(world), args(std::move(args)) {}

};

class RetV: public ValueNode {
private:
    vector<ValueE> globals;
    optional<ValueE> ret;
    ValueE world;

public:

    RetV(Region *region, optional<ValueE> ret,  vector<ValueE> &&globals)
        :ValueNode(region, Vop::Return), globals(std::move(globals)), ret(std::move(ret)) {}

};


class ReadV: public ValueNode {

};


class PrintV: public ValueNode {
private:
    int str_id;
    ValueE world;

};









} // end namespace C0



#endif //C0_VALUENODE_H
