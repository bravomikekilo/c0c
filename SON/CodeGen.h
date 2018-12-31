//
// Created by baomingkun on 18-12-30.
//

#ifndef C0_CODEGEN_H
#define C0_CODEGEN_H

#include "SON.h"
#include "Arch/Asm.h"
#include "IRGraph.h"

namespace C0 {

// perform phi lifting transform
void phiLifting(IRGraph &graph);

//
void coloring(IRGraph &graph);

//
void postProcess(IRGraph &graph);

//
void codeGen(Asm &sheet);


}
#endif //C0_CODEGEN_H
