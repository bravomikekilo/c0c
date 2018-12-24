//
// Created by baomingkun on 18-12-24.
//

#ifndef C0_FINEDRAWER_H
#define C0_FINEDRAWER_H

#include "common.h"
#include "SON.h"
#include <tuple>

namespace C0 {

class FineDrawer {

public:

    void draw(StopN *end);

    string toDot(const string &graph_name) const;

private:

    vector<string> node_str;
    vector<std::tuple<int, int, string>> edges;

    string drawNode(UseE node);

    unordered_set<UseE> visited;
};

}


#endif //C0_FINEDRAWER_H
