//
// Created by baomingkun on 18-11-23.
//

#include <AST/AST.h>
#include "SSABuilder.h"

namespace C0 {


void SSABuilder::visit(IntExpr *e) {
    expr_val = arena.alloc<GetIntV>(curr_region, e->v);
}

void SSABuilder::visit(CharExpr *e) {
    expr_val = arena.alloc<GetIntV>(curr_region, e->v);
}

void SSABuilder::visit(VarExpr *e) {
    if(value_map.count(e->varID) == 0) {
        expr_val = arena.alloc<UndefV>(curr_region);
    } else {
        expr_val = value_map[e->varID];
    }
}

void SSABuilder::visit(OpExpr *e) {
    e->lhs->accept(*this);
    auto lhs = expr_val;

    e->rhs->accept(*this);
    auto rhs = expr_val;

    switch (e->op) {
        case Op::Ind:
            if(e->lhs->outType(curr_table).getBase().is(BaseTypeK::Char)) {
                expr_val = arena.alloc<GetCharV>(curr_region, lhs, rhs);
            } else {
                expr_val = arena.alloc<GetIntV>(curr_region, lhs, rhs);
            }
            return;
        case Op::Add:
            expr_val = arena.alloc<AddV>(curr_region, lhs, rhs);
            return;
        case Op::Sub:
            expr_val = arena.alloc<SubV>(curr_region, lhs, rhs);
            break;
        case Op::Mul:
            expr_val = arena.alloc<MulV>(curr_region, lhs, rhs);
            break;
        case Op::Div:
            expr_val = arena.alloc<DivV>(curr_region, lhs, rhs);
            break;
    }

}

void SSABuilder::visit(CallExpr *e) {

}


void SSABuilder::visit(CondAST *e) {
    e->lhs->accept(*this);
    auto lhs = expr_val;

    e->rhs->accept(*this);
    auto rhs = expr_val;

    switch (e->cp) {
        case Cmp::Greater:
            arena.alloc<GtV>(curr_region, lhs, rhs);
            break;
        case Cmp::Less:
            arena.alloc<LtV>(curr_region, lhs, rhs);
            break;
        case Cmp::GreaterEq:
            arena.alloc<GeV>(curr_region, lhs, rhs);
            break;
        case Cmp::LessEq:
            arena.alloc<LeV>(curr_region, lhs, rhs);
            break;
        case Cmp::Equal:
            arena.alloc<EqV>(curr_region, lhs, rhs);
            break;
        case Cmp::UnEqual:
            arena.alloc<UnEqV>(curr_region, lhs, rhs);
            break;
    }

}


void SSABuilder::visit(IfStmt *e) {

}

void SSABuilder::visit(BlockStmt *e) {

}

void SSABuilder::visit(AsStmt *e) {

}

void SSABuilder::visit(ExprStmt *e) {
    e->exp->accept(*this);
}

void SSABuilder::visit(RetStmt *e) {

}

void SSABuilder::visit(EmptyStmt *e) {}

void SSABuilder::visit(DoStmt *e) {

}

void SSABuilder::visit(ForStmt *e) {

}

void SSABuilder::visit(PrintStmt *e) {

}

void SSABuilder::visit(ReadStmt *e) {

}

void SSABuilder::visit(FuncAST *e) {
    curr_func = e;
    curr_table = e->table;

    for(auto &stmt: e->stmts) {
        stmt->accept(*this);
    }

}


// useless visitor method, for future extension
void SSABuilder::visit(CaseStmt *e) {}
void SSABuilder::visit(WhileStmt *e) {}
void SSABuilder::visit(SwitchStmt *e) {}

}
