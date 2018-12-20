#pragma once

#include "ASTBase.h"
#include "CondAST.h"
#include "ExprAST.h"
#include "common.h"
#include <variant>

namespace C0 {


class StmtAST : public ASTBase {
public:
    StmtAST(Pos pos) : ASTBase(pos) {}

    ~StmtAST() override = default;
};

class IfStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor &visitor) override;

    IfStmt(Pos pos, unique_ptr<CondAST> &&cond, unique_ptr<StmtAST> &&body)
            : StmtAST(pos),
              cond(std::move(cond)),
              trueBranch(std::move(body)),
              falseBranch({}) {}

    IfStmt(Pos pos, unique_ptr<CondAST> &&cond, unique_ptr<StmtAST> &&trueBranch, unique_ptr<StmtAST> &&falseBranch)
            : StmtAST(pos),
              cond(std::move(cond)),
              trueBranch(std::move(trueBranch)),
              falseBranch(optional<unique_ptr<StmtAST>>(std::move(falseBranch))) {}

    unique_ptr<CondAST> cond;
    unique_ptr<StmtAST> trueBranch;
    optional<unique_ptr<StmtAST>> falseBranch;
};

class BlockStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor &visitor) override;

    explicit BlockStmt(Pos pos, vector<unique_ptr<StmtAST>> &&stmts)
            : StmtAST(pos), stmts(std::move(stmts)) {}

    vector<unique_ptr<StmtAST>> stmts;
};

class DoStmt : public StmtAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    DoStmt(Pos pos, unique_ptr<StmtAST> &&body, unique_ptr<CondAST> &&cond)
            : StmtAST(pos), body(std::move(body)), cond(std::move(cond)) {};

    unique_ptr<StmtAST> body;
    unique_ptr<CondAST> cond;

};

class ForStmt : public StmtAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    explicit ForStmt(Pos pos, unique_ptr<CondAST> &&cond, unique_ptr<StmtAST> &&body)
            : StmtAST(pos),
              cond(std::move(cond)), start({}), after({}), body(std::move(body)) {};

    explicit ForStmt(Pos pos, unique_ptr<AsStmt> &&start, unique_ptr<CondAST> &&cond, unique_ptr<StmtAST> &&body)
            : StmtAST(pos),
              cond(std::move(cond)), start(std::move(start)), after({}), body(std::move(body)) {};

    explicit ForStmt(Pos pos,
                     unique_ptr<AsStmt> &&start,
                     unique_ptr<CondAST> &&cond,
                     unique_ptr<AsStmt> &&after,
                     unique_ptr<StmtAST> &&body)
            : StmtAST(pos),
              cond(std::move(cond)),
              start(std::move(start)), after(std::move(after)), body(std::move(body)) {};


    explicit ForStmt(Pos pos,
                     optional<unique_ptr<AsStmt>> &&start,
                     unique_ptr<CondAST> &&cond,
                     optional<unique_ptr<AsStmt>> &&after,
                     unique_ptr<StmtAST> &&body)
            : StmtAST(pos),
              cond(std::move(cond)),
              start(std::move(start)), after(std::move(after)), body(std::move(body)) {};


    optional<unique_ptr<AsStmt>> start;
    unique_ptr<CondAST> cond;
    optional<unique_ptr<AsStmt>> after;
    unique_ptr<StmtAST> body;
};

class WhileStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor &visitor) override;

    WhileStmt(Pos pos, unique_ptr<CondAST> &&cond, unique_ptr<StmtAST> &&body)
            :StmtAST(pos), cond(std::move(cond)), body(std::move(body)) {}

    unique_ptr<CondAST> cond;
    unique_ptr<StmtAST> body;

};

class CaseStmt : public ASTBase {
public:
    // Inherited via ASTBase
    void accept(ASTVisitor &visitor) override;

    explicit CaseStmt(Pos pos, int cond, unique_ptr<StmtAST> &&branch)
            : ASTBase(pos), cond(cond), branch(std::move(branch)) {}

    explicit CaseStmt(Pos pos, char cond, unique_ptr<StmtAST> &&branch)
            : ASTBase(pos), cond(cond), branch(std::move(branch)) {}

    CaseStmt(Pos pos, std::variant<char, int> cond, unique_ptr<StmtAST> &&branch)
            : ASTBase(pos), cond(std::move(cond)), branch(std::move(branch)) {}

    std::variant<char, int> cond;
    unique_ptr<StmtAST> branch;
};

class SwitchStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor &visitor) override;

    SwitchStmt(Pos pos, unique_ptr<ExprAST> &&exp, vector<unique_ptr<CaseStmt>> &&cases)
            : StmtAST(pos), exp(std::move(exp)), cases(std::move(cases)) {}

    unique_ptr<ExprAST> exp;
    vector<unique_ptr<CaseStmt>> cases;
};

class AsStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor &visitor) override;

    AsStmt(Pos pos, unique_ptr<ExprAST> &&lhs, unique_ptr<ExprAST> &&rhs)
            : StmtAST(pos), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

    unique_ptr<ExprAST> lhs;
    unique_ptr<ExprAST> rhs;
};

class ExprStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor &visitor) override;

    ExprStmt(Pos pos, unique_ptr<ExprAST> exp) : StmtAST(pos), exp(std::move(exp)) {}

    unique_ptr<ExprAST> exp;
};

class RetStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor &visitor) override;

    RetStmt(Pos pos) : StmtAST(pos), ret({}) {}

    RetStmt(Pos pos, unique_ptr<ExprAST> ret)
            : StmtAST(pos), ret(std::move(ret)) {}

    optional<unique_ptr<ExprAST>> ret;
};

class EmptyStmt : public StmtAST {
    // Inherited via StmtAST
public:
    void accept(ASTVisitor &visitor) override;

    EmptyStmt(Pos pos) : StmtAST(pos) {};
};


class PrintStmt : public StmtAST {
public:
    optional<int> str;
    optional<unique_ptr<ExprAST>> expr;

    PrintStmt(Pos pos, optional<int> str, optional<unique_ptr<ExprAST>> &&expr)
            : StmtAST(pos), str(std::move(str)), expr(std::move(expr)) {}

    void accept(ASTVisitor &visitor) override {
        return visitor.visit(this);
    }


};


class ReadStmt : public StmtAST {
public:
    vector<unique_ptr<VarExpr>> vars;

    explicit ReadStmt(Pos pos, vector<unique_ptr<VarExpr>> &&vars)
            : StmtAST(pos), vars(std::move(vars)) {}

    void accept(ASTVisitor &visitor) override {
        return visitor.visit(this);
    }


};


}
