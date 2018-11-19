//
// Created by baomingkun on 18-11-18.
//

#include "Quads.h"
#include "BasicBlock.h"

namespace C0 {

string opToString(QuadOp op) {
    switch (op) {
        case QuadOp::Add:
            return "Add";
        case QuadOp::Sub:
            return "Sub";
        case QuadOp::Mul:
            return "Mul";
        case QuadOp::Div:
            return "Div";
        case QuadOp::Copy:
            return "Copy";
        case QuadOp::Call:
            return "Call";
        case QuadOp::Ret:
            return "Ret";
        case QuadOp::GetInt:
            return "GetInt";
        case QuadOp::SetInt:
            return "SetInt";
        case QuadOp::GetChar:
            return "GetChar";
        case QuadOp::SetChar:
            return "SetChar";
        case QuadOp::B:
            return "JMP";
        case QuadOp::Beq:
            return "Bz";
        case QuadOp::Bne:
            return "Bne";
        case QuadOp::Bgt:
            return "Bgt";
        case QuadOp::Bge:
            return "Bge";
        case QuadOp::Blt:
            return "Blt";
        case QuadOp::Ble:
            return "Ble";
    }
}


string varToString(QuadVal v, shared_ptr<SymTable> table) {
    if (!v.isConst) {
        auto id = v.val;
        if (id == 0) {
            return "_";
        }

        if (id < 0) {
            return "#" + std::to_string(-id);
        }
        auto term = table->findVarByID(id);

        if (term->isConst()) {
            return "G:" + term->name;
        } else {
            return "L:" + term->name;
        }

    } else {
        return std::to_string(v.val);
    }

}


string Quad::toString(shared_ptr<SymTable> table) const {
    if (op == QuadOp::Call) {
        auto bulk = fmt::format("{} = {} Call",
                                varToString(dst, table),
                                call_ext->first
        );

        for (const auto &arg: call_ext->second) {
            bulk += fmt::format(" {}", varToString(arg, table));
        }
        return bulk;
    } else if (op == QuadOp::Copy) {
        return fmt::format("{} = Copy {}",
                           varToString(dst, table),
                           varToString(src0, table));
    } else if (op == QuadOp::Ret) {

        return fmt::format("Ret {}", varToString(src0, table));


    } else if (isSetOp(op)) {
        return fmt::format("{} {} {} {}",
                           varToString(dst, table),
                           opToString(op),
                           varToString(src0, table),
                           varToString(src1, table)
        );

    } else if (isBranchOp(op)) {
        return fmt::format("{} {} {} Block:{}",
                           opToString(op),
                           varToString(src0, table),
                           varToString(src1, table),
                           std::to_string(jmp->getBid())
        );

    } else { // all normal simple arithmatic op

        return fmt::format("{} = {} {} {}",
                           varToString(dst, table),
                           varToString(src0, table),
                           opToString(op),
                           varToString(src1, table)
        );

    }

}
}
