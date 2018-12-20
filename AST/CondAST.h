#pragma once
#include "ASTBase.h"
#include "common.h"
#include "ExprAST.h"

namespace C0 {


class CondAST :
    public ASTBase
{
public:
    CondAST(Pos pos, unique_ptr<ExprAST>&& lhs, Cmp cp, unique_ptr<ExprAST>&& rhs)
        :ASTBase(pos), lhs(std::move(lhs)), cp(cp), rhs(std::move(rhs)){}

    explicit CondAST(Pos pos, unique_ptr<ExprAST>&& exp)
        :ASTBase(pos), lhs(std::move(exp)), cp(Cmp::UnEqual), rhs(make_unique<IntExpr>(lhs->getPos(), 0)) {}
    unique_ptr<ExprAST> lhs;
    Cmp cp;
    unique_ptr<ExprAST> rhs;
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    ~CondAST() override = default;

};




}

