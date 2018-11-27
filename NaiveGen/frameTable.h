
#ifndef C0_FRAMETABLE_H
#define C0_FRAMETABLE_H

#include "common.h"
#include "SymTable.h"
#include "Quad/Quads.h"

namespace C0 {

class frameTable {
private:
    unordered_map<QuadVal, int> offsets;
    int offset; /// offset of saved Ra

public:
    void addVal(QuadVal val, int size);

    void addTemp(size_t num_temp);

    bool hasVal(QuadVal val) {
        return offsets.count(val) == 1;
    }

    int getOffset(QuadVal val) {
        return offsets[val];
    }

    int getTopOffset(QuadVal val) {
        return getWholeSize() + getOffset(val);
    }

    int getWholeSize() {
        return -offset + 40;
    }

    int getSave(int num) {
        return offset - 1 - num * 4;
    }

};

} // end of namespace C0

#endif
