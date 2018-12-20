//
// Created by baomingkun on 18-12-20.
//

#ifndef C0_POS_H
#define C0_POS_H

#include "common.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

    struct Pos {
        size_t ln;
        size_t col;
        string toStr() {
            return fmt::format("ln:{}, col:{}", ln, col);
        }
    };

}

#endif //C0_POS_H
