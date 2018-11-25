//
// Created by baomingkun on 18-11-23.
//

#include "NodeArena.h"

namespace C0 {

NodeArena::~NodeArena() {
    for (auto ch : chunks) {
        delete[] ch;
    }
}

}