//
// Created by baomingkun on 18-12-6.
//


#include "ConstN.h"
#include "SCCP.h"

namespace C0 {


void ConstIntN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();

    type->height = T::Top;
    type->constant = v;
}


void ConstCharN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();

    type->height = T::Top;
    type->constant = v;
}



}
