//
// Created by baomingkun on 18-12-5.
//

#include "Node.h"
#include "ProjN.h"
#include "SCCP.h"
#include "ControlN.h"

namespace C0 {

string nopToStr(Nop op) {
    switch (op) {
        case Nop::Region:
            return "Region";
        case Nop::Stop:
            return "Stop";
        case Nop::If:
            return "If";
        case Nop::IfProj:
            return "IfProj";
        case Nop::Add:
            return "Add";
        case Nop::Sub:
            return "Sub";
        case Nop::Mul:
            return "Mul";
        case Nop::Div:
            return "Div";
        case Nop::Call:
            return "Call";
        case Nop::Eq:
            return "Eq";
        case Nop::Ne:
            return "Ne";
        case Nop::Lt:
            return "Lt";
        case Nop::Le:
            return "Le";
        case Nop::Gt:
            return "Gt";
        case Nop::Ge:
            return "Ge";
        case Nop::PrintInt:
            return "PrintInt";
        case Nop::PrintChar:
            return "PrintChar";
        case Nop::ReadChar:
            return "ReadChar";
        case Nop::ReadInt:
            return "ReadInt";
        case Nop::GlobalAddr:
            return "GlobalAddr";
        case Nop::StackSlot:
            return "StackSlot";
        case Nop::Int:
            return "Int";
        case Nop::Char:
            return "Char";
        case Nop::GetInt:
            return "GetInt";
        case Nop::GetChar:
            return "GetChar";
        case Nop::SetInt:
            return "SetInt";
        case Nop::SetChar:
            return "SetChar";
        case Nop::ProjRet:
            return "ProjRet";
        case Nop::ProjArg:
            return "ProjArg";
        case Nop::ProjWorld:
            return "ProjWorld";
        case Nop::ProjGlobal:
            return "ProjGlobal";
        case Nop::InitWorld:
            return "InitWorld";
        case Nop::InitGlobal:
            return "InitGlobal";
        case Nop::Phi:
            return "Phi";
        case Nop::Undef:
            return "Undef";
    }
}

UseE Node::SCCPIdentity(Sea &sea, ProjN *projection) {
    typedef SCCPOptimizer::T T;
    auto t = projection->Payload<T>();
    if (t->height == T::Top) {
        return nullptr;
    } else {
        return projection;
    }
}

UseE Node::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto t = this->Payload<T>();
    if (t->height == T::Top) {
        return nullptr;
    } else {
        return this;
    }
}

string Node::exprAsUse() {
    if(serial == -1) {
        string base = nopToStr(op);
        auto sz = this->size();
        auto region = (RegionN *)(this->at(0));
        base += fmt::format(" BB{}", region->bid);
        // for(size_t i = 1; i < sz; ++i) {
        // base += " ";
        // base += this->at(i)->exprAsUse();
        // }
        return "(" + base + ")";
    } else {
        return "v" + std::to_string(serial);
    }
}
}
