//
// Created by baomingkun on 18-11-20.
//

#ifndef C0_TYPECHECKER_H
#define C0_TYPECHECKER_H

#include "AST/AST.h"

namespace C0 {

class TypeChecker : public ASTVisitor {
public:
    void visit(IntExpr *e) override;

    void visit(CharExpr *e) override;

    void visit(VarExpr *e) override;

    void visit(OpExpr *e) override;

    void visit(CallExpr *e) override;

    void visit(CondAST *e) override;

    void visit(CaseStmt *e) override;

    void visit(IfStmt *e) override;

    void visit(BlockStmt *e) override;

    void visit(WhileStmt *e) override;

    void visit(SwitchStmt *e) override;

    void visit(AsStmt *e) override;

    void visit(ExprStmt *e) override;

    void visit(RetStmt *e) override;

    void visit(EmptyStmt *e) override;

    void visit(DoStmt *e) override;

    void visit(ForStmt *e) override;

    void visit(FuncAST *e) override;

    void visit(PrintExpr *e) override;

    void visit(ReadExpr *e) override;

private:
    vector<string> errors;
    shared_ptr<SymTable> curr_table;
    C0::FuncAST *curr_func = nullptr;

};

}

#endif //C0_TYPECHECKER_H
