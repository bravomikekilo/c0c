#pragma once
#include "ASTBase.h"
#include "common.h"
#include "ExprAST.h"

namespace C0 {


class CondAST :
    public ASTBase
{
public:
    CondAST(unique_ptr<ExprAST>&& lhs, Cmp cp, unique_ptr<ExprAST>&& rhs)
        :lhs(std::move(lhs)), cp(cp), rhs(std::move(rhs)){}

    explicit CondAST(unique_ptr<ExprAST>&& exp)
        :lhs(std::move(exp)), cp(Cmp::UnEqual), rhs(make_unique<IntExpr>(0)) {}
    unique_ptr<ExprAST> lhs;
    Cmp cp;
    unique_ptr<ExprAST> rhs;
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    ~CondAST() override = default;

};




}

