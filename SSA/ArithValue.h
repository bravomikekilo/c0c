//
// Created by baomingkun on 18-11-29.
//

#ifndef C0_ARITHVALUE_H
#define C0_ARITHVALUE_H

#include "ValueBase.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class ArithV: public ValueNode {
protected:
    ValueE lhs;
    ValueE rhs;

public:
    ArithV(Vop op, Region *region, ValueE lhs, ValueE rhs)
        :ValueNode(region, op), lhs(lhs), rhs(rhs) {}

    size_t num_vin() override {
        return 0;
    }

    bool needReg() override {
        return true;
    }

    ValueNode *&vin(size_t index) override {
        if(index == 0) {
            return lhs;
        } else if(index == 1) {
            return rhs;
        } else {
            throw std::out_of_range(fmt::format("{} overflow at index {}", vopToString(op), index));
        }
    }

};

class AddV : public ArithV {
public:
    AddV(Region *region, ValueE lhs, ValueE rhs)
            : ArithV(Vop::Add, region, lhs, rhs) {}
};


class SubV : public ArithV {
public:
    SubV(Region *region, ValueE lhs, ValueE rhs)
            : ArithV(Vop::Sub, region, lhs, rhs) {}

};


class MulV : public ArithV {
public:
    MulV(Region *region, ValueE lhs, ValueE rhs)
            : ArithV(Vop::Mul, region, lhs, rhs) {}

};


class DivV : public ArithV {
public:
    DivV(Region *region, ValueE lhs, ValueE rhs)
            : ArithV(Vop::Div, region, lhs, rhs) {}

};

}

#endif //C0_ARITHVALUE_H
