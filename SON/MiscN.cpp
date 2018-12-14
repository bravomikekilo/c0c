//
// Created by baomingkun on 18-12-12.
//

#include "MiscN.h"
#include "SCCP.h"

namespace C0 {


void ProjRetN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    auto up_type = uses[1]->Payload<T>();
    if(up_type->height == T::Top) {
        type->height = T::Top;
    } else {
        type->height = T::Bottom;
    }
}


void ProjArgN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    type->height = T::Bottom;
}


void UndefN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    type->height = T::Bottom;
}


void InitWorldN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    type->height = T::Bottom;
}

}

