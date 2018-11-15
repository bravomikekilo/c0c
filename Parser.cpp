#include "Parser.h"
#include "c0.h"


namespace C0 {

Parser Parser::fromStr(string str) {
    auto lexer = Lexer(std::move(str));
    return Parser(std::move(lexer));
}

bool Parser::checkSemicolon() {
    auto ret = lexer.peek().is(Sep::Semicolon);
    if (!ret) report("missing semicolon");
    else lexer.next();
    
    return ret;
}

unique_ptr<ExprAST> Parser::parseFactor() {
    const auto& head = lexer.peek();

    if (head.is(Op::Add)) {

    }

    if (head.is(Op::Sub)) {

    }

    // parse paranthese expression
    if(head.is(Sep::LPar)) {
        lexer.next();
        auto ret = parseExpr();
        if (lexer.peek().is(Sep::RPar)) {
            lexer.next();
        } else {
            report("missing )");
        }
        return ret;
    }

    if (head.is(LexKind::Int)) {
        auto v = head.getInt();
        lexer.next();
        return make_unique<IntExpr>(v);
    }

    if (head.is(LexKind::Char)) {
        auto v = head.getChar();
        lexer.next();
        return make_unique<CharExpr>(v);
    }

    if (head.is(LexKind::Ident)) {
        auto name = head.getString();
        lexer.next();
        if (lexer.peek().is(Sep::LBar)) {
            lexer.next();

            auto var = make_unique<VarExpr>(name);
            auto ind = parseExpr();
            expect(Sep::RBar, "loss ] in index");
            return make_unique<OpExpr>(Op::Ind, std::move(var), std::move(ind));
        }

        if(lexer.peek().is(Sep::LPar)) {
            lexer.next();
            vector<unique_ptr<ExprAST>> args;
            if(lexer.peek().is(Sep::RPar)) {
                lexer.next();
                return make_unique<CallExpr>(name, std::move(args));
            }
            args.push_back(parseExpr());
            while(!lexer.peek().is(Sep::RPar)) {
                if(lexer.peek().is(Sep::Comma)) {
                    lexer.next();
                }
                args.push_back(parseExpr());
            }
            lexer.next();
            return make_unique<CallExpr>(name, std::move(args));
        }

        return make_unique<VarExpr>(name);
    }

    report("missing factor");
    return nullptr;
}

unique_ptr<ExprAST> Parser::parseTerm() {
    auto base = parseFactor();
    while (true) {
        const auto& head = lexer.peek();
        if (head.is(Op::Mul) || head.is(Op::Div)) {
            auto op = head.getOp();
            lexer.next();
            auto rhs = parseFactor();
            base = make_unique<OpExpr>(op, std::move(base), std::move(rhs));
        } else {
            break;
        }
    }
    return base;
}

unique_ptr<ExprAST> Parser::parseExpr() {
    optional<Op> pre;
    if(lexer.peek().is(Op::Add) || lexer.peek().is(Op::Sub)) {
        pre = lexer.peek().getOp();
        lexer.next();
    }
    auto base = parseTerm();

    if(pre.has_value() && pre.value() == Op::Sub) {
        auto const_zero = make_unique<CharExpr>((char)0);
        base = make_unique<OpExpr>(Op::Sub, std::move(const_zero), std::move(base));
    }

    while (true) {
        const auto& head = lexer.peek();
        if (head.is(Op::Add) || head.is(Op::Sub)) {
            auto op = head.getOp();
            lexer.next();
            auto rhs = parseTerm();
            base = make_unique<OpExpr>(op, std::move(base), std::move(rhs));
        } else {
            break;
        }
    }
    return base;
}

unique_ptr<CondAST> Parser::parseCond() {
    auto lhs = parseExpr();
    if (lexer.peek().is(LexKind::Cmp)) {
        auto cp = lexer.peek().getCmp();
        lexer.next();
        auto rhs = parseExpr();
        return make_unique<CondAST>(std::move(lhs), cp, std::move(rhs));
    } else {
        return make_unique<CondAST>(std::move(lhs));
    }
}

unique_ptr<AsStmt> Parser::parseAs() {
    auto lhs = parseExpr();
    expect(Sep::Assign, "loss assign in AsStmt");
    auto rhs = parseExpr();
    return make_unique<AsStmt>(std::move(lhs), std::move(rhs));
}

unique_ptr<StmtAST> Parser::parseStmt() {
    
    const auto& head = lexer.peek();
    if (head.is(Keyword::IF)) {
        return parseIf();
    } else if (head.is(Keyword::WHILE)) {
        return parseWhile();
    } else if(head.is(Keyword::DO)) {
        return parseDo();
    } else if(head.is(Keyword::FOR)) {
        return parseFor();
    } else if (head.is(Keyword::SWITCH)) {
        return parseSwitch();
    } else if (head.is(Sep::LCur)) {
        return parseBlock();
    } else if (head.is(Keyword::RETURN)) {
        return parseRet();
    } else if (head.is(Sep::Semicolon)) {
        lexer.next();
        return make_unique<EmptyStmt>();
    }

    auto lhs = parseExpr();

    if (lexer.peek().is(Sep::Assign)) {
        lexer.next();
        auto rhs = parseExpr();
        checkSemicolon();
        return make_unique<AsStmt>(std::move(lhs), std::move(rhs));
    } 

    checkSemicolon();
    return make_unique<ExprStmt>(std::move(lhs));

}

unique_ptr<CaseStmt> Parser::parseCase()
{
    lexer.next(); // jump over case keyword
    const auto& head = lexer.peek();
    std::variant<char, int> cond;

    if (head.is(LexKind::Int)) {
        cond = { head.getInt() };
    } else if (head.is(LexKind::Char)) {
        cond = { head.getChar() };
    } else {
        cond = { -1 };
    }

    expect(Sep::Colon, "loss colon"); // jump over colon
    auto branch = parseStmt();
    return make_unique<CaseStmt>(cond, std::move(branch));
}

unique_ptr<SwitchStmt> Parser::parseSwitch()
{
    lexer.next(); // jump over switch keyword

    expect(Sep::LPar, "loss (");
    auto exp = parseExpr();
    expect(Sep::RPar, "loss )");
    expect(Sep::LCur, "loss {");
    
    vector<unique_ptr<CaseStmt>> cases;
    while (lexer.peek().is(Keyword::CASE)) {
        cases.push_back(parseCase());
    }
    expect(Sep::RCur, "loss }");

    return make_unique<SwitchStmt>(std::move(exp), std::move(cases));

}

unique_ptr<IfStmt> Parser::parseIf()
{
    lexer.next();
    if (lexer.peek().is(Sep::LPar)) lexer.next();
    else report("missig (");
    
    auto cond = parseCond();

    if (lexer.peek().is(Sep::RPar)) lexer.next();
    else report("missig )");

    auto body = parseStmt();

    if(lexer.peek().is(Keyword::ELSE)) { // parse else
        lexer.next();
        auto other = parseStmt();
        return make_unique<IfStmt>(std::move(cond), std::move(body), std::move(other));
    }

    return make_unique<IfStmt>(std::move(cond), std::move(body));
}

unique_ptr<ForStmt> Parser::parseFor() {
    lexer.next();
    expect(Sep::LPar, "loss (");
    optional<unique_ptr<AsStmt>> start;
    if(lexer.peek().is(Sep::Semicolon)) {
        start = {};
        lexer.next();
    } else {
        start = {parseAs()};
        expect(Sep::Semicolon, "loss ;");
    }

    auto cond = parseCond();
    expect(Sep::Semicolon, "loss ;");

    optional<unique_ptr<AsStmt>> after;
    if(lexer.peek().is(Sep::RPar)) {
        after = {};
        lexer.next();
    } else {
        after = {parseAs()};
        expect(Sep::RPar, "loss )");
    }

    auto body = parseStmt();

    return make_unique<ForStmt>(std::move(start), std::move(cond), std::move(after));

}

unique_ptr<DoStmt> Parser::parseDo() {
    lexer.next(); // jump over keyword do
    auto body = parseStmt();
    expect(Keyword::WHILE, "loss while after do");
    expect(Sep::LPar, "loss (");
    auto cond = parseCond();
    expect(Sep::RPar, "loss )");
    checkSemicolon();

    return make_unique<DoStmt>(std::move(body), std::move(cond));

}

unique_ptr<WhileStmt> Parser::parseWhile()
{
    lexer.next();
    if (lexer.peek().is(Sep::LPar)) lexer.next();
    else report("missig (");

    auto cond = parseCond();

    if (lexer.peek().is(Sep::RPar)) lexer.next();
    else report("missig )");

    auto body = parseStmt();

    return make_unique<WhileStmt>(std::move(cond), std::move(body));
}

unique_ptr<StmtAST> Parser::parseBlock()
{
    lexer.next();
    if (lexer.peek().is(Sep::RCur)) {
        lexer.next();
        return make_unique<EmptyStmt>();
    }
    vector<unique_ptr<StmtAST>> stmts;
    while (!lexer.peek().is(Sep::RCur)) {
        stmts.push_back(parseStmt());
    }
    lexer.next();
    return make_unique<BlockStmt>(std::move(stmts));
}

unique_ptr<RetStmt> Parser::parseRet()
{
    lexer.next();
    if (lexer.peek().is(Sep::Semicolon)) {
        lexer.next();
        return make_unique<RetStmt>();
    }
    auto exp = parseExpr();
    checkSemicolon();
    return make_unique<RetStmt>(std::move(exp));
}

void tryParseVarDecl() {

}


}
