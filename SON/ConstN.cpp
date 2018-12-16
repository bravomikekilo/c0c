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
        type->label = {label};
        type->constant = offset;
    } else {
        type->height = T::Top;
    }
}

UseE GlobalAddrN::sameLabel(int offset, Sea &sea) {
    if(offset == this->offset) {
        return this;
    } else {
        return sea.alloc<GlobalAddrN>(uses[0], offset, label);
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

UseE ConstN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    if(uses[0]->Payload<T>()->height == T::Top) {
        return nullptr;
    } else {
        return this;
    }
}
}
