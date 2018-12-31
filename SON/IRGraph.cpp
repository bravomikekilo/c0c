//
// Created by baomingkun on 18-12-30.
//

#include "IRGraph.h"
#include <stack>
#include <queue>
#include <set>
#include <sstream>
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

using std::stack;


void IRGraph::buildPostOrder() {
    stack<pair<RegionN *, bool>> s;
    unordered_set<RegionN *> visited;
    postOrder.clear();

    s.push(pair(head, false)); visited.insert(head);
    size_t index = 0;
    while(!s.empty()) {
        auto head = s.top(); s.pop();
        if(head.second) {
            head.first->bid = index++;
            postOrder.push_back(head.first);
        } else {
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
}



void buildDominance() {






}

void IRGraph::liveAnalysis() {
    // init all use def and live in and live out

    for(auto region: postOrder) {
        region->initLiveness();
    }

    std::queue<RegionN *> worklist;
    auto last_block = (RegionN *)tail->at(0);
    worklist.push(last_block);

    while(!worklist.empty()) {
        auto h = worklist.front(); worklist.pop();
        std::set<UseE> new_live_out;
        h->visitPost([&](auto post) {
            new_live_out.insert(post->liveIn->begin(), post->liveIn->end());
        });

        if(new_live_out == *h->liveOut) {
            // update live out and live in
            auto livein = new_live_out;
            livein.insert(h->useSet->begin(), h->useSet->end());
            livein.erase(h->defSet->begin(), h->defSet->end());
            h->liveIn = make_unique<std::set<UseE>>(std::move(livein));
            h->liveOut = make_unique<std::set<UseE>>(std::move(new_live_out));
            // add all pred to worklist
            h->visitPred([&](auto pred) {
               worklist.push(pred);
            });
        }

    }

}

string IRGraph::irDump() {
    std::stringstream stream;

    for(auto iter = postOrder.rbegin(); iter != postOrder.rend(); ++iter) {
        auto region = *iter;
        stream << fmt::format("#BB{} from:", region->bid);
        region->visitPred([&](auto pred) {
            stream << fmt::format("BB{},", pred->bid);
        });
        stream << std::endl;

        // add live in here
        if(region->liveIn != nullptr) {
            stream << "// live in";
            for(auto ir : *region->liveIn) {
                stream << " " << ir->exprAsUse();
            }
            stream << std::endl;
        }

        // iterate over all ir
        for(auto ir: region->getOrder()) {
            // indent
            stream << "    ";
            stream << ir->asText();
            stream << std::endl;
        }

        if(region == tail->at(0)) {
            // add last ret
            stream << "    \n";

        }

        // add live out here
        if(region->liveOut != nullptr) {
            stream << "// live in";
            for(auto ir : *region->liveOut) {
                stream << " " << ir->exprAsUse();
            }
            stream << std::endl;
        }

        stream << std::endl;


    }


    return stream.str();

}

// label every ir node with a index
void IRGraph::buildIndex() {

    int index = 0;
    for(auto iter = postOrder.rbegin(); iter != postOrder.rend(); ++iter) {
        auto region = *iter;
        region->schedule();

        for(auto ir: region->getOrder()) {
            ir->serial = index;
            ++index;
        }
    }

    tail->serial = index;
}


}
