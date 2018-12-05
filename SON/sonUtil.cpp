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

    while(!s.empty()) {
        auto node = s.top(); s.pop();

        if(visited.count(node)) {
            for(auto use: *node) {
                use->addUse(node);
            }
            continue;
        }

        visited.insert(node);

        node->initDefUse();
        s.push(node);

        for(auto use: *node) {
            if(visited.count(use)) continue;
            s.push(use);
        }

    }

}

void cleanDefUse(StopN *stop) {

    unordered_set<UseE> visited;

    stack<UseE> s;
    s.push(stop);

    while(!s.empty()) {
        auto node = s.top(); s.pop();

        if(visited.count(node)) continue;

        visited.insert(node);

        node->clearDefUse();

        for(auto use: *node) {
            if(visited.count(use)) continue;
            s.push(use);
        }
    }
}

}
