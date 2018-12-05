//
// Created by baomingkun on 18-12-5.
//

#include "DefUseBuilder.h"
#include <stack>

using std::stack;

namespace C0 {


void DefUseBuilder::visit(UseE node) {
    switch (phrase) {
        case 0:
            node->clearUse();
            break;
        case 1:
            countUse(node);
            break;
        case 2:
            sumUse(node);
            break;
        default:
            break;
    }
}

void DefUseBuilder::countUse(UseE node) {
    for(auto use : *node) {
        use->addUse();
    }
}

void DefUseBuilder::sumUse(UseE node) {
    total_sum += node->getNumUse();
    pos_map.insert(pair(node, node->getNumUse()));
}

void DefUseBuilder::setUse(UseE stop) {

    visited_set.clear();

    stack<UseE> s;
    s.push(stop);

    while(!s.empty()) {
        auto node = s.top(); s.pop();

        if(visited_set.count(node)) { // already set iterator
            for(auto use: *node) {
                use->getUser()[--pos_map[use]] = node;
            }
            continue;
        }

        visited_set.insert(node);

        node->setUseDef(head);
        head += node->getNumUse();
        s.push(node);

        for(auto use: *node) {
            if(visited_set.count(use)) continue;
            s.push(use);
        }


    }

}



vector<UseE> DefUseBuilder::buildUseDef(StopN *stop) {
    pos_map.clear();
    phrase = 0;
    walk(stop);

    phrase = 1;
    walk(stop);

    total_sum = 0;

    phrase = 2;
    walk(stop);
    vector<UseE> ret(total_sum);
    std::swap(container, ret);
    head = container.begin();

    setUse(stop);

    return ret;
}



}
