//
// Created by baomingkun on 18-12-5.
//

#include "PhiClean.h"

namespace C0 {


void PhiCleaner::visit(UseE node) {
    if (node->getOp() == Nop::Phi) {
        auto n = (PhiN *)node;
        if (trivial(n)) {
            worklist.push(n);
        }
    }
}

bool PhiCleaner::trivial(PhiN *phi) {
    UseE same = nullptr;

    for (size_t i = 1; i < phi->size(); ++i) {
        auto oprand = phi->at(i);
        if (oprand == same || oprand == phi) {
            continue;
        }

        if (same != nullptr) {
            return false;
        }

        same = oprand;
    }

    return true;
}

void PhiCleaner::optimize(StopN *stop) {
    walk(stop);

    while (!worklist.empty()) {
        auto phi = worklist.top();
        worklist.pop();

        UseE new_node = nullptr;
        if (phi->size() == 1) {
            new_node = sea.alloc<UndefN>();
        } else {
            new_node = phi->at(1);
        }

        for (auto user: phi->getUser()) {
            if (user != phi) {
                user->replace(phi, new_node);
            }
        }

        for (auto user: phi->getUser()) {
            if(user->getOp() == Nop::Phi) {
                auto p = (PhiN *)user;
                if(trivial(p)) {
                    worklist.push(p);
                }
            }
        }

    }
}

}
