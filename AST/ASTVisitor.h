#pragma once
namespace C0 {

class ASTBase;

class IntExpr;
class CharExpr;
class VarExpr;
class OpExpr;
class CallExpr;

class CondAST;

class CaseStmt;

class IfStmt;
class BlockStmt;
class WhileStmt;
class SwitchStmt;
class AsStmt;
class ExprStmt;
class RetStmt;
class EmptyStmt;
class DoStmt;
class ForStmt;

class ReadStmt;
class PrintStmt;

class FuncAST;

class ASTVisitor
{
public:
    virtual void visit(IntExpr *e) = 0;
    virtual void visit(CharExpr *e) = 0;
    virtual void visit(VarExpr *e) = 0;
    virtual void visit(OpExpr *e) = 0;
    virtual void visit(CallExpr *e) = 0;

    virtual void visit(CondAST *e) = 0;

    virtual void visit(CaseStmt *e) = 0;

    virtual void visit(IfStmt *e) = 0;
    virtual void visit(BlockStmt *e) = 0;
    virtual void visit(WhileStmt *e) = 0;
    virtual void visit(SwitchStmt *e) = 0;
    virtual void visit(AsStmt *e) = 0;
    virtual void visit(ExprStmt *e) = 0;
    virtual void visit(RetStmt *e) = 0;
    virtual void visit(EmptyStmt *e) = 0;
    virtual void visit(DoStmt *e) = 0;
    virtual void visit(ForStmt *e) = 0;
    virtual void visit(PrintStmt *e) = 0;
    virtual void visit(ReadStmt *e) = 0;

    virtual void visit(FuncAST *e) = 0;

    virtual ~ASTVisitor() = default;
};

}
