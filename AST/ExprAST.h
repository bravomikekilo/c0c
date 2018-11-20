#pragma once

#include "../common.h"
#include "../c0.h"
#include "ASTBase.h"
#include "Type.h"
#include "SymTable.h"


namespace C0 {

class ExprAST: public ASTBase {
public:
    ~ExprAST() override = default;
};

class IntExpr: public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    explicit IntExpr(int v) : v(v) {};
    int v;
};

class CharExpr: public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    explicit CharExpr(char v) : v(v) {};
    char v;
};

class OpExpr: public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    OpExpr(Op op, unique_ptr<ExprAST>&& lhs, unique_ptr<ExprAST>&& rhs)
        :op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {};
    Op op;
    unique_ptr<ExprAST> lhs;
    unique_ptr<ExprAST> rhs;
};



class VarExpr : public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    explicit VarExpr(VarID id):varID(id) { }
    VarID varID;
};

class CallExpr : public  ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    CallExpr(string name, vector<unique_ptr<ExprAST>>&& args)
        :name(std::move(name)), args(std::move(args)) {};

    string name;
    vector<unique_ptr<ExprAST>> args;

};



class PrintExpr : public ExprAST {
public:
    optional<int> str;
    optional<unique_ptr<ExprAST>> expr;

    PrintExpr(optional<int> str, optional<unique_ptr<ExprAST>>&& expr)
        :str(std::move(str)), expr(std::move(expr)){}

    void accept(ASTVisitor &visitor) override {
        return visitor.visit(this);
    }

};


class ReadExpr : public ExprAST {
public:
    vector<unique_ptr<VarExpr>> vars;
    explicit ReadExpr(vector<unique_ptr<VarExpr>>&& vars)
        : vars(std::move(vars)) {}

    void accept(ASTVisitor &visitor) override {
        return visitor.visit(this);
    }


};




} // end namespace C0
