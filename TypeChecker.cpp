//
// Created by baomingkun on 18-11-20.
//

#include "TypeChecker.h"

#define FMT_HEADER_ONLY

#include "fmt/format.h"
#include "util.h"

void C0::TypeChecker::visit(C0::IntExpr *e) {}

void C0::TypeChecker::visit(C0::CharExpr *e) {}

void C0::TypeChecker::visit(C0::VarExpr *e) {}

// check type of operator
void C0::TypeChecker::visit(C0::OpExpr *e) {
    e->lhs->accept(*this);
    e->rhs->accept(*this);

    auto left_type = e->lhs->outType(curr_table);
    auto right_type = e->rhs->outType(curr_table);

    if (e->op == C0::Op::Ind) {
        if (!left_type.isArray() && !left_type.isError()) {
            addError(e->lhs->getPos(), "left hand side of index is not array");
        }

        if (right_type.isError()) return;


        if (right_type.isArray()) {
            addError(e->rhs->getPos(), "array can't be right hand side of index");
        }

        if (right_type.is(BaseTypeK::Void)) {
            addError(e->rhs->getPos(), "void can't be right hand side of index");
        }

        auto rconst = e->rhs->constEval(*curr_table);
        if (rconst.has_value() && (rconst.value() >= left_type.getLength() || rconst.value() < 0)) {
            addError(e->getPos(),
                     fmt::format("array index out of bound, index {} with index {}",
                             left_type.toString(), rconst.value()
                     )
            );
        }

        return;
    }

    if (e->op == C0::Op::Div) {
        auto rconst = e->rhs->constEval(*curr_table);
        if (rconst.has_value() && rconst.value() == 0) {
            addError(e->getPos(), "div by const zero");
        }
    }

    if (left_type.isArray()) {
        addError(
                e->lhs->getPos(),
                fmt::format("array can't be left hand side of {}", opToString(e->op))
        );
    }

    if (right_type.isArray()) {
        addError(
                e->rhs->getPos(),
                fmt::format("array can't be right hand side of {}", opToString(e->op)));
    }


    if (left_type.is(BaseTypeK::Void)) {
        addError(
                e->lhs->getPos(),
                fmt::format("void can't be left hand side of {}", opToString(e->op)));
    }

    if (right_type.is(BaseTypeK::Void)) {
        addError(
                e->rhs->getPos(),
                fmt::format("void can't be right hand side of {}", opToString(e->op))
        );
    }


}

// check function argument type matching
void C0::TypeChecker::visit(C0::CallExpr *e) {
    auto func_p = curr_table->findFunc(e->name);
    if (!func_p.has_value()) {
        addError(e->getPos(), "call undefined function:" + e->name);
        return;
    }

    auto func = func_p.value();

    auto func_arg_sz = func->args.size();
    auto call_arg_sz = e->args.size();

    if (call_arg_sz < func_arg_sz) {
        addError(
                e->getPos(),
                fmt::format("too few argument for function {}", e->name)
        );
        return;
    }

    if (call_arg_sz > func_arg_sz) {
        addError(
                e->getPos(),
                fmt::format("too many argument for function {}", e->name)
        );
        return;
    }

    for (int i = 0; i < func_arg_sz; ++i) {
        auto &f = func->args[i];
        auto &c = e->args[i];
        auto call_type = e->args[i]->outType(curr_table);
        if (f.first != call_type) {
            addError(
                    e->args[i]->getPos(),
                    fmt::format("incompatible type of argument {} in {}, expected {}, but get {}",
                                f.second, e->name, f.first.toString(), call_type.toString())
            );
        }
    }

}

// check if array is used in compare
void C0::TypeChecker::visit(C0::CondAST *e) {
    auto left_type = e->lhs->outType(curr_table);
    auto right_type = e->rhs->outType(curr_table);

    if (!left_type.isError()) {
        if (left_type.isArray()) {
            addError(e->lhs->getPos(), "array can't be left hand side of cond");
        }

        if (left_type.is(BaseTypeK::Void)) {
            addError(e->lhs->getPos(), "void can't be left hand side of cond");
        }

    }

    if (!right_type.isError()) {
        if (right_type.isArray()) {
            addError(e->rhs->getPos(), "array can't be right hand side of cond");
        }

        if (right_type.is(BaseTypeK::Void)) {
            addError(e->rhs->getPos(), "void can't be right hand side of cond");
        }

    }

}

void C0::TypeChecker::visit(C0::CaseStmt *e) {}


void C0::TypeChecker::visit(C0::IfStmt *e) {
    e->cond->accept(*this);
    e->trueBranch->accept(*this);
    if (e->falseBranch.has_value()) {
        e->falseBranch.value()->accept(*this);
    }
}

void C0::TypeChecker::visit(C0::BlockStmt *e) {
    for (auto &stmt: e->stmts) {
        stmt->accept(*this);
    }
}

void C0::TypeChecker::visit(C0::WhileStmt *e) {
    e->cond->accept(*this);
    e->body->accept(*this);
}

void C0::TypeChecker::visit(C0::SwitchStmt *e) {}

void C0::TypeChecker::visit(C0::AsStmt *e) {
    e->lhs->accept(*this);
    e->rhs->accept(*this);

    // check if type is consistent

    if(!e->lhs->isLeftValue()) {
        addError(e->lhs->getPos(), "invalid left value");
    }

    auto left_type = e->lhs->outType(curr_table);
    auto right_type = e->rhs->outType(curr_table);

    if (left_type.isError() || right_type.isError()) {
        return;
    }

    if (left_type != right_type) {
        addError(e->getPos(),
                 fmt::format(
                         "incompatible assign type, left {}, right {}",
                         left_type.toString(), right_type.toString()
                 )
        );
    }

    if (e->lhs->constEval(*curr_table).has_value()) {
        addError(e->getPos(), "can't assign to const value");
    }

}

void C0::TypeChecker::visit(C0::ExprStmt *e) {
    e->exp->accept(*this);
}

// check if return type is consistent with function return type
void C0::TypeChecker::visit(C0::RetStmt *e) {
    if (curr_func->retType.is(BaseTypeK::Void)) {
        if (e->ret.has_value()) {
            addError(e->getPos(), "void function can't have return value");
        }
        return;
    }

    auto ret_type = curr_func->retType;
    if (!e->ret.has_value()) {
        addError(e->getPos(), fmt::format(
                "return statement with no value, in function returning '{}'",
                ret_type.toString()
        ));
        return;
    }

    auto exp_type = e->ret.value()->outType(curr_table);
    if (exp_type.isError() || ret_type.isError()) {
        return;
    }

    if (exp_type != ret_type) {
        addError(e->getPos(), fmt::format(
                "return value type of {} not match function return type '{}'",
                exp_type.toString(),
                ret_type.toString()
        ));
    }

}

void C0::TypeChecker::visit(C0::EmptyStmt *e) {}

void C0::TypeChecker::visit(C0::DoStmt *e) {
    e->body->accept(*this);
    e->cond->accept(*this);
}

void C0::TypeChecker::visit(C0::ForStmt *e) {

    if (e->start.has_value()) {
        e->start.value()->accept(*this);
    }

    e->cond->accept(*this);

    if (e->after.has_value()) {
        e->after.value()->accept(*this);
    }

    e->body->accept(*this);

}

// load function and symbol table
void C0::TypeChecker::visit(C0::FuncAST *e) {
    curr_func = e;
    curr_table = e->table;
    for (auto &stmt: e->stmts) {
        stmt->accept(*this);
    }
}

void C0::TypeChecker::visit(PrintStmt *e) {
    if (!e->expr.has_value()) {
        return;
    }

    e->expr.value()->accept(*this);

    auto expr_type = e->expr.value()->outType(curr_table);
    if (expr_type.isArray()) {
        addError(e->expr.value()->getPos(), "can't print array");
    }

    if (expr_type.is(BaseTypeK::Void)) {
        addError(e->expr.value()->getPos(), "can't print void");
    }


}

void C0::TypeChecker::visit(ReadStmt *e) {
    for (auto &var: e->vars) {
        var->accept(*this);
        auto var_type = var->outType(curr_table);
        if (var_type.isError()) continue;
        if (var_type.isArray()) {
            addError(var->getPos(), "can't read array type");
            continue;
        }

        if (var_type.is(BaseTypeK::Void)) {
            addError(var->getPos(), "can't read void type");
            continue;
        }
    }
}
