#pragma once
#include "common.h"
#include "Lexer.h"
#include "AST/AST.h"
#include "StackTable.h"

namespace C0 {


class Parser
{

public:
    static Parser fromStr(string str);
    explicit Parser(Lexer lexer) 
        : lexer(std::move(lexer)) , global_table(make_shared<SymTable>()) 
    {
        curr_table = global_table;
    };

    unique_ptr<ExprAST> parseExpr();
    unique_ptr<ExprAST> parseFactor();
    unique_ptr<ExprAST> parseTerm();
    unique_ptr<CondAST> parseCond();

    unique_ptr<StmtAST> parseStmt();

    // called only with first keyword case
    unique_ptr<CaseStmt> parseCase();

    // called only with first keyword switch
    unique_ptr<SwitchStmt> parseSwitch();

    // called only with first keyword if
    unique_ptr<IfStmt> parseIf();

    // called only with first keyword while
    unique_ptr<WhileStmt> parseWhile();

    unique_ptr<ForStmt> parseFor();

    unique_ptr<DoStmt> parseDo();

    unique_ptr<AsStmt> parseAs();

    // called only with first {
    unique_ptr<StmtAST> parseBlock();

    // called only with first keyword return
    unique_ptr<RetStmt> parseRet();

    // void tryParseVarDecl();



private:
    bool checkSemicolon();
    vector<string> errors;
    void report(const string &err) {
        errors.push_back(err);
    }
    Lexer lexer;
    shared_ptr<SymTable> global_table;
    shared_ptr<SymTable> curr_table;
    
    template<typename T>
    bool expect(T sym, const string &err) {
        if (lexer.peek().is(sym)) {
            lexer.next();
            return true;
        } else {
            report(err);
            return false;
        }
    }

};

}
