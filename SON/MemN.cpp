//
// Created by baomingkun on 18-12-14.
//

#include "MemN.h"
#include "SCCP.h"

namespace C0 {


void SetN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    bool bottom = true;
    for(auto use : *this) {
        auto use_t = use->Payload<T>();
        if(use_t->height == T::Top) {
            bottom = false;
        }
    }

    if(bottom) {
        type->height = T::Bottom;
    }

}

UseE SetN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    if(type->height == T::Top) {
        return nullptr;
    } else {
        return this;
    }
}

void GetN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    bool bottom = true;

    for(auto use : *this) {
        auto use_t = use->Payload<T>();
        if(use_t->height == T::Top) {
            bottom = false;
        }
    }

    if(bottom) {
        type->height = T::Bottom;
    }

}

UseE GetN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    if(type->height == T::Top) {
        return nullptr;
    } else {
        return this;
    }
}

}
