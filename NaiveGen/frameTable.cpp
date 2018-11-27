//
// Created by baomingkun on 18-11-25.
//


#include "frameTable.h"

namespace C0 {


void C0::frameTable::addVal(QuadVal val, int size) {
    if(size % 4 != 0) {
        size += 4 - (size % 4);
    }

    auto head = -offset + size;

    offsets.insert(pair(val, -head));

    offset = -head;
}

void frameTable::addTemp(size_t num_temp) {
    for(int i = 0; i < num_temp; ++i) {
        addVal(QuadVal(-i - 1), 4);
    }

}


} // end namespace C0
