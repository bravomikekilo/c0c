#pragma once

#include "ASTVisitor.h"
#include "../common.h"
#include "SymTable.h"

namespace C0 {

using std::tuple;

class ASTDrawer :
        public ASTVisitor {
public:
    using EdgeType = std::tuple<int, int, string>;

    static string dot(ASTBase &ast);
    static string drawProgram(vector<shared_ptr<FuncAST>> &funcs);

    ASTDrawer()
            : currNodeID(0), nodes(vector<string>()), edges(vector<EdgeType>()) {};

    string genDot() const;

    ~ASTDrawer() override = default;

    void visit(CallExpr *e) override;

    void visit(PrintStmt *e) override;

    void visit(ReadStmt *e) override;

private:
    int popNode();

    int currNodeID = 0;
    int subID = -1;
    vector<string> nodes;
    vector<EdgeType> edges;
    shared_ptr<SymTable> table;


    // Inherited via ASTVisitor
    void visit(IntExpr *e) override;

    void visit(CharExpr *e) override;

    void visit(OpExpr *e) override;

    void visit(CondAST *e) override;

    void visit(VarExpr *e) override;

    void visit(IfStmt *e) override;

    void visit(BlockStmt *e) override;

    void visit(WhileStmt *e) override;

    void visit(SwitchStmt *e) override;

    void visit(AsStmt *e) override;

    void visit(ExprStmt *e) override;

    void visit(RetStmt *e) override;

    void visit(EmptyStmt *e) override;

    void visit(CaseStmt *e) override;

    void visit(DoStmt *e) override;

    void visit(ForStmt *e) override;

    void visit(FuncAST *e) override;

};

}
