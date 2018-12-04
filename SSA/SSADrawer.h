//
// Created by baomingkun on 18-11-29.
//

#ifndef C0_SSADRAWER_H
#define C0_SSADRAWER_H

#include "common.h"
#include "SSA.h"
#include <tuple>

namespace C0 {

class SSADrawer {
private:

    unordered_map<Region *, vector<pair<int, string>>> regions;
    unordered_set<int> region_id;
    unordered_map<Region *, int> region_id_map;

    unordered_map<NodeBase *, int> node_id_map;

    vector<std::tuple<int, int, string>> value_edges;
    vector<std::tuple<int, int, string>> region_edges;

    vector<std::tuple<int, int, string>> control_edges;

    void addRegion(Region *region);

    int next_node_id;

    int popNodeID() {
        return next_node_id++;
    }
    
public:
    void addNode(Region *region, string node_str);

    void visit(End *end);

    string genDot();


};

}


#endif //C0_SSADRAWER_H
