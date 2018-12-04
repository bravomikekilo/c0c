//
// Created by baomingkun on 18-11-23.
//

#ifndef C0_SSABUILDER_H
#define C0_SSABUILDER_H

#include "common.h"
#include "AST/ASTVisitor.h"
#include "AST/AST.h"
#include "SSA.h"
#include "NodeArena.h"


namespace C0 {

class BuildContext {
    unordered_map<VarID, ValueE> table;
    bool filled = false;
    bool sealed = false;

public:

    bool hasVar(VarID id) { return table.count(id) == 1; }

    ValueE getVar(VarID id) {
        if(table.count(id) == 0) {
            return nullptr;
        } else {
            return table[id];
        }
    }

    void setVar(VarID id, ValueE val) {
        table[id] = val;
    }

};

class SSABuilder : public ASTVisitor {
public:

    explicit SSABuilder(NodeArena &arena): arena(arena) {}

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

    void visit(PrintStmt *e) override;

    void visit(ReadStmt *e) override;

    void visit(FuncAST *e) override;

private:
    shared_ptr<SymTable> curr_table = nullptr;
    FuncAST *curr_func = nullptr;
    ValueE expr_val = nullptr;
    Region *curr_region = nullptr;
    NodeArena &arena;

    unordered_map<VarID , ValueE> value_map;

    void setVar(VarID id, ValueE val) {
        curr_region->Payload<BuildContext>()->setVar(id, val);
    }

    ValueE getVar(Region *region, VarID id) {
        auto var = region->Payload<BuildContext>()->getVar(id);
        if(var != nullptr) return var;

        return getVarRecursive(region, id);

    }

    ValueE getVarRecursive(Region *region, VarID id) {
        // handle incomplete CFG

        if(region->num_cin() == 1) {
            // upward find a region node
            // return getVar(region->pred.front(), id);
        }

    }

};

}


#endif //C0_SSABUILDER_H
