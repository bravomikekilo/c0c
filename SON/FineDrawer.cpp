//
// Created by baomingkun on 18-12-24.
//

#include "FineDrawer.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include <sstream>
#include <stack>


namespace C0 {

using std::stringstream;
using std::stack;

bool isControlNode(UseE node) {
    auto op = node->getOp();
    switch(op) {
        case Nop::Region:
        case Nop::Stop:
        case Nop::If:
        case Nop::IfProj:
            return true;
        default:
            return false;
    }
}

void FineDrawer::draw(StopN *end) {
    int curr_id = 0;
    unordered_map<UseE, int> node_id;
    unordered_set<UseE> visited;
    stack<UseE> edge_stack;
    edge_stack.push(end);
    visited.insert(end);

    while(!edge_stack.empty()) {
        auto head = edge_stack.top(); edge_stack.pop();

        auto head_id = 0;
        if(!node_id.count(head)) {
            head_id = curr_id;
            node_id.insert(pair(head, curr_id));
            node_str.push_back(drawNode(head));
            ++curr_id;
        } else {
            head_id = node_id.at(head);
        }

        int index = 0;
        for(auto use : *head) {
            if(!isControlNode(use)) {
                continue;
            }
            int use_id = 0;
            if(node_id.count(use)) {
                use_id = node_id.at(use);
            } else {
                use_id = curr_id;
                node_id.insert(pair(use, curr_id));
                node_str.push_back(drawNode(use));
                ++curr_id;
            }

            edges.emplace_back(use_id, head_id, std::to_string(index));
            ++index;

            if(!visited.count(use)) {
                visited.insert(use);
                edge_stack.push(use);
            }
        }

    }
}

string FineDrawer::toDot(const string &graph_name) const {
    std::stringstream stream;

    string graph_header = fmt::format("digraph {} {{\nnode[shape=box]\nrankdir=\"BT\"\n", graph_name);

    stream << graph_header << std::endl;


    // print float node
    auto id = 0;
    for (auto &p: node_str) {
        stream << "\"node" << std::to_string(id) << "\"[\n";
        stream << "label=\"" << p << "\"\n]" << std::endl;
        ++id;
    }

    // print all edge
    for (auto &edge: edges) {
        stream << "\"node" << std::to_string(std::get<0>(edge)) << "\" -> ";
        stream << "\"node" << std::to_string(std::get<1>(edge)) << "\"[\n";
        stream << "color=\"blue\";" << std::endl;
        stream << "label=\"" << std::get<2>(edge) << "\"\n];" << std::endl;
    }

    stream << "}\n\n";
    return stream.str();
}

string FineDrawer::drawNode(UseE node) {
    auto op = node->getOp();
    if(op == Nop::IfProj) {
        auto *n = (IfProjN *)node;
        return n->field ? "true" : "false";
    } else {
        return node->str();
    }
}

}
