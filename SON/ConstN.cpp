//
// Created by baomingkun on 18-12-6.
//


#include "ConstN.h"
#include "SCCP.h"

namespace C0 {


void ConstIntN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();
    if (uses[0]->Payload<T>()->height == T::Bottom) {
        type->height = T::Constant;
        type->type = T::Value;
        type->constant = v;
    } else {
        type->height = T::Top;
    }
}

string ConstIntN::exprAsUse() {
    return Node::exprAsUse() + ":" + std::to_string(v);
}

bool ConstIntN::asCode() {
    return false;
}

bool ConstIntN::same(const Node &other) {
    if(!Node::same(other)) return false;
    const auto &o = (const ConstIntN &)(other);
    return o.v == v;
}


void ConstCharN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();
    if (uses[0]->Payload<T>()->height == T::Bottom) {
        type->height = T::Constant;
        type->type = T::Value;
        type->constant = v;
    } else {
        type->height = T::Top;
    }
}

bool ConstCharN::asCode() {
    return false;
}

string ConstCharN::exprAsUse() {
    return fmt::format("'{}'", v);
}

bool ConstCharN::same(const Node &other) {
    if(!Node::same(other)) return false;
    const auto &o = (const ConstCharN &)(other);
    return o.v == v;
}


void GlobalAddrN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();

    if (uses[0]->Payload<T>()->height == T::Bottom) {
        type->height = T::Constant;
        type->type = T::Label;
        type->label = {label};
        type->constant = offset;
    } else {
        type->height = T::Top;
    }
}

UseE GlobalAddrN::sameLabel(int offset, Sea &sea) {
    if(offset == this->offset) {
        return this;
    } else {
        return sea.alloc<GlobalAddrN>(uses[0], offset, label);
    }
}

bool GlobalAddrN::asCode() {
    return false;
}

string GlobalAddrN::exprAsUse() {
    return fmt::format("{}({})", label, offset);
}

bool GlobalAddrN::same(const Node &other) {
    if(!Node::same(other)) return false;
    const auto &o = (const GlobalAddrN &)(other);
    return o.offset == offset;
}


void StackSlotN::SCCPType() {
    typedef SCCPOptimizer::T T;
    auto type = this->Payload<T>();

    if (uses[0]->Payload<T>()->height == T::Bottom) {
        type->height = T::Constant;
        type->type = T::Pointer;
        type->constant = offset;
    } else {
        type->height = T::Top;
    }
}

bool StackSlotN::asCode() {
    return false;
}

string StackSlotN::exprAsUse() {
    return fmt::format("$sp({})", offset);
}

bool StackSlotN::same(const Node &other) {
    if(!Node::same(other)) return false;
    const auto &o = (const StackSlotN &)(other);
    return o.offset == offset;
}

UseE ConstN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    if(uses[0]->Payload<T>()->height == T::Top) {
        return nullptr;
    } else {
        return this;
    }
}

string ConstN::asText() {
    return Node::asText() + " " + exprAsUse();
}

bool ConstN::needReg() {
    return false;
}
}
