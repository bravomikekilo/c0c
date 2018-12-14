//
// Created by baomingkun on 18-12-12.
//

#include "ArithN.h"
#include "SCCP.h"

namespace C0 {


void SubN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto lhs = uses[1];
    auto rhs = uses[2];

    const T *l_type = lhs->Payload<T>();
    const T *r_type = rhs->Payload<T>();

    auto type = Payload<T>();

    if(l_type->height == T::Top || r_type->height == T::Top) {
        type->height = T::Top;
        return;
    }


    if(r_type->height == T::Bottom || l_type->height == T::Bottom) {
        type->height = T::Bottom;
        return;
    }

    type->height = T::Constant;

    type->type = r_type->type;
    type->constant = l_type->constant - r_type->constant;

}

void AddN::SCCPType() {

    typedef SCCPOptimizer::T T;
    auto lhs = uses[1];
    auto rhs = uses[2];

    const T *l_type = lhs->Payload<T>();
    const T *r_type = rhs->Payload<T>();

    auto type = Payload<T>();

    if(l_type->height == T::Top || r_type->height == T::Top) {
        type->height = T::Top;
        return;
    }

    if(r_type->height == T::Bottom || l_type->height == T::Bottom) {
        type->height = T::Bottom;
        return;
    }

    type->height = T::Constant;

    type->type = r_type->type;
    type->constant = l_type->constant + r_type->constant;
}

void MulN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto lhs = uses[1];
    auto rhs = uses[2];

    const T *l_type = lhs->Payload<T>();
    const T *r_type = rhs->Payload<T>();

    auto type = Payload<T>();

    if(l_type->height == T::Top || r_type->height == T::Top) {
        type->height = T::Top;
        return;
    }

    if(r_type->height == T::Constant) {
        if(r_type->type == T::Value && r_type->constant == 0) {
            type->type = T::Value;
            type->constant = 0;
            return;
        }
    }

    if(l_type->height == T::Constant) {
        if(l_type->type == T::Value && l_type->constant == 0) {
            type->type = T::Value;
            type->constant = 0;
            return;
        }
    }


    if(r_type->height == T::Bottom || l_type->height == T::Bottom) {
        type->height = T::Bottom;
        return;
    }

    type->height = T::Constant;

    type->type = r_type->type;
    type->constant = l_type->constant * r_type->constant;

}

void DivN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto lhs = uses[1];
    auto rhs = uses[2];

    const T *l_type = lhs->Payload<T>();
    const T *r_type = rhs->Payload<T>();

    auto type = Payload<T>();

    if(l_type->height == T::Top || r_type->height == T::Top) {
        type->height = T::Top;
        return;
    }

    if(l_type->height == T::Constant) {
        if(l_type->type == T::Value && l_type->constant == 0) {
            type->type = T::Value;
            type->constant = 0;
            return;
        }
    }


    if(r_type->height == T::Bottom || l_type->height == T::Bottom) {
        type->height = T::Bottom;
        return;
    }

    type->height = T::Constant;

    type->type = r_type->type;
    type->constant = l_type->constant * r_type->constant;



    type->height = T::Constant;

    type->type = r_type->type;
    type->constant = l_type->constant / r_type->constant;
}

}

