//
// Created by baomingkun on 18-12-5.
//

#include "BackDFSWalker.h"
#include <stack>

using std::stack;

namespace C0 {

void BackDFSWalker::walk(UseE stop) {
    visited_set.clear();

    stack<UseE> s;
    s.push(stop);

    while(!s.empty()) {
        auto head = s.top(); s.pop();
        if(visited_set.count(head)) continue;

        visited_set.insert(head);

        for(auto use: *head) {
            if(visited_set.count(use)) continue;
            s.push(use);
        }

        visit(head);

    }
}

}