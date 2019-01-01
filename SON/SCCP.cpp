//
// Created by baomingkun on 18-12-6.
//


#include "SCCP.h"
#include <stack>
#include "NLoad.h"
#include "SON.h"
#include <iostream>

#define FMT_HEADER_ONLY

#include "fmt/format.h"

#include "SONDrawer.h"
#include <iostream>
#include <fstream>
#include "sonUtil.h"
#include "PhiClean.h"

using std::stack;

namespace C0 {

string SCCPOptimizer::T::toStr() {
    if (height == T::Top) {
        return "Top";
    } else if (height == T::Bottom) {
        return "Bottom";
    } else {
        if (type == T::Pointer) {
            return "P:" + std::to_string(constant);
        } else if (type == T::Label) {
            return "L:" + std::to_string(constant);
        } else {
            return "V:" + std::to_string(constant);
        }
    }
};

void SCCPOptimizer::initialize(StopN *stop) {

    unordered_set<UseE> visited;

    stack<UseE> s;
    s.push(stop);
    visited.insert(stop);

    // initialize all node with Top
    while (!s.empty()) {
        auto node = s.top();
        s.pop();

        auto load = new T(T::Top, T::Value, 0);
        payloads.push_back(node);
        node->setPayload(load);

        for (auto use: *node) {
            if (!visited.count(use)) {
                s.push(use);
                visited.insert(use);
            }
        }

    }

}


void SCCPOptimizer::freePayloads() {
    for (auto node: payloads) {
        node->freePayload();
    }
    payloads.clear();
}

void SCCPOptimizer::analysis(pair<RegionN *, StopN *> graph) {
    auto start = graph.first;
    auto stop = graph.second;

    initialize(stop);

    // compute all lattice

    stack<UseE> worklist;
    worklist.push(start);

    /*
    auto drawer = SONDrawer();
    int index = 0;
    */

    while (!worklist.empty()) {

        auto head = worklist.top();
        worklist.pop();
        auto old = *head->Payload<T>();
        head->SCCPType();

        if (old != *head->Payload<T>()) {

            /*
            ++index;
            drawer.draw(stop);
            std::ofstream stream;
            stream.open(fmt::format("sccp/{}_sccp.dot", index));
            stream << drawer.toDot("test") << std::endl;
            stream.close();
            drawer.clear();
            */

            for (auto user: head->getUser()) {
                worklist.push(user);
            }
        }

    }

}

void SCCPOptimizer::transform(pair<RegionN *, StopN *> graph, Sea &sea) {

    auto start = graph.first;
    auto stop = graph.second;

    unordered_set<UseE> visited;
    stack<UseE> worklist;
    worklist.push(stop);

    visited.insert(stop);

    while (!worklist.empty()) {

        auto head = worklist.top();
        worklist.pop();

        auto id = head->SCCPIdentity(sea);

        /*
        auto &head_user = head->getUser();
        for(auto iter = head_user.begin(); iter != head_user.end();) {
            auto user = *iter;
            auto user_t = user->Payload<T>();
            if(user_t->height != T::Bottom) {
                auto ori = iter++;
                head_user.erase(ori);
            } else {
                ++iter;
            }
        }
        */

        if (id != head) {
            std::cout << "replace " << head->exprAsUse() << " with ";
            std::cout << (id ? id->exprAsUse() : "nullptr")  << std::endl;

            for(auto user: head->getUser()) {
                auto user_t = user->Payload<T>();
                if(user_t->height != T::Top) {
                    user->replace(head, id);
                }
            }
            if (id != nullptr) {
                for (auto use : *id) {
                    if (!visited.count(use)) {
                        visited.insert(use);
                        worklist.push(use);
                    }
                }
            }
        } else {
            for (auto use : *head) {
                if (!visited.count(use)) {
                    visited.insert(use);
                    worklist.push(use);
                }
            }
        }
    }

    cleanDefUse(stop);
    buildDefUse(stop);

    PhiCleaner cleaner(sea);
    cleaner.optimize(stop);
    mergeLinearRegion(stop);
}
}
