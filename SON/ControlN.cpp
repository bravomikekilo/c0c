//
// Created by baomingkun on 18-12-11.
//


#include "ControlN.h"
#include "SCCP.h"

using std::list;

namespace C0 {


void RegionN::Linearization() {
    list<UseE> phi;
    for (auto user: getUser()) {
        auto user_op = user->getOp();
        if (user_op != Nop::Region && user_op != Nop::IfProj) {

        }
    }
}

void RegionN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();

    for (auto use : *this) {
        auto use_t = use->Payload<T>();
        if (use_t->height != T::Top) {
            type->height = T::Bottom;
            return;
        }
    }

    if(this->size()) {
        type->height = T::Top;
    } else {
        type->height = T::Bottom;
    }
}

void StopN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    for (auto use : *this) {
        auto use_t = use->Payload<T>();
        if (use_t->height != T::Top) {
            type->height = T::Bottom;
            return;
        }
    }
    type->height = T::Top;
}


void IfN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    auto src_type = uses[0]->Payload<T>();
    auto pre_type = uses[1]->Payload<T>();

    if(src_type->height == T::Top || pre_type->height == T::Top) {
        type->height = T::Top;
        return;
    }

    *type = *pre_type;

}

void IfN::SCCPType(ProjN *n) {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    auto n_type = n->Payload<T>();

    if(type->height != T::Constant) {
        n_type->height = type->height;
        return;
    }

    if(type->constant == n->field) {
        n_type->height = T::Bottom;
    } else {
        n_type->height = T::Top;
    }

}


}
