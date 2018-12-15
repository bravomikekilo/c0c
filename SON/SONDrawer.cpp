//
// Created by baomingkun on 18-12-4.
//

#include "SONDrawer.h"
#include <sstream>

#define FMT_HEADER_ONLY

#include "fmt/format.h"
#include <stack>

using std::stack;

namespace C0 {

void SONDrawer::addEdge(UseE src_node, int src, int dst, std::string label) {

    if (subgraphs.count(dst)) {
        auto src_op = src_node->getOp();
        if (src_op == Nop::Region || src_op == Nop::If || src_op == Nop::IfProj || src_op == Nop::Stop) {
            edges.emplace_back(src, dst, label);
            return;
        } else {
            return;
        }
    }

    edges.emplace_back(src, dst, label);
}

void SONDrawer::addNode(UseE src_node, int id, string str) {
    // std::cout << fmt::format("get node {}", id) << std::endl;
    if (src_node->getOp() == Nop::Region) {
        // std::cout << fmt::format("get region {}", id) << std::endl;
        subgraphs[id].insert(pair(id, str));
        // subgraphs.insert(pair(id, init_set));
    } else {
        auto src_op = src_node->getOp();
        if (src_op == Nop::Stop || src_op == Nop::IfProj || src_op == Nop::Undef) { // float node
            floatNode.emplace_back(id, str);
        } else {
            auto region_id = region_ids[src_node->at(0)];
            subgraphs[region_id].insert(pair(id, str));
        }
    }
}

void SONDrawer::draw(StopN *stop) {

    // first get id for every node
    int next_id = 1;
    stack<UseE> s;
    s.push(stop);

    while (!s.empty()) {
        auto head = s.top();
        s.pop();
        if (node_id.count(head)) continue;

        if (head->getOp() == Nop::Region) {
            region_ids.insert(pair(head, next_id));
        }

        node_id.insert(pair(head, next_id++));

        for (auto use: *head) {
            if (node_id.count(use)) continue;
            s.push(use);
        }
    }

    unordered_set<UseE> visited;
    s.push(stop);
    while (!s.empty()) {
        auto head = s.top();
        s.pop();
        if (visited.count(head)) continue;

        addNode(head, node_id[head], head->str());
        visited.insert(head);

        int index = 0;
        for (auto use: *head) {
            addEdge(head, node_id[head], node_id[use], std::to_string(index));
            ++index;
            if (visited.count(use)) continue;
            s.push(use);
        }
    }


}


string SONDrawer::toDot(const string &graph_name) {

    std::stringstream stream;

    string graph_header = fmt::format("digraph {} {{\nnode[shape=box]\nrankdir=\"BT\"\n", graph_name);

    stream << graph_header << std::endl;

    for (auto &subgraph: subgraphs) {
        auto graph_id = subgraph.first;
        auto &graph = subgraph.second;
        stream << fmt::format("subgraph cluster_region_{} {{\ncolor=blue;\n", graph_id);
        for (auto &node: graph) {
            auto node_id = node.first;
            auto &node_str = node.second;
            // print node string
            stream << "\"node" << std::to_string(node_id) << "\"[\n";
            stream << "label=\"" << node_str << "\"\n]" << std::endl;
        }
        stream << "\n}\n";
    }

    // print float node
    for (auto &p: floatNode) {
        auto node_id = p.first;
        auto &node_str = p.second;
        stream << "\"node" << std::to_string(node_id) << "\"[\n";
        stream << "label=\"" << node_str << "\"\n]" << std::endl;
    }

    // print all edge
    for (auto &edge: edges) {
        stream << "\"node" << std::to_string(std::get<0>(edge)) << "\" -> ";
        stream << "\"node" << std::to_string(std::get<1>(edge)) << "\"[\n";
        stream << "label=\"" << std::get<2>(edge) << "\"\n];" << std::endl;
    }

    stream << "}\n\n";

    return stream.str();
}


}
