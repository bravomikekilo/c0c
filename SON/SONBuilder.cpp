//
// Created by baomingkun on 18-12-4.
//

#include "SONBuilder.h"
#include "AST/AST.h"
#include <utility>

namespace C0 {


void SONBuilder::visit(C0::IntExpr *e) {
    right_val = sea.alloc<ConstIntN>(curr_block, e->v);
}

void SONBuilder::visit(C0::CharExpr *e) {
    right_val = sea.alloc<ConstCharN>(curr_block, e->v);
}

void SONBuilder::visit(C0::VarExpr *e) {
    if (on_right) {
        this->visitRightVar(e);
    } else {
        this->visitLeftVar(e);
    }
}

void SONBuilder::visitLeftVar(VarExpr *e) {
    const auto *term = curr_table->findVarByID(e->varID);
    if (term->isGlobal) {
        auto pointer = sea.alloc<GlobalAddrN>(
                curr_block, global_offsets.at(e->varID),
                term->name
        );
        auto var = readVar(e->varID);
        UseE set = nullptr;
        if (term->type.getBase().is(BaseTypeK::Char)) {
            set = sea.alloc<SetCharN>(curr_block, pointer, var, right_val);
        } else {
            set = sea.alloc<SetIntN>(curr_block, pointer, var, right_val);
        }
        writeVar(e->varID, set);
    } else {
        writeVar(e->varID, right_val);
    }
}

void SONBuilder::visitRightVar(VarExpr *e) {
    const auto *term = curr_table->findVarByID(e->varID);
    if (term->isConst()) {
        if (term->type.is(BaseTypeK::Char)) {
            right_val = sea.alloc<ConstCharN>(curr_block, (char) (term->val.value()));
        } else {
            right_val = sea.alloc<ConstIntN>(curr_block, term->val.value());
        }
        return;
    }
    if (term->isGlobal) {
        addr = sea.alloc<GlobalAddrN>(
                curr_block, global_offsets.at(e->varID),
                term->name
        );
    } else if (term->type.isArray()) {
        addr = sea.alloc<StackSlotN>(curr_block, array_offsets[e->varID]);
    }
    right_val = readVar(e->varID);
    var_id = e->varID;
}


void SONBuilder::visit(C0::OpExpr *e) {
    if (on_right) {
        visitRightOp(e);
    } else {
        visitLeftOp(e);
    }

}

void SONBuilder::visitLeftOp(OpExpr *e) {
    on_right = true;
    auto val = right_val;
    e->lhs->accept(*this);
    auto id = var_id;
    auto base = addr;
    auto lhs = right_val;
    e->rhs->accept(*this);
    auto rhs = right_val;

    UseE set = nullptr;
    if (e->lhs->outType(curr_table).getBase().is(BaseTypeK::Char)) {
        auto pointer = sea.alloc<AddN>(curr_block, base, rhs);
        set = sea.alloc<SetCharN>(curr_block, pointer, lhs, val);
    } else {
        auto expand = sea.alloc<MulN>(curr_block, rhs, sea.alloc<ConstIntN>(curr_block, 4));
        auto pointer = sea.alloc<AddN>(curr_block, base, expand);
        set = sea.alloc<SetIntN>(curr_block, pointer, lhs, val);
    }

    writeVar(id, set);
}

void SONBuilder::visitRightOp(OpExpr *e) {
    e->lhs->accept(*this);
    auto base = addr;
    auto lhs = right_val;
    e->rhs->accept(*this);
    auto rhs = right_val;

    switch (e->op) {
        case Op::Ind: {
            if (e->lhs->outType(curr_table).getBase().is(BaseTypeK::Char)) {
                auto pointer = sea.alloc<AddN>(curr_block, base, rhs);
                right_val = sea.alloc<GetCharN>(curr_block, pointer, lhs);
            } else {
                auto expand = sea.alloc<MulN>(curr_block, rhs, sea.alloc<ConstIntN>(curr_block, 4));
                auto pointer = sea.alloc<AddN>(curr_block, base, expand);
                right_val = sea.alloc<GetIntN>(curr_block, pointer, lhs);
            }
            break;
        }
        case Op::Add:
            right_val = sea.alloc<AddN>(curr_block, lhs, rhs);
            break;
        case Op::Sub:
            right_val = sea.alloc<SubN>(curr_block, lhs, rhs);
            break;
        case Op::Mul:
            right_val = sea.alloc<MulN>(curr_block, lhs, rhs);
            break;
        case Op::Div:
            right_val = sea.alloc<DivN>(curr_block, lhs, rhs);
            break;
    }
}


void SONBuilder::visit(C0::CondAST *e) {
    e->lhs->accept(*this);
    auto lhs = right_val;
    e->rhs->accept(*this);
    auto rhs = right_val;

    switch (e->cp) {
        case Cmp::Greater:
            right_val = sea.alloc<GtN>(curr_block, lhs, rhs);
            break;
        case Cmp::Less:
            right_val = sea.alloc<LtN>(curr_block, lhs, rhs);
            break;
        case Cmp::GreaterEq:
            right_val = sea.alloc<GeN>(curr_block, lhs, rhs);
            break;
        case Cmp::LessEq:
            right_val = sea.alloc<LeN>(curr_block, lhs, rhs);
            break;
        case Cmp::Equal:
            right_val = sea.alloc<EqN>(curr_block, lhs, rhs);
            break;
        case Cmp::UnEqual:
            right_val = sea.alloc<NeN>(curr_block, lhs, rhs);
            break;
    }

}

void SONBuilder::visit(C0::CallExpr *e) {
    vector<UseE> globals;
    vector<UseE> args;

    for (auto &arg: e->args) {
        arg->accept(*this);
        args.push_back(right_val);
    }

    for (auto id: global_ids) {
        globals.push_back(readVar(id));
    }

    auto world = getWorld();

    auto call = sea.alloc<CallN>(e->name, curr_block, world, args, globals);
    auto new_world = sea.alloc<ProjWorldN>(curr_block, call);

    setWorld(new_world);
    right_val = sea.alloc<ProjRetN>(curr_block, call);
}


void SONBuilder::visit(AsStmt *e) {
    e->rhs->accept(*this);
    on_right = false;

    e->lhs->accept(*this);
    on_right = true;
}

void SONBuilder::visit(ExprStmt *e) {
    e->exp->accept(*this);
}

void SONBuilder::visit(IfStmt *e) {
    e->cond->accept(*this);
    auto Cond = sea.alloc<IfN>(curr_block, right_val);
    auto then_branch = sea.alloc<IfProjN>(Cond, true);
    auto else_branch = sea.alloc<IfProjN>(Cond, false);

    if (e->falseBranch.has_value()) {
        auto then_block = sea.alloc<RegionN>(then_branch);
        addContext(then_block);
        sealBlock(then_block);
        curr_block = then_block;

        e->trueBranch->accept(*this);
        if (waiting.empty()) {
            waiting.push_back(curr_block);
        }
        if (has_return) {
            has_return = false;
            waiting.clear();
        }
        vector<UseE> then_waiting;
        std::swap(then_waiting, waiting);


        auto else_block = sea.alloc<RegionN>(else_branch);
        addContext(else_block);
        sealBlock(else_block);
        curr_block = else_block;

        e->falseBranch.value()->accept(*this);
        if (waiting.empty()) {
            waiting.push_back(curr_block);
        }
        if (has_return) {
            has_return = false;
            waiting.clear();
        }

        waiting.insert(waiting.end(), then_waiting.begin(), then_waiting.end());
        if (waiting.empty()) has_return = true;

    } else {
        auto then_block = sea.alloc<RegionN>(then_branch);
        addContext(then_block);
        sealBlock(then_block);
        curr_block = then_block;

        e->trueBranch->accept(*this);
        if (waiting.empty()) {
            waiting.push_back(curr_block);
        }

        if (has_return) {
            has_return = false;
            waiting.clear();
        }

        waiting.push_back(else_branch);
    }
}

void SONBuilder::visit(BlockStmt *e) {
    for (auto &stmt: e->stmts) {

        if (has_return) {
            has_return = false;
            break;
        }

        if (!waiting.empty()) {
            auto block = sea.alloc<RegionN>(waiting);
            addContext(block);
            sealBlock(block);
            waiting.clear();
            curr_block = block;
        }
        stmt->accept(*this);
    }
}


void SONBuilder::visit(RetStmt *e) {
    UseE ret = nullptr;
    if (e->ret.has_value()) {
        e->ret.value()->accept(*this);
        ret = right_val;
    }


    vector<pair<VarID, UseE>> globals;

    for (auto id: global_ids) {
        globals.emplace_back(pair(id, readVar(id)));
    }

    auto world = getWorld();

    last_block->push_back(curr_block);
    world_phi->push_back(world);

    for (auto &p: globals) {
        last_global_phi[p.first]->push_back(p.second);
    }

    if (ret_phi != nullptr) {
        ret_phi->push_back(ret);
    }

    has_return = true;
}


void SONBuilder::visit(DoStmt *e) {

    auto body_block = sea.alloc<RegionN>(curr_block);
    addContext(body_block);
    curr_block = body_block;

    e->body->accept(*this);

    if (has_return) {
        sealBlock(body_block);
        return;
    }

    if (waiting.empty()) {
        e->cond->accept(*this);
        auto ifnode = sea.alloc<IfN>(curr_block, right_val);
        auto then_branch = sea.alloc<IfProjN>(ifnode, true);
        auto else_branch = sea.alloc<IfProjN>(ifnode, false);
        body_block->push_back(then_branch);
        sealBlock(body_block);
        waiting.push_back(else_branch);
    } else {
        auto cond_block = sea.alloc<RegionN>(waiting);
        addContext(cond_block);
        waiting.clear();
        e->cond->accept(*this);
        auto ifnode = sea.alloc<IfN>(curr_block, right_val);
        auto then_branch = sea.alloc<IfProjN>(ifnode, true);
        auto else_branch = sea.alloc<IfProjN>(ifnode, false);
        body_block->push_back(then_branch);
        sealBlock(cond_block);
        waiting.push_back(else_branch);
    }
}

void SONBuilder::visit(ForStmt *e) {
    if (e->start.has_value()) {
        e->start.value()->accept(*this);
    }

    auto cond_block = sea.alloc<RegionN>(curr_block);
    addContext(cond_block);

    curr_block = cond_block;
    e->cond->accept(*this);

    auto ifn = sea.alloc<IfN>(curr_block, right_val);
    auto true_branch = sea.alloc<IfProjN>(ifn, true);
    auto false_branch = sea.alloc<IfProjN>(ifn, false);

    auto body_block = sea.alloc<RegionN>(true_branch);
    addContext(body_block);
    sealBlock(body_block);

    curr_block = body_block;
    e->body->accept(*this);

    if (!has_return) {
        if (waiting.empty()) {
            if (e->after.has_value()) {
                e->after.value()->accept(*this);
            }
            cond_block->push_back(curr_block);
        } else {
            auto after_block = sea.alloc<RegionN>(waiting);
            addContext(after_block);
            sealBlock(after_block);
            cond_block->push_back(after_block);
        }
    } else {
        has_return = false;
    }

    sealBlock(cond_block);
    waiting.push_back(false_branch);
}

void SONBuilder::visit(PrintStmt *e) {
    if (!e->expr.has_value()) {
        auto world = getWorld();
        auto new_world = sea.alloc<PrintIntN>(curr_block, e->str, world);
        setWorld(new_world);
        return;
    }

    e->expr.value()->accept(*this);
    auto arg = right_val;
    auto world = getWorld();

    UseE new_world = nullptr;
    if (e->expr.value()->outType(curr_table).is(BaseTypeK::Char)) {
        new_world = sea.alloc<PrintCharN>(curr_block, e->str, world, arg);
    } else {
        new_world = sea.alloc<PrintIntN>(curr_block, e->str, world, arg);
    }
    setWorld(new_world);
}

void SONBuilder::visit(ReadStmt *e) {
    for (auto &var: e->vars) {
        UseE world = getWorld();
        auto curr_id = var->varID;
        const auto *term = curr_table->findVarByID(var->varID);

        UseE new_world = nullptr;
        UseE v = nullptr;
        if (term->isGlobal) {
            auto global = readVar(curr_id);
            auto pointer = sea.alloc<GlobalAddrN>(
                    curr_block, global_offsets.at(curr_id),
                    term->name
            );
            if (term->type.is(BaseTypeK::Char)) {
                v = sea.alloc<ReadCharN>(curr_block, world);
                setWorld(sea.alloc<ProjWorldN>(curr_block, v));
                auto result = sea.alloc<ProjRetN>(curr_block, v);
                v = sea.alloc<SetCharN>(curr_block, pointer, global, result);
            } else {
                v = sea.alloc<ReadIntN>(curr_block, world);
                setWorld(sea.alloc<ProjWorldN>(curr_block, v));
                auto result = sea.alloc<ProjRetN>(curr_block, v);
                v = sea.alloc<SetIntN>(curr_block, pointer, global, result);
            }
        } else {
            if (term->type.is(BaseTypeK::Char)) {
                v = sea.alloc<ReadCharN>(curr_block, world);
            } else {
                v = sea.alloc<ReadIntN>(curr_block, world);
            }
            setWorld(sea.alloc<ProjWorldN>(curr_block, v));
            v = sea.alloc<ProjRetN>(curr_block, v);
        }
        writeVar(curr_id, v);
    }
}

void SONBuilder::visit(FuncAST *e) {
    curr_table = e->table;
    /// build first block and finish init variable
    buildTable();

    start_block = sea.alloc<RegionN>();
    addContext(start_block);
    sealBlock(start_block);
    curr_block = start_block;


    UseE undef = sea.alloc<UndefN>(curr_block);
    auto *context = curr_block->Payload<BuildContext>();
    int n = 0;
    for (auto &arg: e->args) {
        auto var_id = curr_table->findVarByName(arg.second).value();
        auto proj = sea.alloc<ProjArgN>(curr_block, n);
        context->value.insert(pair(var_id, proj));
        ++n;
    }

    const auto &terms = curr_table->getVarInScope();
    for (auto &term: terms) {
        auto id = term.id;
        if (term.isConst() || context->value.count(id)) continue; // skip function argument and const
        context->value.insert(pair(id, undef));
    }

    setWorld(sea.alloc<InitWorldN>(curr_block));

    last_block = sea.alloc<RegionN>();
    addContext(last_block);
    sealBlock(last_block);

    vector<UseE> ret_globals;
    world_phi = sea.alloc<PhiN>(last_block);

    shared_ptr<SymTable> global_table = curr_table->getGlobalTable();
    const auto &global_terms = global_table->getVarInScope();
    for (auto &term: global_terms) {

        auto phi = sea.alloc<PhiN>(last_block);
        ret_globals.push_back(phi);

        auto id = term.id;
        global_ids.insert(id);
        last_global_phi.insert(pair(id, phi));
        auto init = sea.alloc<InitGlobalN>(start_block, term.name);
        context->value.insert(pair(id, init));
    }
    if (!e->retType.is(BaseTypeK::Void)) {
        ret_phi = sea.alloc<PhiN>(last_block);
        stop = sea.alloc<StopN>(last_block, ret_globals, world_phi, ret_phi);
    } else {
        stop = sea.alloc<StopN>(last_block, ret_globals, world_phi);
    }

    for (auto &stmt: e->stmts) {

        if (has_return) {
            break;
        }

        if (!waiting.empty()) {
            auto block = sea.alloc<RegionN>(waiting);
            addContext(block);
            sealBlock(block);
            waiting.clear();
            curr_block = block;
        }
        stmt->accept(*this);
    }

    if (!has_return) {
        // add implicit return

        vector<pair<VarID, UseE>> globals;

        for (auto id: global_ids) {
            globals.emplace_back(pair(id, readVar(id)));
        }

        auto world = getWorld();

        last_block->push_back(curr_block);
        world_phi->push_back(world);

        for (auto &p: globals) {
            last_global_phi[p.first]->push_back(p.second);
        }

        has_return = true;
    }
}


void SONBuilder::addContext(RegionN *region) {
    auto context = new BuildContext();
    region->setPayload(context);
    contexts.push_back(context);
}


void SONBuilder::writeVar(VarID id, UseE val) {
    auto *context = curr_block->Payload<BuildContext>();
    context->writeVar(id, val);
}

void SONBuilder::setWorld(UseE world) {
    writeVar(0, world);
}

UseE SONBuilder::getWorld() {
    return readVar(0);
}

UseE SONBuilder::readVar(VarID id) {
    return readVar(id, curr_block);
}

UseE SONBuilder::readVar(VarID id, RegionN *region) {
    auto *context = region->Payload<BuildContext>();

    if (id == 0 && context->world != nullptr) {
        return context->world;
    }

    if (context->value.count(id)) {
        return context->value[id];
    } else {
        return readVarRecursive(id, region);
    }
}

UseE SONBuilder::readVarRecursive(VarID id, RegionN *region) {
    auto *context = region->Payload<BuildContext>();
    UseE val = nullptr;

    if (!context->is_sealed) {
        auto phi = sea.alloc<PhiN>(region);
        val = phi;
        context->incomplete_phi.insert(pair(id, phi));
    } else if (region->size() == 1) {
        auto pred = region->front();
        while (pred->getOp() != Nop::Region) {
            pred = pred->front();
        }
        val = readVar(id, (RegionN *) pred);
    } else {
        auto phi = sea.alloc<PhiN>(region);
        context->writeVar(id, phi);
        val = addPhiOperands(id, phi);
    }

    context->writeVar(id, val);
    return val;
}

UseE SONBuilder::addPhiOperands(VarID id, PhiN *phi) {
    for (auto pred: *phi->at(0)) {
        while (pred->getOp() != Nop::Region) {
            pred = pred->at(0);
        }
        phi->push_back(readVar(id, (RegionN *) pred));
    }

    // return tryRemoveTrivialPhi(phi);
    return phi;
}


void SONBuilder::sealBlock(RegionN *block) {
    auto *context = block->Payload<BuildContext>();
    for (auto &p: context->incomplete_phi) {
        addPhiOperands(p.first, p.second);
    }
    context->is_sealed = true;
}


void SONBuilder::buildTable() {
    array_offsets.clear();
    const auto &terms = curr_table->getVarInScope();
    int offset = 36;
    for (const auto &term: terms) {
        if (term.type.isArray()) {
            auto sz = term.type.sizeOf();
            if (sz % 4 != 0) {
                sz += 4 - sz % 4;
            }

            array_offsets.insert(pair(term.id, offset));
            offset += sz;
        }
    }
}

void C0::SONBuilder::visit(C0::EmptyStmt *e) {}

/// useless, wait for extension
void C0::SONBuilder::visit(C0::WhileStmt *e) {}

void C0::SONBuilder::visit(C0::CaseStmt *e) {}

void C0::SONBuilder::visit(C0::SwitchStmt *e) {}


} // end namespace C0
