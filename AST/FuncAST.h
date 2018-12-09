//
// Created by baomingkun on 18-11-17.
//

#ifndef C0_FUNCAST_H
#define C0_FUNCAST_H

#include "common.h"
#include "ASTBase.h"
#include "Type.h"
#include "SymTable.h"
#include "StmtAST.h"


namespace C0 {

class FuncAST : public ASTBase {
public:
    Type retType;
    string name;
    vector<pair<Type, string>> args;
    shared_ptr<SymTable> table;
    vector<unique_ptr<StmtAST>> stmts;

    FuncAST(shared_ptr<SymTable> table, string name,
            Type ret, vector<pair<Type, string>>&& args,
            vector<unique_ptr<StmtAST>>&& stmts)
        :table(std::move(table)), name(std::move(name)) , retType(std::move(ret))
        , args(std::move(args)), stmts(std::move(stmts)) {}

   FuncAST(string name, Type ret, vector<pair<Type, string>> &args)
        :table(nullptr), name(std::move(name)) , retType(std::move(ret))
        , args(std::move(args)), stmts(vector<unique_ptr<StmtAST>>()) {}

    void accept(ASTVisitor &visitor) override {
        visitor.visit(this);
    }
};



}


#endif //C0_FUNCAST_H
