#pragma once
#include "ASTBase.h"
#include "CondAST.h"
#include "ExprAST.h"
#include "common.h"
#include <variant>

namespace C0 {


class StmtAST :
    public ASTBase
{
public:
    ~StmtAST() override = default;
};

class IfStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor & visitor) override;
    IfStmt(unique_ptr<CondAST>&& cond, unique_ptr<StmtAST>&& body)
        :cond(std::move(cond)),
         trueBranch(std::move(body)),
         falseBranch({}) {}

    IfStmt(unique_ptr<CondAST>&& cond, unique_ptr<StmtAST>&& trueBranch, unique_ptr<StmtAST>&& falseBranch)
        :cond(std::move(cond)),
         trueBranch(std::move(trueBranch)),
         falseBranch(optional<unique_ptr<StmtAST>>(std::move(falseBranch))) {}

    unique_ptr<CondAST> cond;
    unique_ptr<StmtAST> trueBranch;
    optional<unique_ptr<StmtAST>> falseBranch;
};

class BlockStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor & visitor) override;
    explicit BlockStmt(vector<unique_ptr<StmtAST>>&& stmts)
        :stmts(std::move(stmts)) {}

    vector<unique_ptr<StmtAST>> stmts;
};

class DoStmt: public StmtAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    DoStmt(unique_ptr<StmtAST>&& body, unique_ptr<CondAST>&& cond)
        :body(std::move(body)), cond(std::move(cond)) {};

    unique_ptr<StmtAST> body;
    unique_ptr<CondAST> cond;

};

class ForStmt: public StmtAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    explicit ForStmt(unique_ptr<CondAST>&& cond)
        :cond(std::move(cond)), start({}), after({}) {};

    explicit ForStmt(unique_ptr<AsStmt>&& start, unique_ptr<CondAST>&& cond)
        :cond(std::move(cond)), start(std::move(start)), after({}) {};

    explicit ForStmt(unique_ptr<AsStmt>&& start, unique_ptr<CondAST>&& cond, unique_ptr<AsStmt>&& after)
        :cond(std::move(cond)), start(std::move(start)), after(std::move(after)) {};

    explicit ForStmt(
            optional<unique_ptr<AsStmt>>&& start,
            unique_ptr<CondAST>&& cond,
            optional<unique_ptr<AsStmt>>&& after
            )
        :cond(std::move(cond)), start(std::move(start)), after(std::move(after)) {};


    optional<unique_ptr<AsStmt>> start;
    unique_ptr<CondAST> cond;
    optional<unique_ptr<AsStmt>> after;
};

class WhileStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor & visitor) override;
    WhileStmt(unique_ptr<CondAST>&& cond, unique_ptr<StmtAST>&& body)
        :cond(std::move(cond)), body(std::move(body)) {}
    
    unique_ptr<CondAST> cond;
    unique_ptr<StmtAST> body;

};

class CaseStmt : public ASTBase {
public:
    // Inherited via ASTBase
    void accept(ASTVisitor & visitor) override;
    explicit CaseStmt(int cond, unique_ptr<StmtAST>&& branch)
        :cond(cond), branch(std::move(branch)) {}

    explicit CaseStmt(char cond, unique_ptr<StmtAST>&& branch)
        :cond(cond), branch(std::move(branch)) {}

    CaseStmt(std::variant<char, int> cond, unique_ptr<StmtAST>&& branch)
        :cond(std::move(cond)), branch(std::move(branch)) {}

    std::variant<char, int> cond;
    unique_ptr<StmtAST> branch;
};

class SwitchStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor & visitor) override;
    SwitchStmt(unique_ptr<ExprAST>&& exp, vector<unique_ptr<CaseStmt>>&& cases)
        :exp(std::move(exp)), cases(std::move(cases)) {}

    unique_ptr<ExprAST> exp;
    vector<unique_ptr<CaseStmt>> cases;
};

class AsStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor & visitor) override;
    AsStmt(unique_ptr<ExprAST>&& lhs, unique_ptr<ExprAST>&& rhs)
        :lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    unique_ptr<ExprAST> lhs;
    unique_ptr<ExprAST> rhs;
};

class ExprStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor & visitor) override;
    explicit ExprStmt(unique_ptr<ExprAST> exp) : exp(std::move(exp)) {}
    unique_ptr<ExprAST> exp;
};

class RetStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor & visitor) override;
    RetStmt() : ret({}) {}
    explicit RetStmt(unique_ptr<ExprAST> ret) : ret(std::move(ret)) {}
    optional<unique_ptr<ExprAST>> ret;
};

class EmptyStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor & visitor) override;
    EmptyStmt() = default;
};


}
