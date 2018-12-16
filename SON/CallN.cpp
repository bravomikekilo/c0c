//
// Created by baomingkun on 18-12-14.
//

#include "CallN.h"
#include "SCCP.h"
#include "ProjN.h"

namespace C0 {


void CallN::SCCPType() {
    typedef SCCPOptimizer::T T;
    bool bottom = true;
    for(auto use: *this) {
        auto type = use->Payload<T>();
        if(type->height == T::Top) {
            bottom = false;
            break;
        }
    }

    if(bottom) {
        auto t = this->Payload<T>();
        t->height = T::Bottom;
    }

}


void CallN::SCCPType(ProjN *n) {
    typedef SCCPOptimizer::T T;
    auto t = this->Payload<T>();

    auto p_type = n->Payload<T>();

    p_type->height = t->height;
}


void PrintN::SCCPType() {
    typedef SCCPOptimizer::T T;
    bool bottom = true;
    for(auto use: *this) {
        auto type = use->Payload<T>();
        if(type->height == T::Top) {
            bottom = false;
            break;
        }
    }

    if(bottom) {
        auto t = this->Payload<T>();
        t->height = T::Bottom;
    }

}

UseE PrintN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    if(type->height == T::Top) {
        return nullptr;
    } else {
        return this;
    }
}

void ReadN::SCCPType() {
    typedef SCCPOptimizer::T T;
    bool bottom = true;
    for(auto use: *this) {
        auto type = use->Payload<T>();
        if(type->height == T::Top) {
            bottom = false;
            break;
        }
    }

    if(bottom) {
        auto t = this->Payload<T>();
        t->height = T::Bottom;
    }
}

void ReadN::SCCPType(ProjN *n) {
    typedef SCCPOptimizer::T T;
    auto t = this->Payload<T>();
    auto p_type = n->Payload<T>();
    p_type->height = t->height;
}

UseE ReadN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto t = this->Payload<T>();
    if(t->height == T::Top) {
        return nullptr;
    } else {
        return this;
    }
}

UseE ReadN::SCCPIdentity(Sea &sea, ProjN *projection) {
    typedef SCCPOptimizer::T T;
    auto t = this->Payload<T>();
    if(t->height == T::Top) {
        return nullptr;
    } else {
        return projection;
    }
}

}