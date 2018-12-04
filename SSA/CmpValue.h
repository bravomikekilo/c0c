//
// Created by baomingkun on 18-11-29.
//

#ifndef C0_CMPVALUE_H
#define C0_CMPVALUE_H

#include "ValueBase.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {


class CmpV: public ValueNode {
protected:
    ValueE lhs;
    ValueE rhs;

public:
    CmpV(Vop op, Region *region, ValueE lhs, ValueE rhs)
        :ValueNode(region, op), lhs(lhs), rhs(rhs) {}

    size_t num_vin() override {
        return 0;
    }

    bool needReg() override {
        return false;
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

class NeV: public CmpV {
public:
    NeV(Region *region, ValueE lhs, ValueE rhs)
        :CmpV(Vop::Ne, region, lhs, rhs) {}

};





}

#endif //C0_CMPVALUE_H
