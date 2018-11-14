#include <sstream>
#include "ASTDrawer.h"
#include "AST.h"
#include "../util.h"

#define FMT_HEADER_ONLY

#include "../fmt/format.h"

namespace C0 {

string ASTDrawer::dot(ASTBase &ast) {
    auto drawer = ASTDrawer();
    ast.accept(drawer);
    return drawer.genDot();
}

string ASTDrawer::genDot() const {
    std::stringstream ss;
    ss << "digraph {\n";
    for (int i = 0; i < nodes.size(); ++i) {
        ss << fmt::format("N{} [label=\"{}\"]", i, nodes[i]) << std::endl;
    }

    for (const auto &edge : edges) {
        ss << fmt::format("N{}->N{}[label=\"{}\"]", std::get<0>(edge), std::get<1>(edge), std::get<2>(edge))
           << std::endl;
    }
    ss << "}\n";
    return ss.str();
}

int ASTDrawer::popNode() {
    return currNodeID++;
}

void ASTDrawer::visit(IntExpr *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nInt:{}", nodeID, e->v);
    nodes.push_back(str);
    subID = nodeID;
}

void ASTDrawer::visit(CharExpr *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nChar:'{}'", nodeID, e->v);
    nodes.push_back(str);
    subID = nodeID;
}

void ASTDrawer::visit(VarExpr *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nVar:{}", nodeID, e->name);
    nodes.push_back(str);
    subID = nodeID;
}

void ASTDrawer::visit(IfStmt *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nIf", nodeID);
    nodes.push_back(str);
    e->cond->accept(*this);
    edges.emplace_back(nodeID, subID, "cond");
    e->trueBranch->accept(*this);
    edges.emplace_back(nodeID, subID, "then");
    if(e->falseBranch.has_value()) {
        e->falseBranch.value()->accept(*this);
        edges.emplace_back(nodeID, subID, "else");
    }

    subID = nodeID;
}

void ASTDrawer::visit(BlockStmt *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nBlock", nodeID);
    nodes.push_back(str);
    auto sz = e->stmts.size();
    for (int i = 0; i < sz; ++i) {
        e->stmts[i]->accept(*this);
        edges.emplace_back(nodeID, subID, std::to_string(i + 1));
    }
    subID = nodeID;
}

void ASTDrawer::visit(WhileStmt *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nWhile", nodeID);
    nodes.push_back(str);
    e->cond->accept(*this);
    edges.emplace_back(nodeID, subID, "cond");
    e->body->accept(*this);
    edges.emplace_back(nodeID, subID, "body");
    subID = nodeID;
}

void ASTDrawer::visit(SwitchStmt *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nSwitch");
    nodes.push_back(str);
    e->exp->accept(*this);
    edges.emplace_back(nodeID, subID, "value");
    auto sz = e->cases.size();
    for (int i = 0; i < sz; ++i) {
        e[i].accept(*this);
        edges.emplace_back(nodeID, subID, fmt::format("case #{}", i + 1));
    }
    subID = nodeID;
}

void ASTDrawer::visit(AsStmt *e) {
    auto nodeID = popNode();
    nodes.push_back(fmt::format("#{}\\nAssign", nodeID));
    e->lhs->accept(*this);
    edges.emplace_back(nodeID, subID, "lhs");
    e->rhs->accept(*this);
    edges.emplace_back(nodeID, subID, "rhs");
    subID = nodeID;
}

void ASTDrawer::visit(ExprStmt *e) {
    e->exp->accept(*this);
}

void ASTDrawer::visit(RetStmt *e) {
    auto nodeID = popNode();
    nodes.push_back(fmt::format("#{}\\nRet", nodeID));
    if (e->ret.has_value()) {
        e->ret.value()->accept(*this);
        edges.emplace_back(nodeID, subID, "exp");
    }
    subID = nodeID;
}

void ASTDrawer::visit(EmptyStmt *e) {
    auto nodeID = popNode();
    nodes.push_back(fmt::format("#{}\\nEmpty", nodeID));
    subID = nodeID;
}

void ASTDrawer::visit(CaseStmt *e) {
    auto nodeID = popNode();
    if (std::holds_alternative<int>(e->cond)) {
        nodes.push_back(fmt::format("#{}\\nCase<int>:{}", nodeID, std::get<int>(e->cond)));
    } else {
        nodes.push_back(fmt::format("#{}\\nCase<char>:{}", nodeID, std::get<char>(e->cond)));
    }
    e->branch->accept(*this);
    edges.emplace_back(nodeID, subID, "branch");
    subID = nodeID;
}

void ASTDrawer::visit(OpExpr *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nOp:{}", nodeID, opToString(e->op));
    nodes.push_back(str);

    e->lhs->accept(*this);
    edges.emplace_back(nodeID, subID, "lhs");

    e->rhs->accept(*this);
    edges.emplace_back(nodeID, subID, "rhs");

    subID = nodeID;
}

void ASTDrawer::visit(CondAST *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nCond:{}", nodeID, cmpToString(e->cp));
    nodes.push_back(str);

    e->lhs->accept(*this);
    edges.emplace_back(nodeID, subID, "lhs");

    e->rhs->accept(*this);
    edges.emplace_back(nodeID, subID, "rhs");

    subID = nodeID;
}

void ASTDrawer::visit(CallExpr *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nCall:{}", nodeID, e->name);
    nodes.push_back(str);

    auto sz = e->args.size();
    for(int i = 0; i < sz; ++i) {
        e->args[i]->accept(*this);
        edges.emplace_back(nodeID, subID, "args" + std::to_string(i + 1));
    }

    subID = nodeID;
}

void ASTDrawer::visit(DoStmt *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nDo", nodeID);
    nodes.push_back(str);

    e->body->accept(*this);
    edges.emplace_back(nodeID, subID, "body");

    e->cond->accept(*this);
    edges.emplace_back(nodeID, subID, "cond");

    subID = nodeID;
}

void ASTDrawer::visit(ForStmt *e) {
    auto nodeID = popNode();
    string str = fmt::format("#{}\\nDo", nodeID);
    nodes.push_back(str);

    if(e->start.has_value()) {
        e->start.value()->accept(*this);
        edges.emplace_back(nodeID, subID, "start");
    }

    e->cond->accept(*this);
    edges.emplace_back(nodeID, subID, "cond");

    if(e->start.has_value()) {
        e->start.value()->accept(*this);
        edges.emplace_back(nodeID, subID, "end");
    }

    subID = nodeID;
}

}
