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
            errors.emplace_back("left hand side of index is not array");
        }

        if (right_type.isError()) return;

        if (right_type.is(BaseTypeK::Char) || right_type.is(BaseTypeK::Int)) return;

        if (right_type.isArray()) {
            errors.emplace_back("array can't be right hand side of index");
        }

        if (right_type.is(BaseTypeK::Void)) {
            errors.emplace_back("void can't be right hand side of index");
        }

        return;
    }

    if (left_type.isArray()) {
        errors.push_back(
                fmt::format("array can't be left hand side of {}", opToString(e->op)));
    }

    if (right_type.isArray()) {
        errors.push_back(
                fmt::format("array can't be right hand side of {}", opToString(e->op)));
    }


    if (left_type.is(BaseTypeK::Void)) {
        errors.push_back(
                fmt::format("void can't be left hand side of {}", opToString(e->op)));
    }

    if (right_type.is(BaseTypeK::Void)) {
        errors.push_back(
                fmt::format("void can't be right hand side of {}", opToString(e->op))
        );
    }


}

// check function argument type matching
void C0::TypeChecker::visit(C0::CallExpr *e) {
    auto func_p = curr_table->findFunc(e->name);
    if (!func_p.has_value()) {
        errors.emplace_back("call undefined function");
        return;
    }

    auto func = func_p.value();

    auto func_arg_sz = func->args.size();
    auto call_arg_sz = e->args.size();

    if (call_arg_sz < func_arg_sz) {
        errors.push_back(
                fmt::format("too few argument for {} function", e->name)
        );
        return;
    }

    if (call_arg_sz > func_arg_sz) {
        errors.push_back(
                fmt::format("too many argument for {} function", e->name)
        );
        return;
    }

    for (int i = 0; i < func_arg_sz; ++i) {
        auto &f = func->args[i];
        auto &c = e->args[i];
        auto call_type = e->args[i]->outType(curr_table);
        if (f.first != call_type) {
            errors.push_back(
                    fmt::format("incompatible type of {} and {} in {} call",
                                f.first.toString(), call_type.toString(), e->name)
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
            errors.emplace_back("array can't be left hand side of cond");
        }

        if (left_type.is(BaseTypeK::Void)) {
            errors.emplace_back ("void can't be left hand side of cond");
        }

    }

    if(!right_type.isError()) {
        if (right_type.isArray()) {
            errors.emplace_back("array can't be right hand side of cond");
        }

        if (right_type.is(BaseTypeK::Void)) {
            errors.emplace_back ("void can't be right hand side of cond");
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

    auto left_type = e->lhs->outType(curr_table);
    auto right_type = e->rhs->outType(curr_table);

    if (left_type.isError() || right_type.isError()) {
        return;
    }

    if (left_type != right_type) {
        errors.emplace_back("inconsistent assign type");
    }

}

void C0::TypeChecker::visit(C0::ExprStmt *e) {
    e->exp->accept(*this);
}

// check if return type is consistent with function return type
void C0::TypeChecker::visit(C0::RetStmt *e) {
    if(curr_func->retType.is(BaseTypeK::Void)) {
        if(e->ret.has_value()) {
            errors.emplace_back("void function can't have return value");
        }
        return;
    }

    auto ret_type = curr_func->retType;
    if(!e->ret.has_value()) {
        errors.push_back(fmt::format(
                "return statement with no value, in function returning '{}'",
                ret_type.toString()
                ));
        return;
    }

    auto exp_type = e->ret.value()->outType(curr_table);
    if(exp_type.isError() || ret_type.isError()) {
        return;
    }

    if(exp_type != ret_type) {
        errors.push_back(fmt::format(
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


}

// load function and symbol table
void C0::TypeChecker::visit(C0::FuncAST *e) {
    curr_func = e;
    curr_table = e->table;
    for(auto &stmt: e->stmts) {
        stmt->accept(*this);
    }
}

// check type of print argument, array argument is not allowed
void C0::TypeChecker::visit(C0::PrintExpr *e) {
    if(!e->expr.has_value()) {
        return;
    }

    auto expr_type = e->expr.value()->outType(curr_table);
    if(expr_type.isArray()) {
        errors.emplace_back("can't print array");
    }

    if(expr_type.is(BaseTypeK::Void)) {
        errors.emplace_back("can't print void");
    }

}

// check type of read argument, array argument is not allowed
void C0::TypeChecker::visit(C0::ReadExpr *e) {
    for(auto &var: e->vars) {
        auto var_type = var->outType(curr_table);
        if(var_type.isError()) continue;
        if(var_type.isArray()) {
            errors.emplace_back("can't read array type");
            continue;
        }

        if(var_type.is(BaseTypeK::Void)) {
            errors.emplace_back("can't read void type");
            continue;
        }
    }
}
