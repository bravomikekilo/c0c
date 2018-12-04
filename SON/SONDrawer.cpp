//
// Created by baomingkun on 18-12-4.
//

#include "SONDrawer.h"
#include <sstream>
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

void SONDrawer::addEdge(UseE src_node, int src, int dst, std::string label) {

    if(subgraphs.count(dst)) {
        auto src_op = src_node->getOp();
        if(src_op == Nop::Region || src_op == Nop::If || src_op == Nop::IfProj || src_op == Nop::Stop) {
            edges.emplace_back(src, dst, label);
            return;
        } else {
            return;
        }
    }

    edges.emplace_back(src, dst, label);
}

void SONDrawer::addNode(UseE src_node, int id, string str) {
    if(src_node->getOp() == Nop::Region) {
        if(!subgraphs.count(id)) {
            unordered_map<int, string> init_set = {{id, str}};
            subgraphs.insert(pair(id, init_set));
        }
    } else {
        auto src_op = src_node->getOp();
        if(src_op == Nop::Stop || src_op == Nop::IfProj) { // float node
            floatNode.emplace_back(id, str);
        } else {
            auto region_id = region_ids[src_node->at(0)];
            subgraphs[region_id].insert(pair(id, str));
        }
    }
}

string SONDrawer::toDot(const string &graph_name) {

    std::stringstream stream;

    string graph_header = fmt::format("digraph {} {\nnode=[shape=record]", graph_name);

    stream << graph_header << std::endl;

    for(auto &subgraph: subgraphs) {
        auto graph_id = subgraph.first;
        auto &graph = subgraph.second;
        stream << fmt::format("subgraph region_{}{\ncolor=blue;", graph_id);
        for(auto &node: graph) {
            auto node_id = node.first;
            auto &node_str = node.second;
            // print node string
            stream << "\"node" << std::to_string(node_id) << "\"[\n";
            stream << "label=\"" << node_str << "\"\n]" << std::endl;
        }
        stream << "\n}\n";
    }

    // print float node
    for(auto &p: floatNode) {
        auto node_id = p.first;
        auto &node_str = p.second;
        stream << "\"node" << std::to_string(node_id) << "\"[\n";
        stream << "label=\"" << node_str << "\"\n]" << std::endl;
    }

    // print all edge
    for(auto &edge: edges) {
        stream << "\"node" << std::to_string(std::get<0>(edge)) << "\" -> ";
        stream << "\"node" << std::to_string(std::get<1>(edge)) << "\"[\n";
        stream << "label=\""  << std::get<2>(edge) << "\"\n];" << std::endl;
    }

    stream << "\n}";

    return stream.str();
}


}
