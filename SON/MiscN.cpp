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


/// meet function for phi function
void PhiN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    auto sz = size();
    auto region = uses[0];

    if(region->Payload<T>()->height != T::Bottom) {
        return;
    }

    for(int i = 1; i < sz; ++i) {
        auto use = uses[i];
        auto use_t = use->Payload<T>();
        if(use_t->height == T::Bottom) {
            type->height = T::Bottom;
            return;
        }

        if(use_t->height == T::Top) {
            continue;
        }

        // use_t is constant

        if(*type == *use_t || type->height == T::Top) {
            *type = *use_t;
        } else {
            type->height = T::Bottom;
            return;
        }

    }


}

void InitGlobalN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    type->height = T::Bottom;
}

}

