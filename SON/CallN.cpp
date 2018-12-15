//
// Created by baomingkun on 18-12-14.
//

#include "CallN.h"
#include "SCCP.h"

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

}