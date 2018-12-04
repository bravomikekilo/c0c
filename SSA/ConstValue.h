//
// Created by baomingkun on 18-11-29.
//

#ifndef C0_CONSTVALUE_H
#define C0_CONSTVALUE_H

#include "ValueBase.h"
#include "common.h"

namespace C0 {

class ConstV: public ValueNode {
public:
    ConstV(Region *region, Vop op)
        :ValueNode(region, op) {}

    size_t num_vin() override {
        return 0;
    }

    ValueNode *&vin(size_t index) override {
        throw std::out_of_range("ConstV overflow " + std::to_string(index));
    };

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


class FpV: public ConstV {
public:
    explicit FpV(Region *region)
        :ConstV(region, Vop::Fp) {}
};






}
#endif //C0_CONSTVALUE_H
