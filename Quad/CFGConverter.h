//
// Created by baomingkun on 18-11-18.
//

#ifndef C0_CFGCONVERTOR_H
#define C0_CFGCONVERTOR_H


#include "AST/AST.h"
#include "BasicBlock.h"


namespace C0 {

class CFGConverter: public ASTVisitor {

public:

    explicit CFGConverter(BasicBlockBuilder &builder): builder(builder) {
        curr_block = builder.create();
        start_block = curr_block;
    };


    void visit(IntExpr *e) override;

    void visit(CharExpr *e) override;

    void visit(VarExpr *e) override;

    void visit(OpExpr *e) override;

    void visit(CallExpr *e) override;

    void visit(CondAST *e) override;

    void visit(CaseStmt *e) override {}

    void visit(IfStmt *e) override;

    void visit(BlockStmt *e) override;

    void visit(WhileStmt *e) override {}

    void visit(SwitchStmt *e) override {}

    void visit(AsStmt *e) override;

    void visit(ExprStmt *e) override;

    void visit(RetStmt *e) override;

    void visit(EmptyStmt *e) override;

    void visit(DoStmt *e) override;

    void visit(ForStmt *e) override;

    void visit(FuncAST *e) override;

    BasicBlock *getResult() {return start_block;}


    ~CFGConverter() override = default;


private:
    Quad &lastInst() {
        return curr_block->insts.back();
    }

    void cleanWaiting(BasicBlock *next) {
        for(auto p: wait_for_next_block) {
            *p = next;
        }
        wait_for_next_block.clear();
    }

    void addWaiting(BasicBlock **next) {
        wait_for_next_block.push_back(next);
    }

    bool hasWaiting() {
        return !wait_for_next_block.empty();
    }

    int peekTempReg() {
        return next_temp_reg;
    }

    QuadVal popTempReg() {
        return QuadVal(next_temp_reg--);
    }

    void freeAllTempReg() {
        next_temp_reg = -1;
    }



    BasicBlockBuilder &builder;
    BasicBlock *curr_block;
    BasicBlock *start_block;
    bool on_left_side = false;
    bool expr_is_leaf = false;
    bool left_is_var = false;
    QuadVal left_var;
    QuadVal leaf_val;
    QuadVal right_val;

    int next_temp_reg = -1;

    vector<BasicBlock **> wait_for_next_block;

};

}






#endif //C0_CFGCONVERTOR_H
