//
// Created by baomingkun on 18-12-5.
//

#include "PhiClean.h"

namespace C0 {


void PhiCleaner::visit(UseE node) {
    if (node->getOp() == Nop::Phi) {
        auto n = (PhiN *) node;
        auto is_trivial = trivial(n);
        if (is_trivial) {
            worklist.push(pair(n, is_trivial.value()));
        }
    }
}

optional<UseE> PhiCleaner::trivial(PhiN *phi) {
    UseE same = nullptr;

    for (size_t i = 1; i < phi->size(); ++i) {
        auto oprand = phi->at(i);
        if (oprand == same || oprand == phi) {
            continue;
        }

        if (same != nullptr) {
            return {};
        }

        same = oprand;
    }

    return {same};
}

void PhiCleaner::optimize(StopN *stop) {
    walk(stop);

    while (!worklist.empty()) {
        auto item = worklist.top();
        worklist.pop();

        auto phi = item.first;

        UseE new_node = nullptr;
        if (item.second) {
            new_node = item.second;
        } else {
            new_node = undef;
        }

        std::cerr << "replace node " << phi->exprAsUse() << " with " << new_node->exprAsUse() << std::endl;

        for(auto use: *phi) {
            use->getUser().erase(phi);
        }

        for (auto user: phi->getUser()) {
            if (user != phi) {
                std::cerr << "replace all " << phi->exprAsUse() << " in " << user->exprAsUse() << " with " << new_node->exprAsUse() << std::endl;
                user->replace(phi, new_node);

                new_node->addUser(user);

                if (user->getOp() == Nop::Phi) {
                    auto p = (PhiN *) user;
                    auto is_trivial = trivial(p);
                    if (is_trivial) {
                        worklist.push(pair(p, is_trivial.value()));
                    }

                }
            }
        }


    }
}

}
