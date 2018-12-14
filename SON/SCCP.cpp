//
// Created by baomingkun on 18-12-6.
//


#include "SCCP.h"
#include <stack>
#include "Payload.h"
using std::stack;

namespace C0 {

void SCCPOptimizer::initialize(StopN *stop) {

    unordered_set<UseE> visited;

    stack<UseE> s;
    s.push(stop);

    // initialize all node with Top
    while(!s.empty()) {
        auto node = s.top(); s.pop();

        if(visited.count(node)) continue;

        visited.insert(node);
        auto load = new T{T::Top, T::Value, 0};
        payloads.push_back(load);
        node->setPayload(load);

        for(auto use: *node) {
            if(visited.count(use) == 0) {
                s.push(use);
            }
        }

    }

}


void SCCPOptimizer::freePayloads() {
    for(auto *load: payloads) {
        delete load;
    }
    payloads.clear();
}

void SCCPOptimizer::opt(pair<RegionN*, StopN *> graph) {
    auto start = graph.first;
    auto stop = graph.second;

    initialize(stop);

    // compute all lattice

    stack<UseE> worklist;
    worklist.push(start);

    while(!worklist.empty()) {
        auto head = worklist.top(); worklist.pop();
        auto old = *head->Payload<T>();
        head->SCCPType();
        if(old != *head->Payload<T>()) {
            for(auto user: head->getUser()) {
                worklist.push(user);
            }
        }

    }


    freePayloads();
}

}
