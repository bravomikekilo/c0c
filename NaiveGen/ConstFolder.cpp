//
// Created by baomingkun on 19-1-3.
//

#include "ConstFolder.h"

namespace C0 {


void C0::ConstFolder::visit(C0::BasicBlock *block) {
    block->constFolding(*table);
}

}
