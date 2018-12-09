//
// Created by baomingkun on 18-11-26.
//

#include "CFGSerializer.h"
#include "RegAlloc.h"
#include "Asm/Insts.h"
#include "AST/FuncAST.h"

namespace C0 {

void CFGSerializer::visit(C0::BasicBlock *block) {
    restored = false;

    if (verbose) {
        std::cout << "serialize basic block:" << block->getBid() << std::endl;
    }
    const auto *reg_table = block->payload<RegTable>();
    auto curr_block_label = make_unique<LInst>(getBlockLabel(block));
    list.addInst(std::move(curr_block_label));

    for (const auto &p: *reg_table) {
        if (verbose) {
            std::cout << varToString(p.first, sym_table)
                      << "->" << p.second->toString() << std::endl;
        }
        auto v = p.first;
        const auto *term = sym_table->findVarByID(v.val);
        if (term == nullptr) { // temp variable
            list.pushInst<LwInst>(
                    unique_ptr<Reg>(p.second->clone()),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(v)
            );
            continue;
        }

        if (term->isGlobal) {
            if (term->type.isArray()) {
                list.pushInst<LaInst>(
                        unique_ptr<Reg>(p.second->clone()),
                        getGlobalLabel(term->name)
                );
            } else {
                list.pushInst<LwInst>(
                        unique_ptr<Reg>(p.second->clone()),
                        getGlobalLabel(term->name)
                );
            }
            continue;
        }

        if (term->type.isArray()) {
            list.pushInst<AddI>(
                    unique_ptr<Reg>(p.second->clone()),
                    make_unique<SpReg>(),
                    make_unique<IntReg>(frame_table->getTopOffset(v))
            );
        } else {
            list.pushInst<LwInst>(
                    unique_ptr<Reg>(p.second->clone()),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(v)
            );
        }
    }

    for (auto &inst: block->insts) {
        switch (inst.op) {
            case QuadOp::Add:
            case QuadOp::Sub:
            case QuadOp::Mul:
            case QuadOp::Div:
                handleArith(inst, reg_table);
                break;
            case QuadOp::Copy:
                handleCopy(inst, reg_table);
                break;
            case QuadOp::Call:
                handleCall(inst, reg_table);
                break;
            case QuadOp::Ret:
                handleRet(inst, reg_table);
                break;
            case QuadOp::Read:
                handleRead(inst, reg_table);
                break;
            case QuadOp::PrintChar:
            case QuadOp::PrintInt:
                handlePrint(inst, reg_table);
                break;
            case QuadOp::GetInt:
            case QuadOp::GetChar:
                handleGet(inst, reg_table);
                break;
            case QuadOp::SetInt:
            case QuadOp::SetChar:
                handleSet(inst, reg_table);
                break;
            case QuadOp::B:
                handleB(inst, reg_table);
                break;
            case QuadOp::Beq:
            case QuadOp::Bne:
            case QuadOp::Bgt:
            case QuadOp::Bge:
            case QuadOp::Blt:
            case QuadOp::Ble:
                handleJmp(inst, reg_table);
                break;
        }
    }

    if (!restored) {
        storeRegAfterBasicBlock(reg_table);
    }

    if (block->next != nullptr && nextBlock() != block->next) {
        unique_ptr<Inst> jmp = make_unique<BInst>(getBlockLabel(block->next));
        list.addInst(std::move(jmp));
    }

}

void CFGSerializer::handleArith(Quad &q, const RegTable *table) {

    unique_ptr<Reg> dst;
    bool dst_on_stack = table->count(q.dst) == 0;
    if (dst_on_stack) {
        dst = make_unique<TReg>(2);
    } else {
        dst = unique_ptr<Reg>(table->at(q.dst)->clone());
    }

    auto vl = q.src0.constVal(*sym_table);
    auto vr = q.src1.constVal(*sym_table);

    if (vl.has_value() && vr.has_value()) {
        int s = evalArith(q.op, vl.value(), vr.value());
        list.pushInst<LiInst>(
                unique_ptr<Reg>(dst->clone()),
                s
        );
    } else {

        auto src0 = getVal(q.src0, 0, table);
        auto src1 = getVal(q.src1, 1, table);


        unique_ptr<Inst> inst;
        switch (q.op) {
            case QuadOp::Add:
                inst = make_unique<AddI>(std::move(dst), std::move(src0), std::move(src1));
                break;
            case QuadOp::Sub:
                inst = make_unique<SubI>(std::move(dst), std::move(src0), std::move(src1));
                break;
            case QuadOp::Mul:
                inst = make_unique<MulI>(std::move(dst), std::move(src0), std::move(src1));
                break;
            case QuadOp::Div:
                inst = make_unique<DivI>(std::move(dst), std::move(src0), std::move(src1));
                break;
        }

        list.addInst(std::move(inst));
    }

    if (dst_on_stack) {
        auto *term = sym_table->findVarByID(q.dst.val);
        if (term != nullptr && term->isGlobal) {
            list.pushInst<SwInst>(
                    make_unique<TReg>(2),
                    getGlobalLabel(term->name)
            );
        } else {
            auto store = make_unique<SwInst>(
                    make_unique<TReg>(2),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(q.dst)
            );
            list.addInst(std::move(store));
        }

    }


}


void CFGSerializer::handleJmp(Quad &q, const RegTable *table) {

    auto lv = q.src0.constVal(*sym_table);
    auto rv = q.src1.constVal(*sym_table);

    if (lv.has_value() && rv.has_value()) {
        if (evalCond(q.op, lv.value(), rv.value())) {
            list.pushInst<BInst>(getBlockLabel(q.jmp));
        }
        return;
    }

    auto src0 = getVal(q.src0, 0, table);
    auto src1 = getVal(q.src1, 1, table);


    auto label = getBlockLabel(q.jmp);
    unique_ptr<Inst> inst;
    if (!restored) {
        storeRegAfterBasicBlock(table);
        restored = true;
    }

    switch (q.op) {
        case QuadOp::Beq:
            inst = make_unique<BeqInst>(std::move(src0), std::move(src1), label);
            break;
        case QuadOp::Bne:
            inst = make_unique<BneInst>(std::move(src0), std::move(src1), label);
            break;
        case QuadOp::Bgt:
            inst = make_unique<BgtInst>(std::move(src0), std::move(src1), label);
            break;
        case QuadOp::Bge:
            inst = make_unique<BgeInst>(std::move(src0), std::move(src1), label);
            break;
        case QuadOp::Blt:
            inst = make_unique<BltInst>(std::move(src0), std::move(src1), label);
            break;
        case QuadOp::Ble:
            inst = make_unique<BleInst>(std::move(src0), std::move(src1), label);
            break;
    }

    list.addInst(std::move(inst));

}

void CFGSerializer::handleCopy(Quad &q, const RegTable *table) {
    unique_ptr<Reg> src0;
    auto src_const = q.src0.constVal(*sym_table);

    unique_ptr<Reg> src_reg;
    if (src_const.has_value()) {
        list.pushInst<LiInst>(make_unique<TReg>(0), src_const.value());
        src_reg = make_unique<TReg>(0);
    } else {
        src_reg = getVal(q.src0, 0, table);
    }

    unique_ptr<Reg> dst;
    if (q.dst.val == 0) return;

    const auto *term = sym_table->findVarByID(q.dst.val);

    if (table->count(q.dst) != 0) { // target on register
        if (src_const.has_value()) {
            list.pushInst<LiInst>(  // load literal
                    unique_ptr<Reg>(table->at(q.dst)->clone()),
                    src_const.value()
            );
        } else {
            list.pushInst<MoveInst>( // move to register
                    unique_ptr<Reg>(table->at(q.dst)->clone()),
                    std::move(src_reg)
            );
        }
    } else {  // target is global or on stack
        if (term != nullptr && term->isGlobal) { // target is global value
            list.pushInst<SwInst>(
                    std::move(src_reg),
                    getGlobalLabel(term->name)
            );
        } else {
            list.pushInst<SwInst>(
                    std::move(src_reg),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(q.dst)
            );
        }
    }

}

void CFGSerializer::handleGet(Quad &q, const RegTable *table) {
    auto op = q.op;

    putValTo(q.src0, make_unique<TReg>(0), table);
    putValTo(q.src1, make_unique<TReg>(1), table);


    if (op == QuadOp::GetInt) {
        list.pushInst<MulI>(
                std::move(make_unique<TReg>(1)),
                std::move(make_unique<TReg>(1)),
                std::move(make_unique<IntReg>(4))
        );
    }

    list.pushInst<AddI>(
            make_unique<TReg>(1),
            make_unique<TReg>(1),
            make_unique<TReg>(0)
    );

    if (table->count(q.dst) == 1) {
        if (op == QuadOp::GetInt) {
            list.pushInst<LwInst>(
                    unique_ptr<Reg>(table->at(q.dst)->clone()),
                    make_unique<TReg>(1)
            );
        } else {
            list.pushInst<LbInst>(
                    unique_ptr<Reg>(table->at(q.dst)->clone()),
                    make_unique<TReg>(1)
            );
        }
        return;
    }

    if (op == QuadOp::GetInt) {
        list.pushInst<LwInst>(
                make_unique<TReg>(2),
                make_unique<TReg>(1)
        );
    } else {
        list.pushInst<LbInst>(
                make_unique<TReg>(2),
                make_unique<TReg>(1)
        );
    }

    const auto *term = sym_table->findVarByID(q.dst.val);

    if (term != nullptr && term->isGlobal) {
        list.pushInst<SwInst>(
                make_unique<TReg>(2),
                getGlobalLabel(term->name)
        );
    } else {
        list.pushInst<SwInst>(
                make_unique<TReg>(2),
                make_unique<SpReg>(),
                frame_table->getTopOffset(q.dst)
        );
    }

}

void CFGSerializer::handleSet(Quad &q, const RegTable *table) {
    auto op = q.op;

    putValTo(q.dst, make_unique<TReg>(0), table);
    putValTo(q.src0, make_unique<TReg>(1), table);

    if (op == QuadOp::SetInt) {
        list.pushInst<MulI>(
                make_unique<TReg>(1),
                make_unique<TReg>(1),
                make_unique<IntReg>(4)
        );

    }

    list.pushInst<AddI>(
            make_unique<TReg>(1),
            make_unique<TReg>(0),
            make_unique<TReg>(1)
    );

    // putValTo(q.src1, make_unique<TReg>(0), table);
    unique_ptr<Reg> var;
    auto var_const = q.src1.constVal(*sym_table);
    if (var_const.has_value()) {
        list.pushInst<LiInst>(
                make_unique<TReg>(0),
                var_const.value()
        );
        var = make_unique<TReg>(0);
    } else {
        var = getVal(q.src1, 0, table);
    }

    if (op == QuadOp::SetInt) {
        list.pushInst<SwInst>(
                std::move(var),
                make_unique<TReg>(1)
        );
    } else {
        list.pushInst<SbInst>(
                std::move(var),
                make_unique<TReg>(1)
        );
    }

}

void CFGSerializer::handleB(Quad &q, const RegTable *table) {
    auto inst = make_unique<BInst>(getBlockLabel(q.jmp));
    list.addInst(std::move(inst));
}

void CFGSerializer::handlePrint(Quad &q, const RegTable *table) {
    if (q.str_id >= 0) {

        list.pushInst<LiInst>(
                make_unique<VReg>(0), 4
        );

        list.pushInst<LaInst>(
                make_unique<AReg>(0),
                Asm::genStringLabel(q.str_id)
        );

        list.addInst(std::move(make_unique<SysCallInst>()));
    }

    if (q.src0.val != 0) {

        if (q.op == QuadOp::PrintChar) {
            list.pushInst<LiInst>(make_unique<VReg>(0), 11);
        } else {
            list.pushInst<LiInst>(make_unique<VReg>(0), 1);
        }
        putValTo(q.src0, make_unique<AReg>(0), table);
        list.pushInst<SysCallInst>();
    }

    list.pushInst<LiInst>(make_unique<VReg>(0), 11);
    list.pushInst<LiInst>(make_unique<AReg>(0), '\n');
    list.pushInst<SysCallInst>();

}

void CFGSerializer::handleRead(Quad &q, const RegTable *table) {
    for (auto &dst: q.call_ext->second) {
        const auto *term = sym_table->findVarByID(dst.val);
        if (term != nullptr && term->type.is(BaseTypeK::Char)) {
            list.pushInst<LiInst>(make_unique<VReg>(0), 12);
        } else {
            list.pushInst<LiInst>(make_unique<VReg>(0), 5);
        }
        list.pushInst<SysCallInst>();

        if (table->count(dst) == 1) { // target on register
            list.pushInst<MoveInst>(
                    unique_ptr<Reg>(table->at(dst)->clone()),
                    make_unique<VReg>(0)
            );
            continue;
        }

        if (term != nullptr && term->isGlobal) {
            list.pushInst<SwInst>(
                    make_unique<VReg>(0),
                    getGlobalLabel(term->name)
            );
        } else {
            list.pushInst<SwInst>(
                    make_unique<VReg>(0),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(dst)
            );
        }

    }
}

void CFGSerializer::issueRet() {
    recoverReg();

    list.pushInst<AddI>(
            make_unique<SpReg>(),
            make_unique<SpReg>(),
            make_unique<IntReg>(func_offset.at(curr_func->name))
    );

    list.pushInst<RetInst>();


}


void CFGSerializer::handleRet(Quad &q, const RegTable *table) {

    // recover $ra
    list.pushInst<LwInst>(make_unique<RaReg>(), make_unique<SpReg>(), 32);

    auto val = q.src0;

    if (val.val == 0 && !val.isConst) {
        issueRet();
        return;
    }

    auto ret_const = val.constVal(*sym_table);
    if (ret_const.has_value()) {
        list.pushInst<LiInst>(
                make_unique<VReg>(0),
                ret_const.value()
        );
        issueRet();
        return;
    }

    auto *term = sym_table->findVarByID(val.val);
    if (term != nullptr && term->isGlobal) {
        list.pushInst<LwInst>(
                make_unique<VReg>(0),
                getGlobalLabel(term->name)
        );
        issueRet();
        return;
    }


    if (table->count(val)) {
        list.pushInst<MoveInst>(
                make_unique<VReg>(0),
                unique_ptr<Reg>(table->at(val)->clone())
        );
        // return make_unique<SReg>(*table->at(val));
    } else {
        list.pushInst<LwInst>(
                make_unique<VReg>(0),
                make_unique<SpReg>(),
                frame_table->getTopOffset(val)
        );
    }
    issueRet();
}

void CFGSerializer::handleCall(Quad &q, const RegTable *table) {
    const auto &func_name = q.call_ext->first;
    const auto &args = q.call_ext->second;
    auto num_arg = args.size();
    for (int i = 0; i < num_arg; ++i) {
        const int arg_offset = (i + 1) * -4;
        const auto &arg = args[i];
        auto *term = sym_table->findVarByID(arg.val);
        auto arg_const = arg.constVal(*sym_table);
        if (arg_const.has_value()) {
            list.pushInst<LiInst>(
                    make_unique<TReg>(0),
                    arg_const.value()
            );
            list.pushInst<SwInst>(
                    make_unique<TReg>(0),
                    make_unique<SpReg>(),
                    arg_offset
            );
            continue;
        }

        if (term != nullptr && term->isGlobal) {
            list.pushInst<LwInst>(
                    make_unique<TReg>(0),
                    getGlobalLabel(term->name)
            );
            list.pushInst<SwInst>(
                    make_unique<TReg>(0),
                    make_unique<SpReg>(),
                    arg_offset
            );
            continue;
        }

        if (table->count(arg)) {
            list.pushInst<SwInst>(
                    unique_ptr<Reg>(table->at(arg)->clone()),
                    make_unique<SpReg>(),
                    arg_offset
            );
        } else {
            list.pushInst<LwInst>(
                    make_unique<TReg>(0),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(arg)
            );
            list.pushInst<SwInst>(
                    make_unique<TReg>(0),
                    make_unique<SpReg>(),
                    arg_offset
            );
        }

    }

    list.pushInst<SubI>(
            make_unique<SpReg>(),
            make_unique<SpReg>(),
            make_unique<IntReg>(func_offset.at(func_name))
    );

    list.pushInst<CallInst>(
            func_name
    );
    // store function return value
    if (q.dst.val == 0) return;

    if (table->count(q.dst) == 1) {  // result on register
        list.pushInst<MoveInst>(
                unique_ptr<Reg>(table->at(q.dst)->clone()),
                make_unique<VReg>(0)
        );
    } else { // not on register
        const auto *term = sym_table->findVarByID(q.dst.val);

        if (term != nullptr && term->isGlobal) { // global variable
            list.pushInst<SwInst>(
                    make_unique<VReg>(0),
                    getGlobalLabel(term->name)
            );
        } else { // on stack
            list.pushInst<SwInst>(
                    make_unique<VReg>(0),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(q.dst)
            );
        }
    }

}

unique_ptr<Reg> CFGSerializer::getVal(QuadVal &val, size_t n, const RegTable *table) {
    auto val_const = val.constVal(*sym_table);
    if (val_const.has_value()) {
        return make_unique<IntReg>(val_const.value());
    }


    const auto *term = sym_table->findVarByID(val.val);

    if (term != nullptr && term->isGlobal) {
        list.pushInst<LwInst>(
                make_unique<TReg>(n),
                getGlobalLabel(term->name)
        );
        return make_unique<TReg>(n);
    }

    if (table->count(val)) {
        return unique_ptr<Reg>(table->at(val)->clone());
    } else {
        if (term != nullptr && term->type.isArray()) {
            list.pushInst<LaInst>(
                    make_unique<TReg>(n),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(val)
            );
        } else {
            list.pushInst<LwInst>(
                    make_unique<TReg>(n),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(val)
            );
        }

    }
    return make_unique<TReg>(n);


}

void CFGSerializer::serialize(shared_ptr<FuncAST> func, BasicBlock *block, shared_ptr<frameTable> frame) {
    curr_func = func;
    sym_table = func->table;
    frame_table = frame;
    list.pushInst<LInst>(func->name);
    list.pushInst<SwInst>(make_unique<RaReg>(), make_unique<SpReg>(), 32);
    saveReg();
    walk(block);
    frame_table = nullptr;
}

void CFGSerializer::loadGlobalScalar(const string &name, unique_ptr<Reg> &&reg) {

}

void CFGSerializer::storeGlobalScalar(const string &name, unique_ptr<Reg> &&reg) {

}

void CFGSerializer::loadGlobalArrayAddr(const string &name, unique_ptr<Reg> &&reg) {

}

void CFGSerializer::putValTo(const QuadVal &val, unique_ptr<Reg> reg, const RegTable *table) {
    if (val.val == 0 && !val.isConst) return;

    auto val_const = val.constVal(*sym_table);
    if (val_const.has_value()) {
        list.pushInst<LiInst>(
                unique_ptr<Reg>(reg->clone()),
                val_const.value()
        );
        return;
    }

    const auto *term = sym_table->findVarByID(val.val);
    if (term != nullptr && term->isGlobal) {
        if (term->type.isArray()) {
            list.pushInst<LaInst>(
                    unique_ptr<Reg>(reg->clone()),
                    getGlobalLabel(term->name)
            );
        } else {
            list.pushInst<LwInst>(
                    unique_ptr<Reg>(reg->clone()),
                    getGlobalLabel(term->name)
            );
        }
    } else {
        if (table->count(val) == 0) { // on stack
            if (term != nullptr && term->type.isArray()) {
                list.pushInst<LaInst>(
                        unique_ptr<Reg>(reg->clone()),
                        make_unique<SpReg>(),
                        frame_table->getTopOffset(val)
                );
            } else {
                list.pushInst<LwInst>(
                        unique_ptr<Reg>(reg->clone()),
                        make_unique<SpReg>(),
                        frame_table->getTopOffset(val)
                );
            }
        } else { // on register
            list.pushInst<MoveInst>(
                    unique_ptr<Reg>(reg->clone()),
                    unique_ptr<Reg>(table->at(val)->clone())
            );
        }

    }

}

void CFGSerializer::saveReg() {
    for (int i = 0; i < 8; ++i) {
        list.pushInst<SwInst>(
                make_unique<SReg>(i),
                make_unique<SpReg>(),
                4 * i
        );
    }
}

void CFGSerializer::storeRegAfterBasicBlock(const RegTable *reg_table) {
    for (const auto &p: *reg_table) {
        const auto *term = sym_table->findVarByID(p.first.val);
        if (term != nullptr && term->type.isArray()) {
            continue;
        }
        list.pushInst<SwInst>(
                unique_ptr<Reg>(reg_table->at(p.first)->clone()),
                make_unique<SpReg>(),
                frame_table->getTopOffset(p.first)
        );
    }

}

void CFGSerializer::recoverReg() {
    for (int i = 0; i < 8; ++i) {
        list.pushInst<LwInst>(
                make_unique<SReg>(i),
                make_unique<SpReg>(),
                4 * i
        );
    }
}

bool CFGSerializer::evalCond(QuadOp op, int lhs, int rhs) {
    switch (op) {
        case QuadOp::Beq:
            return lhs == rhs;
        case QuadOp::Bne:
            return lhs != rhs;
        case QuadOp::Bgt:
            return lhs > rhs;
        case QuadOp::Bge:
            return lhs >= rhs;
        case QuadOp::Blt:
            return lhs < rhs;
        case QuadOp::Ble:
            return lhs <= rhs;
        default:
            return false;
    }
}

int CFGSerializer::evalArith(QuadOp op, int lhs, int rhs) {
    switch (op) {
        case QuadOp::Add:
            return lhs + rhs;
        case QuadOp::Sub:
            return lhs - rhs;
        case QuadOp::Mul:
            return lhs * rhs;
        case QuadOp::Div:
            return lhs / rhs;
        default:
            return 0;
    }
}

} // end namespace C0
