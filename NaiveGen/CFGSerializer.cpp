//
// Created by baomingkun on 18-11-26.
//

#include "CFGSerializer.h"
#include "RegAlloc.h"
#include "Asm/Insts.h"
#include "AST/FuncAST.h"

namespace C0 {

void CFGSerializer::visit(C0::BasicBlock *block) {
    const auto *reg_table = block->payload<RegTable>();
    auto curr_block_label = make_unique<LInst>(getBlockLabel(block));
    list.addInst(std::move(curr_block_label));

    for (const auto &p: *reg_table) {
        // putValTo(p.first, unique_ptr<Reg>(p.second->clone()), reg_table);
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
            case QuadOp::Print:
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

    for (const auto &p: *reg_table) {
        const auto *term = sym_table->findVarByID(p.first.val);
        if (term != nullptr && term->type.isArray()) {
            continue;
        }
        list.pushInst<SwInst>(
                unique_ptr<Reg>(reg_table->at(p.first)->clone()),
                unique_ptr<SpReg>(),
                frame_table->getTopOffset(p.first)
        );
    }

    if (nextBlock() != block->next) {
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

    if (q.src0.isConst && q.src1.isConst) {
        auto vl = sym_table->findVarByID(q.src0.val)->val.value();
        auto vr = sym_table->findVarByID(q.src0.val)->val.value();
        int s = vl + vr;
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

    if (q.src1.isConst && q.src0.isConst) {
        int lv = sym_table->findVarByID(q.src0.val)->val.value();
        int rv = sym_table->findVarByID(q.src1.val)->val.value();
        if (evalCond(q.op, lv, rv)) {
            list.pushInst<BInst>(getBlockLabel(q.jmp));
        } else {
            return;
        }
    }

    unique_ptr<Reg> src0;
    if (table->count(q.src0) == 0) {
        auto load = make_unique<LwInst>(
                make_unique<TReg>(0),
                make_unique<SpReg>(), frame_table->getTopOffset(q.src0)
        );
        list.addInst(std::move(load));
        src0 = make_unique<TReg>(0);
    } else {
        src0 = unique_ptr<Reg>(table->at(q.src0)->clone());
    }

    unique_ptr<Reg> src1;
    // settle src0
    if (table->count(q.src1) == 0) {
        auto load = make_unique<LwInst>(
                make_unique<TReg>(0),
                make_unique<SpReg>(), frame_table->getTopOffset(q.src0));
        list.addInst(std::move(load));
        src1 = make_unique<TReg>(1);
    } else {
        src1 = unique_ptr<Reg>(table->at(q.src0)->clone());
    }

    auto label = getBlockLabel(q.jmp);
    unique_ptr<Inst> inst;
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
    unique_ptr<Reg> target;
    if (table->count(q.src0) == 0) {
        auto load = make_unique<LwInst>(
                make_unique<TReg>(0),
                make_unique<SpReg>(), frame_table->getTopOffset(q.src0)
        );
        list.addInst(std::move(load));
        src0 = make_unique<TReg>(0);
        target = make_unique<TReg>(0);
    } else {
        src0 = unique_ptr<Reg>(table->at(q.src0)->clone());
        target = make_unique<TReg>(0);
    }

    unique_ptr<Reg> dst;
    if (table->count(q.dst) != 0) {
        dst = unique_ptr<Reg>(table->at(q.src0)->clone());
        unique_ptr<Inst> inst = make_unique<MoveInst>(std::move(src0), std::move(dst));
        list.addInst(std::move(inst));
        return;
    } else {
        unique_ptr<Inst> inst = make_unique<SwInst>(
                std::move(target),
                make_unique<SpReg>(), frame_table->getTopOffset(q.dst)
        );
        list.addInst(std::move(inst));
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
            std::move(make_unique<TReg>(1)),
            std::move(make_unique<TReg>(0)),
            std::move(make_unique<IntReg>(1))
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

    if (op == QuadOp::GetInt) {
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

    putValTo(q.src1, make_unique<TReg>(0), table);

    if (op == QuadOp::GetInt) {
        list.pushInst<SwInst>(
                make_unique<TReg>(0),
                make_unique<TReg>(1)
        );
    } else {
        list.pushInst<SbInst>(
                make_unique<TReg>(0),
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
        auto li = make_unique<LiInst>(make_unique<VReg>(0), 4);
        list.addInst(std::move(li));
        auto load_str_id = make_unique<LiInst>(make_unique<AReg>(0), q.str_id);
        list.addInst(std::move(load_str_id));
        list.addInst(std::move(make_unique<SysCallInst>()));
    }

    if (q.str_id >= 0) {
        auto li = make_unique<LiInst>(make_unique<VReg>(0), 4);
        list.addInst(std::move(li));
        auto load_str_id = make_unique<LiInst>(make_unique<AReg>(0), q.str_id);
        list.addInst(std::move(load_str_id));
        list.addInst(std::move(make_unique<SysCallInst>()));
    }

    if (q.dst.val != 0) {
        auto *ret_term = sym_table->findVarByID(q.dst.val);
        if (ret_term != nullptr && ret_term->type.is(BaseTypeK::Char)) {
            list.pushInst<LiInst>(make_unique<VReg>(0), 11);
        } else {
            list.pushInst<LiInst>(make_unique<VReg>(0), 1);
        }
        putValTo(q.dst, make_unique<AReg>(0), table);
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

void CFGSerializer::handleRet(Quad &q, const RegTable *table) {

    // recover $ra

    auto val = q.src0;

    if (val.val == 0) {
        list.pushInst<RetInst>();
    }

    if (val.isConst) {
        auto *term = sym_table->findVarByID(val.val);
        int v = term->val.value();
        list.pushInst<LiInst>(
                make_unique<VReg>(0),
                v
        );
        list.pushInst<RetInst>();
        return;
    }

    auto *term = sym_table->findVarByID(val.val);
    if (term != nullptr && term->isGlobal) {
        list.pushInst<LwInst>(
                make_unique<VReg>(0),
                getGlobalLabel(term->name)
        );
        list.pushInst<RetInst>();
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

    list.pushInst<SubI>(
            make_unique<SpReg>(),
            make_unique<SpReg>(),
            make_unique<IntReg>(func_offset.at(curr_func->name))
    );

    list.pushInst<RetInst>();

}

void CFGSerializer::handleCall(Quad &q, const RegTable *table) {
    const auto &func_name = q.call_ext->first;
    const auto &args = q.call_ext->second;
    auto num_arg = args.size();
    for (int i = 0; i < num_arg; ++i) {
        const auto &arg = args[i];
        auto *term = sym_table->findVarByID(arg.val);
        if (arg.isConst) {
            int v = term->val.value();
            list.pushInst<LiInst>(
                    make_unique<TReg>(0),
                    v
            );
            list.pushInst<SwInst>(
                    make_unique<TReg>(0),
                    make_unique<SpReg>(),
                    (i + 1) * 4
            );
            continue;
        }

        if (term != nullptr && term->isGlobal) {
            list.pushInst<LwInst>(
                    make_unique<VReg>(0),
                    getGlobalLabel(term->name)
            );
            list.pushInst<SwInst>(
                    make_unique<TReg>(0),
                    make_unique<SpReg>(),
                    (i + 1) * 4
            );
            return;
        }

        if (table->count(arg)) {
            list.pushInst<SwInst>(
                    unique_ptr<Reg>(table->at(arg)->clone()),
                    make_unique<SpReg>(),
                    (i + 1) * 4
            );
        } else {
            list.pushInst<LwInst>(
                    make_unique<VReg>(0),
                    make_unique<SpReg>(),
                    frame_table->getTopOffset(arg)
            );
            list.pushInst<SwInst>(
                    make_unique<VReg>(0),
                    make_unique<SpReg>(),
                    (i + 1) * 4
            );
        }

    }

    list.pushInst<AddI>(
            make_unique<SpReg>(),
            make_unique<SpReg>(),
            make_unique<IntReg>(func_offset.at(func_name))
    );

    list.pushInst<CallInst>(
            func_name
    );

}

unique_ptr<Reg> CFGSerializer::getVal(QuadVal &val, size_t n, const RegTable *table) {
    if (val.isConst) {
        auto *term = sym_table->findVarByID(val.val);
        int v = term->val.value();
        return make_unique<IntReg>(v);
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
    list.pushInst<LInst>(
            func->name
            );
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
    if (val.val == 0) return;

    if (val.isConst) {
        list.pushInst<LiInst>(
                unique_ptr<Reg>(reg->clone()),
                sym_table->findVarByID(val.val)->val.value()
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
                        unique_ptr<SpReg>(),
                        frame_table->getTopOffset(val)
                );
            } else {
                list.pushInst<LwInst>(
                        unique_ptr<Reg>(reg->clone()),
                        unique_ptr<SpReg>(),
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

} // end namespace C0
