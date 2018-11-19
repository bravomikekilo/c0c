//
// Created by baomingkun on 18-11-18.
//

#include "CFGConverter.h"


namespace C0 {


void CFGConverter::visit(IntExpr *e) {
    expr_is_leaf = true;
    leaf_val = QuadVal(e->v, true);
}

void CFGConverter::visit(CharExpr *e) {
    expr_is_leaf = true;
    leaf_val = QuadVal(e->v, true);
}

void CFGConverter::visit(VarExpr *e) {
    if(on_left_side) {
        left_is_var = true;
        left_var = QuadVal(e->varID, false);
        // auto result_reg = QuadVal(e->varID, false);
        // curr_block->insts.back().dst = result_reg;
        return;
    }
    expr_is_leaf = true;
    leaf_val = QuadVal(e->varID, false);
}


void CFGConverter::visit(OpExpr *e) {
    auto op = e->op;
    QuadOp qop;
    switch (op) {
        case Op::Ind:
            qop = QuadOp::GetInt; // TODO: get array type
            break;
        case Op::Add:
            qop = QuadOp::Add;
            break;
        case Op::Sub:
            qop = QuadOp::Sub;
            break;
        case Op::Mul:
            qop = QuadOp::Mul;
            break;
        case Op::Div:
            qop = QuadOp::Div;
            break;
    }

    if(on_left_side) {
        on_left_side = false;
        QuadVal i;
        e->rhs->accept(*this);
        if(expr_is_leaf) {
            expr_is_leaf = false;
            i = leaf_val;
        } else {
            i = popTempReg();
        }

        e->lhs->accept(*this);
        expr_is_leaf = false;
        auto arr = leaf_val;

        curr_block->insts.emplace_back(arr, qop, i, right_val);
        return;
    }

    // set left hand side expression
    auto temp_reg = peekTempReg();
    auto lhs_reg = QuadVal(temp_reg);
    e->lhs->accept(*this);
    if (expr_is_leaf) {
        expr_is_leaf = false;
        lhs_reg = leaf_val;
    } else {
        lastInst().dst = lhs_reg;
        next_temp_reg = --temp_reg;
    }


    // set right hand side expression
    auto rhs_reg = QuadVal(temp_reg);
    e->rhs->accept(*this);
    if (expr_is_leaf) {
        expr_is_leaf = false;
        rhs_reg = leaf_val;
    } else {
        lastInst().dst = rhs_reg;
        next_temp_reg = --temp_reg;
    }

    // insert own expression
    curr_block->insts.emplace_back(QuadVal(), qop, lhs_reg, rhs_reg);


}

void CFGConverter::visit(CallExpr *e) {
    vector<QuadVal> args;
    auto temp_reg = peekTempReg();
    for (auto &arg :e->args) {
        arg->accept(*this);
        if (expr_is_leaf) {
            expr_is_leaf = false;
            args.push_back(leaf_val);
        } else {
            auto arg_reg = QuadVal(temp_reg);
            lastInst().dst = arg_reg;
            next_temp_reg = --temp_reg;
            args.push_back(arg_reg);
        }
    }

    curr_block->insts.emplace_back(e->name, std::move(args), QuadVal());

}

static QuadOp convertCmp(C0::Cmp cmp) {
    switch (cmp) {
        case Cmp::Greater:
            return QuadOp::Bgt;
        case Cmp::Less:
            return QuadOp::Blt;
        case Cmp::GreaterEq:
            return QuadOp::Bge;
        case Cmp::LessEq:
            return QuadOp::Ble;
        case Cmp::Equal:
            return QuadOp::Beq;
        case Cmp::UnEqual:
            return QuadOp::Bne;
    }
}

void CFGConverter::visit(CondAST *e) {

    // auto lhs_reg = QuadVal(-1);
    auto temp_reg = peekTempReg();
    auto lhs_reg = QuadVal(temp_reg);
    e->lhs->accept(*this);
    if (expr_is_leaf) {
        expr_is_leaf = false;
        lhs_reg = leaf_val;
    } else {
        lastInst().dst = lhs_reg;
        next_temp_reg = --temp_reg;
    }

    auto rhs_reg = QuadVal(temp_reg);
    e->rhs->accept(*this);
    if (expr_is_leaf) {
        expr_is_leaf = false;
        rhs_reg = leaf_val;
    } else {
        lastInst().dst = rhs_reg;
        next_temp_reg = --temp_reg;
    }
    auto cond = convertCmp(e->cp);
    curr_block->insts.emplace_back(cond, lhs_reg, rhs_reg, nullptr);

}

void CFGConverter::visit(IfStmt *e) {
    e->cond->accept(*this); // generate condition instruction
    auto *main_branch = &(curr_block->insts.back().jmp);
    if (e->falseBranch.has_value()) {
        auto head_block = curr_block;
        auto true_head_block = builder.create();
        *main_branch = true_head_block;

        curr_block = true_head_block;
        e->trueBranch->accept(*this);
        vector<BasicBlock **> true_waiting;
        addWaiting(&curr_block->next);
        std::swap(true_waiting, wait_for_next_block);

        auto false_head_block = builder.create();
        head_block->next = false_head_block;

        curr_block = false_head_block;
        e->falseBranch.value()->accept(*this);
        addWaiting(&curr_block->next);
        wait_for_next_block.insert(wait_for_next_block.end(), true_waiting.begin(), true_waiting.end());

    } else {
        auto head_block = curr_block;
        curr_block = builder.create();
        *main_branch = curr_block;
        e->trueBranch->accept(*this);
        addWaiting(&curr_block->next);

        addWaiting(&head_block->next);
    }
}

void CFGConverter::visit(BlockStmt *e) {
    for (auto &stmt: e->stmts) {
        if (hasWaiting()) {
            curr_block = builder.create();
            cleanWaiting(curr_block);
        }
        stmt->accept(*this);
    }
}

void CFGConverter::visit(AsStmt *e) {

    e->rhs->accept(*this);
    if(expr_is_leaf) {
        expr_is_leaf = false;
        right_val = leaf_val;
    } else {
        right_val = popTempReg();
        curr_block->insts.back().dst = right_val;
    }

    on_left_side = true;
    e->lhs->accept(*this);
    on_left_side = false;

    if(left_is_var) {
        left_is_var = false;
        if(right_val.isConst) {
            curr_block->insts.emplace_back(left_var, QuadOp::Copy, right_val, QuadVal());
        } else {
            curr_block->insts.back().dst = left_var;
        }
    }

    freeAllTempReg();
}

void CFGConverter::visit(ExprStmt *e) {
    e->exp->accept(*this);
    freeAllTempReg();
}

void CFGConverter::visit(FuncAST *e) {
    for (auto &stmt: e->stmts) {
        if (hasWaiting()) {
            curr_block = builder.create();
            cleanWaiting(curr_block);
        }
        stmt->accept(*this);
    }
    if (hasWaiting()) {
        curr_block = builder.create();
        curr_block->insts.emplace_back();
        cleanWaiting(curr_block);
    }
}

void CFGConverter::visit(ForStmt *e) {
    if (e->start.has_value()) {
        e->start.value()->accept(*this);
    }

    auto cond_block = builder.create();
    curr_block->next = cond_block;
    curr_block = cond_block;
    e->cond->accept(*this);

    auto body_block = builder.create();
    curr_block->insts.back().jmp = body_block;

    curr_block = body_block;
    e->body->accept(*this);

    if(e->after.has_value()) {
        if(curr_block == body_block) {
            e->after.value()->accept(*this);
            addWaiting(&curr_block->next);
        } else {
            auto after_block = builder.create();
            cleanWaiting(after_block);
            e->after.value()->accept(*this);
            addWaiting(&after_block->next);
        }
    }

    cleanWaiting(cond_block);
    addWaiting(&cond_block->next);

}

void CFGConverter::visit(DoStmt *e) {
    if (!curr_block->insts.empty()) {
        auto next_block = builder.create();
        if (curr_block->next == nullptr) {
            curr_block->next = next_block;
        }
        curr_block = next_block;
        cleanWaiting(next_block);
    }
    auto head_block = curr_block;

    e->body->accept(*this);
    if (hasWaiting()) {
        auto next_block = builder.create();
        curr_block->next = next_block;
        curr_block = next_block;
        cleanWaiting(curr_block);
    }
    e->cond->accept(*this);
    curr_block->insts.back().jmp = head_block;

    addWaiting(&curr_block->next);

}

void CFGConverter::visit(EmptyStmt *e) {}

void CFGConverter::visit(RetStmt *e) {
    if (e->ret.has_value()) {
        e->ret.value()->accept(*this);
        auto ret_val = QuadVal(-1);
        if (expr_is_leaf) {
            expr_is_leaf = false;
            ret_val = leaf_val;
        } else {
            lastInst().dst = ret_val;
        }
        curr_block->insts.emplace_back(ret_val);
    } else {
        curr_block->insts.emplace_back();
    }
}

}




