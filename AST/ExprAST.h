#pragma once

#include "../common.h"
#include "../c0.h"
#include "ASTBase.h"
#include "Type.h"


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
    explicit VarExpr(string n, int id):name(std::move(n)), varID(id) { }
    string name;
    int varID;
    // int Type;
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




} // end namespace C0
