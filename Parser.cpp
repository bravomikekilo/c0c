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
    const auto &head = lexer.peek();

    if (head.is(Op::Add)) {
        lexer.next();
        if (lexer.peek().is(LexKind::Int)) {
            auto v = head.getInt();
            lexer.next();
            return make_unique<IntExpr>(v);
        } else {
            errors.emplace_back("duplicate +");
            return parseFactor();
        }
    }

    if (head.is(Op::Sub)) {
        lexer.next();
        if (lexer.peek().is(LexKind::Int)) {
            auto v = head.getInt();
            lexer.next();
            return make_unique<IntExpr>(-v);
        } else {
            errors.emplace_back("duplicate -");
            return parseFactor();
        }
    }

    // parse paranthese expression
    if (head.is(Sep::LPar)) {
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
        auto varID = curr_table->findVarByName(name);
        VarID id = varID.value_or(0);
        /*
        if (!varID.has_value()) {
            errors.push_back("unknown variable:" + name);
        } else {
            id = varID.value();
        }
        */



        lexer.next();
        if (lexer.peek().is(Sep::LPar)) {  // is a function call
            lexer.next();
            vector<unique_ptr<ExprAST>> args;
            if (lexer.peek().is(Sep::RPar)) {
                lexer.next();
                return make_unique<CallExpr>(name, std::move(args));
            }
            args.push_back(parseExpr());
            while (!lexer.peek().is(Sep::RPar)) {
                if (lexer.peek().is(Sep::Comma)) {
                    lexer.next();
                } else {
                    errors.emplace_back("missing , in function call");
                }
                args.push_back(parseExpr());
            }
            lexer.next();
            return make_unique<CallExpr>(name, std::move(args));
        }

        if (id == 0) {
            errors.push_back("unknown variable:" + name);
        }
        if (lexer.peek().is(Sep::LBar)) {
            lexer.next();
            auto var = make_unique<VarExpr>(id);
            auto ind = parseExpr();
            expect(Sep::RBar, "loss ] in index");
            return make_unique<OpExpr>(Op::Ind, std::move(var), std::move(ind));
        }


        return make_unique<VarExpr>(id);
    }

    report("missing factor");
    return nullptr;
}

unique_ptr<ExprAST> Parser::parseTerm() {
    auto base = parseFactor();
    while (true) {
        const auto &head = lexer.peek();
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
    if (lexer.peek().is(Op::Add) || lexer.peek().is(Op::Sub)) {
        pre = lexer.peek().getOp();
        lexer.next();
    }
    auto base = parseTerm();

    if (pre.has_value() && pre.value() == Op::Sub) {
        auto const_zero = make_unique<IntExpr>(0);
        base = make_unique<OpExpr>(Op::Sub, std::move(const_zero), std::move(base));
    }
    while (true) {
        const auto &head = lexer.peek();
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

    const auto &head = lexer.peek();
    if (head.is(Keyword::IF)) {
        return parseIf();
    } else if (head.is(Keyword::DO)) {
        auto parsed = parseDo();
        return parsed;
    } else if (head.is(Keyword::FOR)) {
        auto parsed = parseFor();
        return parsed;
    } else if (head.is(Keyword::SWITCH)) {
        auto parsed = parseSwitch();
        return parsed;
    } else if (head.is(Sep::LCur)) {
        auto parsed = parseBlock();
        return parsed;
    } else if (head.is(Keyword::RETURN)) {
        auto parsed = parseRet();
        return parsed;
    } else if (head.is(Keyword::PRINTF)) {
        auto parsed = parsePrint();
        return parsed;
    } else if (head.is(Keyword::SCANF)){
        auto parsed = parseRead();
        return parsed;
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

unique_ptr<CaseStmt> Parser::parseCase() {
    lexer.next(); // jump over case keyword
    const auto &head = lexer.peek();
    std::variant<char, int> cond;

    if (head.is(LexKind::Int)) {
        cond = {head.getInt()};
    } else if (head.is(LexKind::Char)) {
        cond = {head.getChar()};
    } else {
        cond = {-1};
    }

    expect(Sep::Colon, "loss colon"); // jump over colon
    auto branch = parseStmt();
    return make_unique<CaseStmt>(cond, std::move(branch));
}

unique_ptr<SwitchStmt> Parser::parseSwitch() {
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

unique_ptr<IfStmt> Parser::parseIf() {
    lexer.next();
    if (lexer.peek().is(Sep::LPar)) lexer.next();
    else report("missing (");

    auto cond = parseCond();

    if (lexer.peek().is(Sep::RPar)) lexer.next();
    else report("missing )");

    auto body = parseStmt();

    if (lexer.peek().is(Keyword::ELSE)) { // parse else
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
    if (lexer.peek().is(Sep::Semicolon)) {
        start = {};
        lexer.next();
    } else {
        start = {parseAs()};
        expect(Sep::Semicolon, "loss ;");
    }

    auto cond = parseCond();
    expect(Sep::Semicolon, "loss ;");

    optional<unique_ptr<AsStmt>> after;
    if (lexer.peek().is(Sep::RPar)) {
        after = {};
        lexer.next();
    } else {
        after = {parseAs()};
        expect(Sep::RPar, "loss )");
    }

    auto body = parseStmt();

    return make_unique<ForStmt>(std::move(start), std::move(cond), std::move(after), std::move(body));

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

unique_ptr<WhileStmt> Parser::parseWhile() {
    lexer.next();
    if (lexer.peek().is(Sep::LPar)) lexer.next();
    else report("missing (");

    auto cond = parseCond();

    if (lexer.peek().is(Sep::RPar)) lexer.next();
    else report("missing )");

    auto body = parseStmt();

    return make_unique<WhileStmt>(std::move(cond), std::move(body));
}

unique_ptr<StmtAST> Parser::parseBlock() {
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

unique_ptr<RetStmt> Parser::parseRet() {
    lexer.next();
    if (lexer.peek().is(Sep::Semicolon)) {
        lexer.next();
        return make_unique<RetStmt>();
    }
    auto exp = parseExpr();
    checkSemicolon();
    return make_unique<RetStmt>(std::move(exp));
}

bool isVarHead(const Lex &lex) {
    if (!lex.is(LexKind::Keyword)) {
        return false;
    }
    auto key = lex.getKeyword();
    return key == Keyword::CHAR || key == Keyword::INT;

}

void Parser::tryParseVar(bool global) {
    while (isVarHead(lexer.peek())) {
        auto baseType = BaseTypeK::Error;
        if (lexer.peek().is(Keyword::CHAR)) {
            baseType = BaseTypeK::Char;
        } else if (lexer.peek().is(Keyword::INT)) {
            baseType = BaseTypeK::Int;
        }
        // auto baseType = lexer.peek().is(Keyword::CHAR) ? BaseTypeK::Char : BaseTypeK::Int;
        lexer.next();
        do {
            if (!lexer.peek().is(LexKind::Ident)) {
                errors.emplace_back("missing a variable name");
                if (lexer.peek().is(Sep::Semicolon)) {
                    break;
                }
                lexer.next();
            } else {
                auto name = lexer.peek().getString();
                lexer.next();
                if (lexer.peek().is(Sep::LBar)) {
                    lexer.next();
                    size_t length = 0;
                    if (lexer.peek().is(LexKind::Int)) {
                        length = lexer.peek().getInt();
                        lexer.next();
                    } else {
                        if (lexer.peek().is(Sep::RBar)) {
                            length = 0;
                            errors.emplace_back("missing length in array variable");
                        } else {
                            errors.emplace_back("invalid length of array variable");
                            lexer.next();
                        }
                    }
                    expect(Sep::RBar, "miss ] in array variable");
                    Type t(baseType, length);
                    if (curr_table->hasVarInScope(name)) {
                        errors.push_back("multiple definition of variable:" + name);
                    } else {
                        curr_table->insert(SymTerm{popVarID(), t, name, global, {}});
                    }
                } else { // scalar variable
                    Type t(baseType);
                    if (curr_table->hasVarInScope(name)) {
                        errors.push_back("multiple definition of variable:" + name);
                    } else {
                        curr_table->insert(SymTerm{ popVarID(), t, name, global, {} });
                    }
                }
            }

            if (lexer.peek().is(Sep::Semicolon)) {
                break;
            }

            if (lexer.peek().is(LexKind::Eof)) {
                errors.push_back("missing ; in var definition");
                break;
            }
            if (lexer.peek().is(Sep::Comma)) {
                lexer.next();
            }
        } while (true);
        lexer.next();
    }
}

void Parser::tryParseConst(bool global) {
    while (lexer.peek().is(Keyword::CONST)) {
        lexer.next();
        auto baseType = BaseTypeK::Error;
        if (lexer.peek().is(Keyword::CHAR)) {
            baseType = BaseTypeK::Char;
            lexer.next();
        } else if (lexer.peek().is(Keyword::INT)) {
            baseType = BaseTypeK::Int;
            lexer.next();
        }

        if (baseType == BaseTypeK::Error) {
            errors.emplace_back("can't find type for const");
        }
        // auto baseType = lexer.peek().is(Keyword::CHAR) ? BaseTypeK::Char : BaseTypeK::Int;
        do {
            if (!lexer.peek().is(LexKind::Ident)) {
                errors.emplace_back("miss a variable name");
            } else {
                auto name = lexer.peek().getString();
                lexer.next();
                expect(Sep::Assign, "miss = in const");
                int val = 0;


                if (lexer.peek().is(LexKind::Int)) {
                    val = lexer.peek().getInt();
                    lexer.next();
                } else if (lexer.peek().is(Op::Add)) {
                    lexer.next();
                    if (lexer.peek().is(LexKind::Int)) {
                        val = lexer.peek().getInt();
                        lexer.next();
                    } else {
                        errors.emplace_back("can't find const value");
                    }
                } else if (lexer.peek().is(Op::Sub)) {
                    lexer.next();
                    if (lexer.peek().is(LexKind::Int)) {
                        val = -lexer.peek().getInt();
                        lexer.next();
                    } else {
                        errors.emplace_back("can't find const value");
                    }
                } else if (lexer.peek().is(LexKind::Char)) {
                    val = lexer.peek().getChar();
                    lexer.next();
                } else {
                    errors.emplace_back("can't find const value");
                }

                Type t(baseType);
                if (curr_table->hasVarInScope(name)) {
                    errors.push_back("multiple definition of variable:" + name);
                } else {
                    curr_table->insert(SymTerm{popVarID(), t, name, global, {val}});
                }
            }
            if (lexer.peek().is(Sep::Comma)) {
                lexer.next();
            }
        } while (!lexer.peek().is(Sep::Semicolon));
        lexer.next();
    }
}

pair<vector<shared_ptr<FuncAST>>, shared_ptr<SymTable>> Parser::parseProg() {
    vector<shared_ptr<FuncAST>> funcs;
    tryParseConst(true);
    while (isVarHead(lexer.peek())) {
        auto baseType = lexer.peek().is(Keyword::CHAR) ? BaseTypeK::Char : BaseTypeK::Int;
        lexer.next();
        if (!lexer.peek().is(LexKind::Ident)) {
            errors.emplace_back("miss a variable or function name");
        }
        auto first_name = lexer.peek().getString();
        lexer.next();
        if (lexer.peek().is(Sep::LPar)) {
            auto func = parseFunc(Type(baseType), first_name);
            funcs.push_back(func);
            // func_table.insert(pair(first_name, func));
            break;
        } else if (lexer.peek().is(Sep::LBar)) {
            lexer.next();
            size_t length = 0;
            if (lexer.peek().is(LexKind::Int)) {
                length = lexer.peek().getInt();
                lexer.next();
            } else {
                if (lexer.peek().is(Sep::RBar)) {
                    length = 0;
                    errors.emplace_back("missing length in array variable");
                } else {
                    errors.emplace_back("invalid length in array variable");
                    lexer.next();
                }
            }
            expect(Sep::RBar, "miss ] in array variable");
            Type t(baseType, length);
            if (curr_table->hasVarInScope(first_name)) {
                errors.push_back("multiple definition of variable:" + first_name);
            } else {
                curr_table->insert(SymTerm{popVarID(), t, first_name, false, {}});
            }
        } else {
            Type t(baseType);
            if (curr_table->hasVarInScope(first_name)) {
                errors.push_back("multiple definition of variable:" + first_name);
            } else {
                curr_table->insert(SymTerm{popVarID(), t, first_name, false, {}});
            }
        }


        while (lexer.peek().is(Sep::Comma)) {
            lexer.next();
            if (!lexer.peek().is(LexKind::Ident)) {
                errors.emplace_back("miss a variable name");
            } else {
                auto name = lexer.peek().getString();
                lexer.next();
                if (lexer.peek().is(Sep::LBar)) {
                    lexer.next();
                    size_t length = 0;
                    if (lexer.peek().is(LexKind::Int)) {
                        length = lexer.peek().getInt();
                        lexer.next();
                    } else {
                        if (lexer.peek().is(Sep::RBar)) {
                            length = 0;
                            errors.emplace_back("missing length in array variable");
                        } else {
                            errors.emplace_back("invalid length of array variable");
                            lexer.next();
                        }
                    }
                    expect(Sep::RBar, "miss ] in array variable");
                    Type t(baseType, length);
                    if (curr_table->hasVarInScope(name)) {
                        errors.push_back("multiple definition of variable:" + name);
                    } else {
                        curr_table->insert(SymTerm{popVarID(), t, name, true, {}});
                    }
                } else { // scalar variable
                    Type t(baseType);
                    if (curr_table->hasVarInScope(name)) {
                        errors.push_back("multiple definition of variable:" + name);
                    } else {
                        curr_table->insert(SymTerm{popVarID(), t, name, true, {}});
                    }
                }
            }
            /*
            if (lexer.peek().is(Sep::Comma)) {
                lexer.next();
            }
            */
        };
        checkSemicolon();
    }


    while (!lexer.peek().is(LexKind::Eof)) {
        auto func = parseFunc();
        funcs.push_back(func);
        if (func != nullptr) {
            func_table.insert(pair(func->name, func));
        }
    }

    return pair(funcs, global_table);

}

Type Parser::parseRetType() {
    auto head = lexer.peek();
    lexer.next();
    if (head.is(Keyword::VOID)) {
        return Type(BaseTypeK::Void);
    } else if (head.is(Keyword::CHAR)) {
        return Type(BaseTypeK::Char);
    } else if (head.is(Keyword::INT)) {
        return Type(BaseTypeK::Int);
    } else {
        return Type(BaseTypeK::Error);
    }

}


Type Parser::parseArgType() {
    auto head = lexer.peek();
    lexer.next();
    if (head.is(Keyword::VOID)) {
        return Type(BaseTypeK::Error);
    } else if (head.is(Keyword::CHAR)) {
        return Type(BaseTypeK::Char);
    } else if (head.is(Keyword::INT)) {
        return Type(BaseTypeK::Int);
    } else {
        return Type(BaseTypeK::Error);
    }

}

shared_ptr<FuncAST> Parser::parseFunc() {
    auto retType = parseRetType();

    string func_name;
    if (lexer.peek().is(LexKind::Ident)) {
        func_name = lexer.peek().getString();
        lexer.next();
    } else {
        errors.emplace_back("can't find function name");
    }

    auto func = parseFunc(retType, func_name);

    return func;
}

shared_ptr<FuncAST> Parser::parseFunc(Type ret, string func_name) {

    expect(Sep::LPar, "missing ( in function");
    curr_table = make_shared<SymTable>(curr_table);

    vector<pair<Type, string>> args;

    if (!lexer.peek().is(Sep::RPar)) {
        auto first_type = parseArgType();
        string first_arg;
        if (lexer.peek().is(LexKind::Ident)) {
            first_arg = lexer.peek().getString();
            lexer.next();
        }
        curr_table->insert({popVarID(), first_type, first_arg, false, {}});
        args.emplace_back(pair(first_type, first_arg));

        while (!lexer.peek().is(Sep::RPar)) {
            if (lexer.peek().is(LexKind::Eof)) {
                return {};
            }
            if (lexer.peek().is(Sep::LCur)) {
                errors.emplace_back("missing )");
                break;
            }
            if (lexer.peek().is(Sep::Comma)) {
                lexer.next();
            } else {
                errors.emplace_back("missing comma");
            }

            auto arg_type = parseArgType();

            string arg_name;
            if (lexer.peek().is(LexKind::Ident)) {
                arg_name = lexer.peek().getString();
                lexer.next();
            }
            curr_table->insert({popVarID(), arg_type, arg_name, false, {}});
            args.emplace_back(pair(arg_type, arg_name));

        }
    }
    lexer.next();


    expect(Sep::LCur, "missing { in function");
    tryParseConst(false);
    tryParseVar(false);

    vector<unique_ptr<StmtAST>> stmts;

    while (!lexer.peek().is(Sep::RCur)) {
        if (lexer.peek().is(LexKind::Eof)) {
            errors.emplace_back("missing } at end of file");
            break;
        }
        stmts.push_back(parseStmt());
    }

    lexer.next();

    return make_shared<FuncAST>(curr_table, func_name, ret, std::move(args), std::move(stmts));
}

unique_ptr<StmtAST> Parser::parseRead() {
    lexer.next();
    bool finish = false;
    vector<unique_ptr<VarExpr>> args;
    expect(Sep::LPar, "missing ( in read");
    if (!lexer.peek().is(LexKind::Ident)) {
        errors.emplace_back("can't find variable in read");
        if (lexer.peek().is(Sep::RPar)) {
            lexer.next();
            finish = true;
        }
    } else {
        auto var_name = lexer.peek().getString();
        lexer.next();
        auto var_id = curr_table->findVarByName(var_name);
        VarID id = 0;
        if (!var_id.has_value()) {
            errors.push_back("unknown variable:" + var_name);
        } else {
            id = var_id.value();
        }
        args.push_back(std::move(make_unique<VarExpr>(id)));
    }

    if (!finish) {
        while (!lexer.peek().is(Sep::RPar)) {
            if (lexer.peek().is(LexKind::Eof)) break;
            if (lexer.peek().is(Sep::Comma)) {
                lexer.next();
            } else {
                errors.emplace_back("missing , in read");
            }
            if (lexer.peek().is(LexKind::Ident)) {
                auto var_name = lexer.peek().getString();
                lexer.next();
                auto var_id = curr_table->findVarByName(var_name);
                VarID id = 0;
                if (!var_id.has_value()) {
                    errors.push_back("unknown variable:" + var_name);
                } else {
                    id = var_id.value();
                }
                args.push_back(std::move(make_unique<VarExpr>(id)));
            } else {
                errors.emplace_back("missing variable in read");
                lexer.next();
            }
        }
        lexer.next();
        finish = true;

    }
    checkSemicolon();
    return make_unique<ReadStmt>(std::move(args));
}

unique_ptr<StmtAST> Parser::parsePrint() {
    lexer.next();
    expect(Sep::LPar, "missing print");
    optional<int> str;

    if (lexer.peek().is(LexKind::String)) {
        auto str_id = curr_table->addString(lexer.peek().getString());
        str = str_id;
        lexer.next();
    }

    if (lexer.peek().is(Sep::RPar)) {
        lexer.next();
        optional<unique_ptr<ExprAST>> exp;
        checkSemicolon();
        return make_unique<PrintStmt>(str, std::move(exp));
    }

    if (str.has_value()) {
        expect(Sep::Comma, "missing , in print");
    }

    optional<unique_ptr<ExprAST>> exp = {parseExpr()};

    expect(Sep::RPar, "missing ) in print");
    checkSemicolon();
    return make_unique<PrintStmt>(str, std::move(exp));

}


}
