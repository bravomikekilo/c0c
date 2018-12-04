//
// Created by baomingkun on 18-11-29.
//

#include "SSADrawer.h"
#include <queue>
#include <stack>
#define FMT_HEADER_ONLY
#include "fmt/format.h"

using std::queue;
using std::stack;
using std::tuple;

namespace C0 {

void SSADrawer::addNode(C0::Region *region, std::string node_str) {





}

void SSADrawer::addRegion(C0::Region *region) {

}

void SSADrawer::visit(End *end) {
    unordered_set<ValueE> visited_value;
    stack<ValueE> value_s;

    auto ret = end->getRet();
    auto world = end->getRet();
    if(ret != nullptr) value_s.push(ret);
    if(world != nullptr) value_s.push(world);

    // map all value and control node with id
    while(!value_s.empty()) {
        auto head = value_s.top(); value_s.pop();


        if(node_id_map.count(head) != 0) continue;
        node_id_map.insert(pair(head, popNodeID()));

        auto n_vin = head->num_vin();
        for(size_t i = 0; i < n_vin; ++i) {
            auto in = head->vin(i);
            if(in == nullptr || node_id_map.count(in)) continue;
            value_s.push(in);
        }
    }

    stack<ControlE> control_s;
    auto n_cin = end->num_cin();
    for(size_t i = 0; i < n_cin; ++i) {
        control_s.push(end->cin(i));
    }

    while(!control_s.empty()) {
        auto head = control_s.top(); value_s.top();


        if(node_id_map.count(head) != 0) continue;
        auto node_id = popNodeID();
        node_id_map.insert(pair(head, node_id));

        if(head->getOp() == Cop::Region) {
            region_id.insert(node_id);
        }

        auto n_cin = head->num_cin();
        for(size_t i = 0; i < n_cin; ++i) {
            auto in = head->cin(i);
            if(in == nullptr || node_id_map.count(in)) continue;
            control_s.push(in);
        }
    }


    if(ret != nullptr) value_s.push(ret);
    if(world != nullptr) value_s.push(world);

    while(!value_s.empty()) {
        auto head = value_s.top(); value_s.pop();
        if(visited_value.count(head)) continue;
        visited_value.insert(head);
        auto head_region = head->control();
        if(!regions.count(head_region)) {
            regions.insert(pair(head_region, vector<pair<int, string>>()));
            region_id_map.insert(pair(head_region, region_id_map.size()));
        }
        auto value_id = node_id_map[head];
        regions[head->control()].push_back(pair(value_id, head->str()));

        auto n_vin = head->num_vin();
        for(size_t i = 0; i < n_vin; ++i) {
            auto in = head->vin(i);
            if(in == nullptr) continue;
            value_edges.emplace_back(tuple(value_id, node_id_map[in], fmt::format("op{}", i + 1)));
            if(visited_value.count(in)) continue;
            value_s.push(in);
        }
    }

    while(!value_s.empty()) {
        auto head = value_s.top(); value_s.pop();
        if(visited_value.count(head)) continue;
        visited_value.insert(head);
        auto head_region = head->control();
        if(!regions.count(head_region)) {
            regions.insert(pair(head_region, vector<pair<int, string>>()));
            region_id_map.insert(pair(head_region, region_id_map.size()));
        }
        auto value_id = node_id_map[head];
        regions[head->control()].push_back(pair(value_id, head->str()));

        auto n_vin = head->num_vin();
        for(size_t i = 0; i < n_vin; ++i) {
            auto in = head->vin(i);
            if(in == nullptr) continue;
            value_edges.emplace_back(tuple(value_id, node_id_map[in], fmt::format("op{}", i + 1)));
            if(visited_value.count(in)) continue;
            value_s.push(in);
        }
    }

    n_cin = end->num_cin();
    for(size_t i = 0; i < n_cin; ++i) {
        control_s.push(end->cin(i));
    }

    while(!control_s.empty()) {
        auto head = control_s.top(); value_s.top();

        



    }

}


}

