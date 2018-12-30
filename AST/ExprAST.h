#pragma once

#include "../common.h"
#include "../c0.h"
#include "ASTBase.h"
#include "Type.h"
#include "SymTable.h"


namespace C0 {

class ExprAST: public ASTBase {
public:
    ExprAST(Pos pos): ASTBase(pos) {}
    ~ExprAST() override = default;
    virtual Type outType(shared_ptr<SymTable> table) = 0;
    virtual bool isLeftValue() {return false;};
    virtual optional<int> constEval(const SymTable &table) = 0;
};

class IntExpr: public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    explicit IntExpr(Pos pos, int v) :ExprAST(pos), v(v) {}

    Type outType(shared_ptr<SymTable> table) override {
        return Type(BaseTypeK::Int);
    };

    optional<int> constEval(const SymTable &table) override {
        return {v};
    }

    int v;
};

class CharExpr: public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    explicit CharExpr(Pos pos, char v) :ExprAST(pos), v(v) {};

    Type outType(shared_ptr<SymTable> table) override {
        return Type(BaseTypeK::Char);
    }

    optional<int> constEval(const SymTable &table) override {
        return {v};
    }

    char v;
};

class OpExpr: public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    OpExpr(Pos pos, Op op, unique_ptr<ExprAST>&& lhs, unique_ptr<ExprAST>&& rhs)
        :ExprAST(pos), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {};
    Op op;
    unique_ptr<ExprAST> lhs;
    unique_ptr<ExprAST> rhs;

    Type outType(shared_ptr<SymTable> table) override {
        auto lt = lhs->outType(table);
        auto rt = rhs->outType(table);

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

    optional<int> constEval(const SymTable &table) override {
        auto vl = lhs->constEval(table);
        auto vr = rhs->constEval(table);
        if(vl.has_value() && vr.has_value()) {
            optional<int> ret;
            switch(op) {
                case Op::Add:
                    ret = {vl.value() + vr.value()};
                    break;
                case Op::Sub:
                    ret = {vl.value() - vr.value()};
                    break;
                case Op::Mul:
                    ret = {vl.value() * vr.value()};
                    break;
                case Op::Div:
                    ret = {vl.value() / vr.value()};
                    break;
                default:
                    ret = {};
                    break;
            }
            return ret;
        } else {
            return {};
        }
    }

    bool isLeftValue() override {
        return op == C0::Op::Ind;
    }

};



class VarExpr : public ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
    VarExpr(Pos pos, VarID id):ExprAST(pos), varID(id) { }
    VarID varID;

    Type outType(shared_ptr<SymTable> table) override {
        auto *term = table->findVarByID(varID);
        if(term == nullptr) {
            return Type(BaseTypeK::Error);
        } else {
            return term->type;
        }
    }

    optional<int> constEval(const SymTable &table) override {
        auto term = table.findVarByID(varID);
        if(term && term->isConst()) {
            return term->val.value();
        } else {
            return {};
        }
    }

    bool isLeftValue() override {
        return true;
    }
};

class CallExpr : public  ExprAST {
public:
    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    CallExpr(Pos pos, string name, vector<unique_ptr<ExprAST>>&& args)
        :ExprAST(pos), name(std::move(name)), args(std::move(args)) {};

    string name;
    vector<unique_ptr<ExprAST>> args;

    Type outType(shared_ptr<SymTable> table) override;

    optional<int> constEval(const SymTable &table) override {
        return {};
    }

};

class PareExpr : public ExprAST {
public:

    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }

    PareExpr(Pos pos, unique_ptr<ExprAST>&& exp):ExprAST(pos), exp(std::move(exp)) {}

    unique_ptr<ExprAST> exp;

    Type outType(shared_ptr<SymTable> table) override;

    optional<int> constEval(const SymTable &table) override {
        return exp->constEval(table);
    }
};



} // end namespace C0
