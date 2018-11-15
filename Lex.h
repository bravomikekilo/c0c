#pragma once
#include "c0.h"
#include "common.h"
#include "util.h"
#include <variant>

namespace C0 {

enum class LexKind {
    Int,
    Char,
    String,
    Ident,
    Keyword,
    Op,
    Cmp,
    Sep,
    Error,
    Eof,
};



class Lex
{
private:
    explicit Lex(std::string &str, LexKind kind) : val(ValType(str)), kind(kind) {}
public:
    using ValType = std::variant<int, char, Op, Cmp, Keyword, Sep, std::string>;
    explicit Lex() : val(ValType(-1)), kind(LexKind::Eof) {}
    explicit Lex(int x) : val(ValType(x)), kind(LexKind::Int) {}
    explicit Lex(char c) : val(ValType(c)), kind(LexKind::Char) {}
    explicit Lex(Op op) : val(ValType(op)), kind(LexKind::Op) {}
    explicit Lex(Cmp cmp) : val(ValType(cmp)), kind(LexKind::Cmp) {}
    explicit Lex(Keyword key) : val(ValType(key)), kind(LexKind::Keyword) {}
    explicit Lex(Sep sep) : val(ValType(sep)), kind(LexKind::Sep) {}

    static Lex String(std::string& str);
    static Lex Ident(std::string& str);
    static Lex Error(std::string str);
       
    bool isError() const {
        return kind == LexKind::Error;
    }

    bool is(LexKind k) const {
        return kind == k;
    }

    bool is(int x) const {
        return (std::holds_alternative<int>(val))
            && std::get<int>(val) == x;
    }
    
    int getInt() const {
        return std::get<int>(val);
    }

    bool is(char c) const {
        return (std::holds_alternative<char>(val))
            && std::get<char>(val) == c;
    }

    char getChar() const {
        return std::get<char>(val);
    }

    bool is(Op op) const {
        return (std::holds_alternative<Op>(val))
            && std::get<Op>(val) == op;
    }

    Op getOp() const {
        return std::get<Op>(val);
    }

    bool is(Cmp cmp) const {
        return (std::holds_alternative<Cmp>(val))
            && std::get<Cmp>(val) == cmp;
    }

    Cmp getCmp() const {
        return std::get<Cmp>(val);
    }

    bool is(Keyword key) const {
        return (std::holds_alternative<Keyword>(val))
            && std::get<Keyword>(val) == key;
    }

    Keyword getKeyword() const {
        return std::get<Keyword>(val);
    }


    bool is(Sep sep) const {
        return (std::holds_alternative<Sep>(val))
            && std::get<Sep>(val) == sep;
    }

    Sep getSep() const {
        return std::get<Sep>(val);
    }
     
    string getString() const {
        return std::get<string>(val);
    }


    string toString() const {
        switch (kind)
        {
        case C0::LexKind::Int:
            return "Int:" + std::to_string(std::get<int>(val));
        case C0::LexKind::Char: {
            std::string ret = "char:";
            ret.push_back(std::get<char>(val));
            return ret;
        }
        case C0::LexKind::String:
            return "str:\"" + std::get<string>(val) + "\"";
        case C0::LexKind::Ident:
            return "ident:" + std::get<string>(val);
        case C0::LexKind::Keyword:
            return "keyword:" + keyToString(std::get<Keyword>(val));
        case C0::LexKind::Op:
            return "op:" + opToString(std::get<Op>(val));
        case C0::LexKind::Cmp:
            return "cmp:" + cmpToString(getCmp());
        case C0::LexKind::Sep:
            return "sep:" + sepToString(std::get<Sep>(val));
        case C0::LexKind::Error:
            return "error:" + std::get<string>(val);
        case C0::LexKind::Eof:
            return "Eof";
        default:
            break;
        }
        return "internal error";
    }

private:
    ValType val;
    LexKind kind;

};

} // using namespace C0