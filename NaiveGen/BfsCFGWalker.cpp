
#include "BfsCFGWalker.h"


void C0::BfsCFGWalker::walk(BasicBlock *start) {
    q.push(start);
    while (!q.empty()) {
        auto b = q.front();
        q.pop();
        if (visited.count(b) == 0) {
            visited.insert(b);
        } else {
            continue;
        }
        if (b->next != nullptr && visited.count(b->next) == 0) {
            q.push(b->next);
        }

        if (!b->insts.empty()) {
            auto jmp = b->insts.back().jmp;
            if (jmp != nullptr && visited.count(jmp) == 0) {
                q.push(jmp);
            }
        }

        visit(b);
    }
}

C0::BasicBlock *C0::BfsCFGWalker::nextBlock() {
    if (q.empty()) return nullptr;
    return q.front();
}
