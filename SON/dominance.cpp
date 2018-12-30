//
// Created by baomingkun on 18-12-17.
//

#include "dominance.h"
#include <stack>

namespace C0 {

using std::stack;

RegionN *interSection(RegionN *b1, RegionN *b2, const vector<RegionN *> &dom) {
    while(b1 != b2) {
        while(b1->bid < b2->bid) {
            b1 = dom[b1->bid];
        }

        while(b2->bid < b1->bid) {
            b2 = dom[b2->bid];
        }
    }

    return b1;
}

vector<RegionN *> computeDominanceTree(pair<RegionN *, StopN *> graph) {
    auto start = graph.first;
    auto stop = graph.second;

    // first label all region, and record post order
    size_t index = 0;
    stack<pair<RegionN *, bool>> s;
    unordered_set<RegionN *> visited;
    vector<RegionN *> postOrder;

    s.push(pair(start, false)); visited.insert(start);

    while(!s.empty()) {
        auto head = s.top(); s.pop();
        if(head.second) {
            head.first->bid = index++;
            postOrder.push_back(head.first);
        } else{
            auto node = head.first;

            s.push(pair(node, true));
            node->visitPost([&](RegionN *region) {
                if (!visited.count(region)) {
                    visited.insert(region);
                    s.push(pair(region, false));
                }
            });

        }
    }


    vector<RegionN *> ret(index, nullptr);

    ret[start->bid] = start;
    bool changed = true;

    while(changed) {
        changed = false;
        // in reverse post-order (except start_node)
        for(auto sz = postOrder.size() - 1; sz > 0; --sz) {
            auto region = postOrder[sz];
            RegionN *new_idom = nullptr;

            bool first = true;
            region->visitPred([&](RegionN *pred){
                if(first && ret[pred->bid] != nullptr) {
                    first = false;
                    new_idom = pred;
                } else {
                    if(ret[pred->bid] != nullptr) {
                        new_idom = interSection(pred, new_idom, ret);
                    }
                }
            });

            if(ret[region->bid] != new_idom) {
                ret[region->bid] = new_idom;
                changed = true;
            }
        }
    }

    return ret;
}

}