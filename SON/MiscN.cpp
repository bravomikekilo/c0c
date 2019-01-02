//
// Created by baomingkun on 18-12-12.
//

#include "MiscN.h"
#include "SCCP.h"
#include "ConstN.h"
#include "Sea.h"

namespace C0 {



void ProjArgN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    type->height = T::Bottom;
}

string ProjArgN::asText() {
    return Node::asText() + ":" + std::to_string(n);
}


void UndefN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    type->height = T::Bottom;
}

bool UndefN::asCode() {
    return false;
}

string UndefN::exprAsUse() {
    return "undef";
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

UseE PhiN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    if(type->height == T::Top) {
        return nullptr;
    } else if(type->height == T::Bottom) {

        auto sz = size();
        size_t null_num = 0;
        for(auto i = 0; i < sz; ++i) {
            auto use = uses[i];
            if(use == nullptr || use->Payload<T>()->height == T::Top) {
                ++null_num;
            }
        }

        if(null_num != 0) {
            auto new_arr = new UseE[sz - null_num];

            for (auto i = 0, j = 0; i < sz; ++i) {
                auto use = uses[i];
                if (use == nullptr || use->Payload<T>()->height == T::Top) {
                    continue;
                }
                new_arr[j] = use;
                ++j;
            }

            delete[] uses;
            uses = new_arr;
            num_uses = sz - null_num;
        }

        return this;
    }

    if(type->type == T::Value) {
        return sea.alloc<ConstIntN>(uses[0], type->constant);
    } else if(type->type == T::Pointer) {
        return sea.alloc<StackSlotN>(uses[0], type->constant);
    } else {
        return sea.alloc<GlobalAddrN>(uses[0], type->constant, type->label.value());
    }

}

bool PhiN::needReg() {
    auto use_op = uses[1]->getOp();
    switch (use_op) {
        case Nop::Undef:
        case Nop::SetChar:
        case Nop::SetInt:
        case Nop::ProjWorld:
        case Nop::InitWorld:
        case Nop::ProjGlobal:
        case Nop::PrintInt:
        case Nop::PrintChar:
            return false;
        case Nop::Phi:
            return uses[1]->needReg();
        default:
            return true;
    }
}

void InitGlobalN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    type->height = T::Bottom;
}

string InitGlobalN::asText() {
    return Node::asText() + ":" + name;
}

}

