#include "StmtAST.h"

inline void C0::IfStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }

inline void C0::BlockStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }

void C0::WhileStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }

void C0::SwitchStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }

void C0::AsStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }

void C0::ExprStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }

void C0::RetStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }

void C0::EmptyStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }

void C0::CaseStmt::accept(ASTVisitor & visitor) { visitor.visit(this); }
