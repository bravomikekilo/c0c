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

    s.push(pair(head, false));
    visited.insert(head);
    size_t index = 0;
    while (!s.empty()) {
        auto head = s.top();
        s.pop();
        if (head.second) {
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

RegionN *interSection(RegionN *b1, RegionN *b2, const vector<RegionN *> &dom);


vector<RegionN *> IRGraph::buildDominance() {
    auto start = head;
    auto stop = tail;

    // first label all region, and record post order
    size_t index = postOrder.size();
    // stack<pair<RegionN *, bool>> s;
    unordered_set<RegionN *> visited;
    // vector<RegionN *> postOrder;

    /*
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
    */



    vector<RegionN *> ret(index, nullptr);

    ret[start->bid] = start;
    // std::cout << "start->bid " << start->bid << std::endl;
    bool changed = true;

    while (changed) {
        changed = false;
        // in reverse post-order (except start_node)
        for (int sz = postOrder.size() - 2; sz >= 0; --sz) {
            auto region = postOrder[sz];
            // std::cout << "visit" << region->bid << std::endl;

            RegionN *new_idom = nullptr;

            bool first = true;
            region->visitPred([&](RegionN *pred) {
                /*
                std::cout << "BB" << region->bid << " pred BB" << pred->bid << std::endl;
                std::cout << "pred idom "
                          << (ret[pred->bid] != nullptr ? std::to_string(ret[pred->bid]->bid) : "nullptr") << std::endl;
                */
                if (first) {
                    if (ret[pred->bid] != nullptr) {
                        first = false;
                        new_idom = pred;
                    }
                } else {
                    if (ret[pred->bid] != nullptr) {
                        new_idom = interSection(pred, new_idom, ret);
                        // if (!new_idom) std::cout << "wrong in new_idom" << std::endl;
                    }
                }
            });

            if (ret[region->bid] != new_idom) {
                ret[region->bid] = new_idom;
                changed = true;
            }
        }
    }

    return ret;

}

void IRGraph::initLiveness() {
    for (auto region: postOrder) {
        region->initLiveness();
    }
}


void IRGraph::liveAnalysis() {
    // init all use def and live in and live out

    for (auto region: postOrder) {
        region->initLiveness();
    }

    std::cout << "$$##" << " init liveness" << std::endl;
    std::cout << irDump(false) << std::endl;


    std::queue<RegionN *> worklist;
    for (auto iter = postOrder.begin(); iter != postOrder.end(); iter++) {
        std::set<UseE> new_live_out;
        auto h = *iter;
        std::cout << "compute live out for block " << h->bid << std::endl;
        h->visitPost([&](auto post) {
            // new_live_out.insert(post->liveIn->begin(), post->liveOut->end());
            for (auto use: *post->liveIn) {
                new_live_out.insert(use);
            }
        });

        auto livein = new_live_out;
        h->liveOut = make_unique<std::set<UseE>>(std::move(new_live_out));

        livein.insert(h->useSet->begin(), h->useSet->end());

        for (auto def: *h->defSet) {
            livein.erase(def);
        }

        if (livein != *h->liveIn) {
            worklist.push(h);
            h->liveIn = make_unique<std::set<UseE>>(std::move(livein));
        }
    }

    std::cout << "finish first round liveness analysis" << std::endl;
    std::cout << "$$## first round liveness analysis" << std::endl;
    std::cout << irDump(false) << std::endl;


    // auto last_block = (RegionN *) tail->at(0);
    // worklist.push(last_block);

    while (!worklist.empty()) {
        auto h = worklist.front();
        worklist.pop();
        std::set<UseE> new_live_out;
        h->visitPost([&](auto post) {
            new_live_out.insert(post->liveIn->begin(), post->liveIn->end());
        });

        std::cout << "compute live out of BB" << h->bid << std::endl;
        for (auto node: new_live_out) {
            std::cout << node->exprAsUse() << " ";
        }
        std::cout << std::endl;


        if (new_live_out != *h->liveOut) {
            std::cout << "update live out of BB" << h->bid << std::endl;
            // update live out and live in
            auto livein = new_live_out;
            livein.insert(h->useSet->begin(), h->useSet->end());
            // livein.erase(h->defSet->begin(), h->defSet->end());
            for (auto def : *h->defSet) {
                livein.erase(def);
            }

            h->liveIn = make_unique<std::set<UseE>>(std::move(livein));
            h->liveOut = make_unique<std::set<UseE>>(std::move(new_live_out));
            // add all pred to worklist
            h->visitPred([&](auto pred) {
                worklist.push(pred);
            });
        }

    }

}

string IRGraph::irDump(bool draw_payload) {
    std::stringstream stream;

    for (auto iter = postOrder.rbegin(); iter != postOrder.rend(); ++iter) {
        auto region = *iter;
        stream << fmt::format("#BB{} @{} from:", region->bid, region->getPos().toStr());
        region->visitPred([&](auto pred) {
            stream << fmt::format("BB{},", pred->bid);
        });

        if (draw_payload) {
            if (auto p = region->Payload<NLoad>(); p) {
                stream << "   // ";
                stream << p->toStr();
            }
        }

        stream << std::endl;

        // add live in here
        if (region->liveIn != nullptr) {
            stream << "// live in";
            for (auto ir : *region->liveIn) {
                stream << " " << ir->exprAsUse();
            }
            stream << std::endl;
        }

        if (region->useSet != nullptr) {
            stream << "// use set ";
            for (auto ir: *region->useSet) {
                stream << " " << ir->exprAsUse();
            }
            stream << std::endl;
        }

        if (region->defSet != nullptr) {
            stream << "// def set ";
            for (auto ir: *region->defSet) {
                stream << " " << ir->exprAsUse();
            }
            stream << std::endl;
        }

        // iterate over all ir
        for (auto ir: region->getOrder()) {
            // indent
            stream << "    ";
            stream << ir->asText();

            stream << " // ";
            if (draw_payload) {
                if (auto p = ir->Payload<NLoad>(); p) {
                    stream << "Payload:";
                    stream << p->toStr();
                }
            }

            stream << " USER:";
            for (auto user: ir->getUser()) {
                stream << " " << user->exprAsUse();
            }

            stream << std::endl;
        }

        if (region == tail->at(0)) {
            // add last ret
            stream << "    \n";

        }

        // add live out here
        if (region->liveOut != nullptr) {
            stream << "// live out";
            for (auto ir : *region->liveOut) {
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
    for (auto iter = postOrder.rbegin(); iter != postOrder.rend(); ++iter) {
        auto region = *iter;
        region->schedule();

        for (auto ir: region->getOrder()) {
            ir->serial = index;
            ++index;
        }
    }

    tail->serial = index;
}

void IRGraph::cleanControlFlow() {
    for(auto region: postOrder) {
        if(region->getOrder().size() == 0 && region->size() == 1) {
            auto pred = region->at(0);
            for(auto user: region->getUser()) {
                user->replace(region, pred);
                pred->getUser().erase(region);
                pred->getUser().insert(user);
            }
        }
    }


}

// CSE inside basic block
void IRGraph::localCSE() {
    for(auto region: postOrder) {
        unordered_map<Nop, unordered_set<UseE>> hash;
        for(auto node: region->getOrder()) {
            if(hash.count(node->getOp())) {
                auto &set = hash.at(node->getOp());
                bool unique = true;
                for(auto ori: set) {
                    if(ori->same(*node)) {
                        // get a same cse
                        std::cout << "same node " << ori->serial << " and " << node->serial << std::endl;
                        unique = false;

                        for(auto user: node->getUser()) {
                            user->replace(node, ori);
                        }

                        for(auto use: *node) {
                            use->getUser().erase(node);
                        }
                    }
                }

                if(unique) {
                    set.insert(node);
                }

            } else {
                hash.insert(pair(node->getOp(), unordered_set<UseE>()));
            }
        }
    }
}

}
