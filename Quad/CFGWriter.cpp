//
// Created by baomingkun on 19-1-3.
//

#include "CFGWriter.h"

namespace C0 {


string writeCFG(C0::BasicBlock *start, const shared_ptr<C0::SymTable> &table) {

    std::stringstream ss;

    std::stack<BasicBlock *> s;
    if (start != nullptr) s.push(start);
    unordered_set<BasicBlock *> visited;

    while (!s.empty()) {
        auto b = s.top();
        s.pop();
        if (visited.count(b) != 0) continue;
        visited.insert(b);

        ss << ".BB" << b->getBid() << "@" << b->pos.toStr() << std::endl;
        for (const auto &q: b->insts) {
            ss << "    " << q.toString(table) << std::endl;
        }

        if (b->next) {
            s.push(b->next);
            ss << "   " << "next " << b->next->getBid() << std::endl;
        }

        if (!b->insts.empty()) {
            auto other = b->insts.back().jmp;
            if (other) {
                s.push(other);
                // ss << "    " << "jmp " << other->getBid() << std::endl;
            }
        }
    }

    return ss.str();

}

}
