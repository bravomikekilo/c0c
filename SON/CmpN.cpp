//
// Created by baomingkun on 18-12-14.
//

#include "CmpN.h"
#include "SCCP.h"
#include "Sea.h"
#include "ConstN.h"

namespace C0 {


void CmpN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto region_type = uses[0]->Payload<T>();
    auto type = Payload<T>();

    if(region_type->height == T::Top) {
        type->height = T::Top;
        return;
    }

    const auto *l_type = uses[1]->Payload<T>();
    const auto *r_type = uses[2]->Payload<T>();


    if(l_type->height == T::Top || r_type->height == T::Top) {
        type->height = T::Top;
        return;
    }

    if(l_type->height == T::Constant && r_type->height == T::Constant) {
        type->height = T::Constant;
        type->type = T::Value;
        if(l_type->type == r_type->type) {
            if(cmp(l_type->constant, r_type->constant)) {
                type->constant = 1;
            } else {
                type->constant = 0;
            }
        } else {
            type->constant = 0;
        }
        return;
    }

    type->height = T::Bottom;
}

UseE CmpN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();

    if(type->height == T::Top) {
        return nullptr;
    } else if(type->height == T::Bottom) {
        return this;
    } else {
        return sea.alloc<ConstIntN>(uses[0], type->constant);
    }
}


}
