//
// Created by baomingkun on 18-11-18.
//

#include "CFGDrawer.h"
#include <stack>
#include <sstream>


namespace C0 {


void CFGDrawer::draw(C0::BasicBlock *start, shared_ptr<SymTable> table) {


    sym_table = std::move(table);

    std::stack<BasicBlock *> s;
    if (start != nullptr) s.push(start);

    while (!s.empty()) {
        auto b = s.top();
        s.pop();
        if (visited.count(b) != 0) continue;
        visited.insert(b);

        std::list<string> l;
        for (const auto &q: b->insts) {
            l.push_back(q.toString(sym_table));
        }
        cfg.insert(pair(b->getBid(), l));

        if (b->next) {
            s.push(b->next);
            edges.emplace_back(b->getBid(), b->next->getBid(), "next");
        }

        if (!b->insts.empty()) {
            auto other = b->insts.back().jmp;
            if (other) {
                s.push(other);
                edges.emplace_back(b->getBid(), other->getBid(), "jmp");
            }
        }

    }
}

string CFGDrawer::getDot(const string &graph_name) const {
    std::stringstream stream;
    stream << fmt::format("digraph {} {{\ngraph[\nrankdir = \"TD\"\n];\n", graph_name);

    for (const auto &p: cfg) {
        stream << "\"node" << std::to_string(p.first) << "\"[\n";
        std::stringstream labelstream;
        labelstream << "<header> block:" << std::to_string(p.first);

        const auto &insts = p.second;

        auto sz = insts.size();
        int count = 1;

        for (const auto &inst: insts) {
            if (count >= sz) break;
            labelstream << "|" << inst;
            ++count;
        }

        labelstream << "| <tail> "; // << insts.back();
        if (!insts.empty()) labelstream << insts.back();


        stream << "label = \"{" << labelstream.str() << "}\"\n";


        stream << "shape=\"record\"\n];\n";
    }

    for (const auto &p: edges) {
        stream << "\"node" << std::to_string(std::get<0>(p))
               << "\":tail -> "
               << "\"node" << std::to_string(std::get<1>(p))
               << "\":header[\n";
        stream << "label=\"" << std::get<2>(p) << "\"\n];" << std::endl;
    }

    stream << "\n}";

    return stream.str();
}


}