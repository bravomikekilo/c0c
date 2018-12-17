//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_SONDRAWER_H
#define C0_SONDRAWER_H

#include "Node.h"
#include "SON.h"
#include <tuple>

using std::tuple;

namespace C0 {


class SONDrawer {

private:
    unordered_map<int, unordered_map<int, string>> subgraphs;
    unordered_map<UseE, int> region_ids;
    vector<pair<int, string>> floatNode;

    unordered_map<UseE, int> node_id;

    vector<tuple<int, int, string>> edges;
    vector<pair<int, int>> def_use_edge;

    void addEdge(UseE src_node, int src, int dst, string label);
    void addNode(UseE src_node, int id, string str);


public:

    void draw(StopN *stop, bool with_use = false);

    string toDot(const string &graph_name);

    void clear() {
        subgraphs.clear();
        region_ids.clear();
        floatNode.clear();
        node_id.clear();
        edges.clear();
        def_use_edge.clear();
    }

};


}


#endif //C0_SONDRAWER_H
