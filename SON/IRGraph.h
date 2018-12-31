//
// Created by baomingkun on 18-12-30.
//

#ifndef C0_IRGRAPH_H
#define C0_IRGRAPH_H

#include "common.h"
#include "Sea.h"
#include "SON.h"
#include <functional>

namespace C0 {

class IRGraph {
private:
    Sea ocean;
    RegionN *head;
    StopN *tail;
    vector<RegionN *> postOrder;

public:

    IRGraph(RegionN *head, StopN *tail)
        :head(head), tail(tail) {}

    RegionN *getHead() {
        return head;
    }

    StopN *getTail() {
        return tail;
    }

    pair<RegionN *, StopN *> getPair() {
        return pair(head, tail);
    }

    // find the post order of nodes
    void buildPostOrder();

    void buildIndex();

    // build dominance tree
    void buildDominance();

    // build loop info
    void buildLoopInfo();

    // perform liveness analysis
    void liveAnalysis();

    // build live interval
    void buildLiveInterval();

    // preorder walk of dominace tree, must build dominance first
    void dominancePreOrderWalk(std::function<void(RegionN *)> func);

    // post order walk of CFG
    void postOrderWalk(std::function<void(RegionN *)> func);

    // reverse post order walk of CFG
    void reversePostOrderWalk(std::function<void(RegionN *)> func);

    string irDump();


};

}


#endif //C0_IRGRAPH_H
