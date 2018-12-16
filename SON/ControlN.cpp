//
// Created by baomingkun on 18-12-11.
//


#include "ControlN.h"
#include "SCCP.h"
#include "ProjN.h"

#include <stack>
using std::list;


namespace C0 {


void RegionN::Linearization() {
    vector<UseE> phis;
    std::stack<UseE> s;
    unordered_set<UseE> visited;
    visited.insert(this);

    for (auto user: getUser()) {
        auto user_op = user->getOp();
        if (user_op == Nop::Region || user_op == Nop::If) { continue; }

        bool is_head = true;
        for (auto user_user: user->getUser()) {
            if (user_user->getOp() != Nop::Phi && user_user->front() == this) {
                is_head = false;
                break;
            }
        }

        if (is_head) {
            s.push(user);
            visited.insert(user);
        }
    }


    while (!s.empty()) {
        auto head = s.top();
        s.pop();
        if (head->getOp() == Nop::Phi) {
            phis.push_back(head);
        } else {
            this->_linear.push_front(head);
        }

        for(auto use : *head) {
            if(!visited.count(use) && use->front() == this) {
                s.push(use);
            }
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

    if (this->size()) {
        type->height = T::Top;
    } else {
        type->height = T::Bottom;
    }
}

UseE RegionN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    if (type->height == T::Top) {
        return nullptr;
    } else {

        auto sz = size();
        size_t null_num = 0;
        for (auto i = 0; i < sz; ++i) {
            auto use = uses[i];
            if (use == nullptr || use->Payload<T>()->height == T::Top) {
                ++null_num;
            }
        }

        if (null_num != 0) {
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

    if (src_type->height == T::Top || pre_type->height == T::Top) {
        type->height = T::Top;
        return;
    }

    *type = *pre_type;

}

void IfN::SCCPType(ProjN *n) {
    typedef SCCPOptimizer::T T;
    auto type = Payload<T>();
    auto n_type = n->Payload<T>();

    if (type->height != T::Constant) {
        n_type->height = type->height;
        return;
    }

    if (type->constant == n->field) {
        n_type->height = T::Constant;
        n_type->type = T::Value;
        n_type->constant = 1;
    } else {
        n_type->height = T::Top;
    }

}

UseE IfN::SCCPIdentity(Sea &sea) {
    typedef SCCPOptimizer::T T;
    auto t = Payload<T>();
    return Node::SCCPIdentity(sea);
}

UseE IfN::SCCPIdentity(Sea &sea, ProjN *projection) {
    typedef SCCPOptimizer::T T;
    auto type_t = projection->Payload<T>();
    if(type_t->height == T::Top) {
        return nullptr;
    } else if(type_t->height == T::Bottom){
        return projection;
    } else {
        return uses[0];
    }
    // return Node::SCCPIdentity(sea, projection);
}


}
