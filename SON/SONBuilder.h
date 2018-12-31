//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_SEABUILDER_H
#define C0_SEABUILDER_H

#include "SON.h"
#include "Sea.h"
#include "common.h"
#include "AST/ASTVisitor.h"
#include "SymTable.h"
#include "NLoad.h"

namespace C0 {


class BuildContext : public NLoad {

public:
    unordered_map<VarID, UseE> value;
    UseE world;

    bool is_sealed;

    unordered_map<VarID, PhiN *> incomplete_phi;

    void writeVar(VarID id, UseE val) {
        if (id == 0) {
            world = val;
        } else {
            value[id] = val;
        }
    }

    string toStr() override {
        return "BuildContext";
    }

    ~BuildContext() override = default;
};


class SONBuilder : public ASTVisitor {
private:
    Sea &sea;

    vector<Node *> contexts;

    const unordered_map<VarID, int> &global_offsets;
    unordered_map<VarID, int> array_offsets;

    unordered_set<VarID> global_ids;

    unordered_map<VarID, PhiN *> last_global_phi;

    RegionN *start_block = nullptr;
    RegionN *last_block = nullptr;
    PhiN *ret_phi = nullptr;
    PhiN *world_phi = nullptr;
    StopN *stop = nullptr;

    shared_ptr<SymTable> curr_table = nullptr;

    vector<UseE> waiting;

    bool has_return = false;
    VarID var_id = -1;
    UseE addr = nullptr;
    UseE right_val = nullptr;

    RegionN *curr_block = nullptr;

    bool on_right = true;


    void buildTable();

    UseE addPhiOperands(VarID id, PhiN *phi);
    // UseE tryRemoveTrivialPhi(PhiN *phi);

    UseE readVar(VarID id);

    UseE readVar(VarID id, RegionN *region);

    UseE readVarRecursive(VarID id, RegionN *region);

    void writeVar(VarID id, UseE val);

    UseE getWorld();

    void setWorld(UseE world);

    void addContext(RegionN *region);


public:
    SONBuilder(Sea &sea, const unordered_map<VarID, int> &global_offset)
        : sea(sea), global_offsets(global_offset) {}

    pair<RegionN *, StopN *> getResult() {
        return pair(start_block, stop);
    }

    ~SONBuilder() override {
        // free all allocated build context
        for (auto region: contexts) {
            region->freePayload();
        }
    }

    void visit(IntExpr *e) override;

    void visit(CharExpr *e) override;

    void visit(VarExpr *e) override;

    void visitLeftVar(VarExpr *e);

    void visitRightVar(VarExpr *e);

    void visit(OpExpr *e) override;

    void visitLeftOp(OpExpr *e);

    void visitRightOp(OpExpr *e);

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

    void sealBlock(RegionN *block);

    void visit(PareExpr *e) override;


};


}


#endif //C0_SEABUILDER_H
