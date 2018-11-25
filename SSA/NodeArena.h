//
// Created by baomingkun on 18-11-23.
//

#ifndef C0_NODEARENA_H
#define C0_NODEARENA_H

#include "SSA.h"
#include <list>


namespace C0 {

class NodeArena { // arena for SSA Node
private:
    size_t arena_size;

    std::list<char *> chunks;

    char *curr;

    size_t water_mark = 0;
    char *top;

public:

    explicit NodeArena(size_t arena_size = 4096)
        :arena_size(arena_size) {
        curr = new char[arena_size];
        chunks.push_back(curr);
        top = curr;
    }

    template<typename NodeT, typename ...Arguments>
    NodeT *alloc(Arguments&& ...args) {

        auto sz = sizeof(NodeT);
        if(water_mark + sz > arena_size) {
            auto *new_chunk = new char[arena_size]; // allocate new chunk;
            curr = new_chunk;
            chunks.push_back(curr);
            water_mark = 0;
            top = curr;
        }

        water_mark = water_mark + sz;

        auto *ret = new (curr) NodeT(std::forward<Arguments>(args)...);

        curr += sz;

        return ret;

    }

    ~NodeArena();




};

}



#endif //C0_NODEARENA_H
