//
// Created by baomingkun on 18-12-6.
//


#include "sonUtil.h"
#include <stack>

using std::stack;

namespace C0 {

void buildDefUse(StopN *stop) {

    unordered_set<UseE> visited;

    stack<UseE> s;
    s.push(stop);

    while (!s.empty()) {
        auto node = s.top();
        s.pop();

        if (visited.count(node)) {
            for (auto use: *node) {
                use->addUse(node);
            }
            continue;
        }

        visited.insert(node);

        node->initDefUse();
        s.push(node);

        for (auto use: *node) {
            if (visited.count(use)) continue;
            s.push(use);
        }

    }

}

void cleanDefUse(StopN *stop) {

    unordered_set<UseE> visited;

    stack<UseE> s;
    s.push(stop);

    while (!s.empty()) {
        auto node = s.top();
        s.pop();

        if (visited.count(node)) continue;

        visited.insert(node);

        node->clearDefUse();

        for (auto use: *node) {
            if (visited.count(use) || use == nullptr) continue;
            s.push(use);
        }
    }
}


void mergeLinearRegion(StopN *stop) {

    unordered_set<UseE> visited;

    stack<UseE> s;
    s.push(stop);

    while (!s.empty()) {
        auto node = s.top();
        s.pop();

        if (visited.count(node)) continue;

        visited.insert(node);

        if (node->getOp() == Nop::Region) {
            for (auto use: *node) {
                if (!visited.count(use)) {
                    s.push(use);
                }
            }

            if (node->size() == 1) {
                auto pred = node->at(0);
                if (pred->getOp() == Nop::Region) {
                    auto pred_user = pred->getUser();
                    auto region_cnt = 0;
                    for (auto user : pred_user) {
                        auto user_op = user->getOp();
                        if (user_op == Nop::Region) {
                            ++region_cnt;
                        }
                    }

                    if (region_cnt == 1) {

                        pred->getUser().erase(node);

                        pred->addUse(node->getUser());

                        for (auto user : node->getUser()) {
                            user->replace(node, pred);
                        }
                    }


                }
            }

        } else {
            if (!visited.count(node->at(0))) s.push(node->at(0));
        }
    }

}

unordered_map<VarID, int> getGlobalOffset(const vector<SymTerm> &terms) {
    unordered_map<VarID, int> ret;
    int offset = 0;
    for (auto term : terms) {
        if (term.isConst()) {
            continue;
        }
        auto sz = term.type.sizeOf();
        if (sz % 4 != 0) {
            sz += 4 - sz % 4;
        }
        ret.insert(pair(term.id, offset));
        offset += sz;
    }
    return ret;
}

}
