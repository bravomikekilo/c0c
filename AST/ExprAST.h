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
    virtual Type outType(shared_ptr<SymTable> table) = 0;
};

class IntExpr: public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    explicit IntExpr(int v) : v(v) {}

    Type outType(shared_ptr<SymTable> table) override {
        return Type(BaseTypeK::Int);
    };

    int v;
};

class CharExpr: public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    explicit CharExpr(char v) : v(v) {};

    Type outType(shared_ptr<SymTable> table) override {
        return Type(BaseTypeK::Char);
    }

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

    Type outType(shared_ptr<SymTable> table) override {
        if(op != C0::Op::Ind) {
            return Type(BaseTypeK::Int);
        } else {
            if(lhs->outType(table).isArray()) {
                return lhs->outType(table).getBase();
            } else {
                return Type(BaseTypeK::Int);
            }
        }
    }

};



class VarExpr : public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    explicit VarExpr(VarID id):varID(id) { }
    VarID varID;

    Type outType(shared_ptr<SymTable> table) override {
        auto *term = table->findVarByID(varID);
        if(term == nullptr) {
            return Type(BaseTypeK::Error);
        } else {
            return term->type;
        }
    }
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

    Type outType(shared_ptr<SymTable> table) override;

};



} // end namespace C0
