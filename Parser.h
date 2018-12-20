#pragma once

#include "common.h"
#include "Lexer.h"
#include "AST/AST.h"
#include "SymTable.h"

namespace C0 {


class Parser {

public:
    static Parser fromStr(const string &str);

    explicit Parser(Lexer lexer)
            : lexer(std::move(lexer)), global_table(SymTable::createGlobalTable()) {
        curr_table = global_table;
    };

    unique_ptr<ExprAST> parseExpr();

    unique_ptr<ExprAST> parseFactor();

    unique_ptr<ExprAST> parseTerm();

    unique_ptr<CondAST> parseCond();

    void tryParseVar(bool global);

    void tryParseConst(bool global);

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

    unique_ptr<StmtAST> parseRead();

    unique_ptr<StmtAST> parsePrint();

    shared_ptr<FuncAST> parseFunc();

    shared_ptr<FuncAST> parseFunc(Pos func_pos, Type ret, string func_name);

    Type parseRetType();

    Type parseArgType();

    pair<vector<shared_ptr<FuncAST>>, shared_ptr<SymTable>> parseProg();

    vector<string> &getError() {
        return errors;
    }


private:
    bool checkSemicolon();


    VarID popVarID() {
        return nextVarID++;

    }

    void addError(Pos pos, const string &msg) {
        errors.push_back(fmt::format("line:{}, col:{}: {}", pos.ln, pos.col, msg));
    }

    void addError(const string &msg) {
        auto pos = lexer.headPos();
        errors.push_back(fmt::format("line:{}, col:{}: {}", pos.ln, pos.col, msg));
    }

    vector<string> errors;
    Lexer lexer;
    shared_ptr<SymTable> global_table;
    VarID nextVarID = 1;
    shared_ptr<SymTable> curr_table;
    unordered_map<string, shared_ptr<FuncAST>> func_table;

    template<typename T>
    bool expect(T sym, const string &err) {
        if (lexer.peek().is(sym)) {
            lexer.next();
            return true;
        } else {
            addError(lexer.headPos(), err);
            return false;
        }
    }

};

}
