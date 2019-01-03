#include <fstream>
#include "util.h"

namespace C0 {
optional<Keyword> isKeyword(const string &raw) {
    if (raw == "const") {
        return Keyword::CONST_;
    }
    if (raw == "if") {
        return Keyword::IF;
    }
    if (raw == "else") {
        return Keyword::ELSE;
    }
    if (raw == "do") {
        return Keyword::DO;
    }
    if (raw == "while") {
        return Keyword::WHILE;
    }
    if (raw == "for") {
        return Keyword::FOR;
    }
    /*
    if (raw == "switch") {
        return Keyword::SWITCH;
    }
    if (raw == "case") {
        return Keyword::CASE;
    }
    */
    if (raw == "scanf") {
        return Keyword::SCANF;
    }
    if (raw == "printf") {
        return Keyword::PRINTF;
    }
    if (raw == "return") {
        return Keyword::RETURN;
    }
    if (raw == "int") {
        return Keyword::INT;
    }
    if (raw == "char") {
        return Keyword::CHAR;
    }
    if (raw == "void") {
        return Keyword::VOID_;
    }
    return optional<Keyword>();
}

string sepToString(Sep sep) {
    switch (sep) {
        case Sep::LPar:
            return "(";
        case Sep::RPar:
            return ")";
        case Sep::LBar:
            return "[";
        case Sep::RBar:
            return "]";
        case Sep::LCur:
            return "{";
        case Sep::RCur:
            return "}";
        case Sep::Comma:
            return ",";
        case Sep::Colon:
            return ":";
        case Sep::Semicolon:
            return ";";
        case Sep::Assign:
            return "=";
        default:
            return {};
    }
}

string opToString(Op op) {
    switch (op) {
        case Op::Ind:
            return "[]";
        case Op::Add:
            return "+";
        case Op::Sub:
            return "-";
        case Op::Mul:
            return "*";
        case Op::Div:
            return "/";
        default:
            return {};
    }
}

string cmpToString(Cmp cmp) {
    switch (cmp) {
        case Cmp::Greater:
            return ">";
        case Cmp::GreaterEq:
            return ">=";
        case Cmp::Less:
            return "<";
        case Cmp::LessEq:
            return "<=";
        case Cmp::Equal:
            return "==";
        case Cmp::UnEqual:
            return "!=";
        default:
            return {};
    }
}

string keyToString(Keyword key) {
    switch (key) {
        case Keyword::CONST_:
            return "const";
        case Keyword::IF:
            return "if";
        case Keyword::ELSE:
            return "else";
        case Keyword::DO:
            return "do";
        case Keyword::WHILE:
            return "while";
        case Keyword::FOR:
            return "for";
        case Keyword::SWITCH:
            return "switch";
        case Keyword::CASE:
            return "case";
        case Keyword::SCANF:
            return "scanf";
        case Keyword::PRINTF:
            return "printf";
        case Keyword::RETURN:
            return "return";
        case Keyword::INT:
            return "int";
        case Keyword::CHAR:
            return "char";
        case Keyword::VOID_:
            return "void";
        default:
            return {};
    }
}

std::string getFileContents(const char *filename) {
    std::ifstream infile(filename, std::ios::in | std::ios::binary);
    if (infile.is_open()) {
        std::string contents;
        infile.seekg(0, std::ios::end);
        contents.resize(infile.tellg()); // same as: std::string contents(infile.tellg(), '\0');
        infile.seekg(0, std::ios::beg);
        infile.read(&contents[0], contents.size());
        infile.close();
        return (contents);
    }
    throw (errno);
}

}  // end namespace C0

