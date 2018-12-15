//
// Created by baomingkun on 18-12-6.
//


#include "ConstN.h"
#include "SCCP.h"

namespace C0 {


void ConstIntN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();
    if (uses[0]->Payload<T>()->height == T::Bottom) {
        type->height = T::Constant;
        type->type = T::Value;
        type->constant = v;
    } else {
        type->height = T::Top;
    }
}


void ConstCharN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();
    if (uses[0]->Payload<T>()->height == T::Bottom) {
        type->height = T::Constant;
        type->type = T::Value;
        type->constant = v;
    } else {
        type->height = T::Top;
    }
}


void GlobalAddrN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();

    if (uses[0]->Payload<T>()->height == T::Bottom) {
        type->height = T::Constant;
        type->type = T::Label;
        type->constant = offset;
    } else {
        type->height = T::Top;
    }
}


void StackSlotN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();

    if (uses[0]->Payload<T>()->height == T::Bottom) {
        type->height = T::Constant;
        type->type = T::Pointer;
        type->constant = offset;
    } else {
        type->height = T::Top;
    }
}

}
