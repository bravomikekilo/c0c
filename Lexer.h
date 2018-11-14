#pragma once
#include "common.h"
#include "Lex.h"
#include <sstream>

namespace C0 {

class Lexer
{
public:
    explicit Lexer(const std::string &source)
        : stream(std::stringstream(source)), errors(vector<Lex>()) {
        next();
    };
    const Lex &peek() const { return currLex; }
    const vector<Lex> &getErrors() const { return errors; }
    void next();
private:
    vector<Lex> errors;
    optional<Lex> skipWhitespace();
    Lex currLex;
    std::stringstream stream;
    Lex parseIdentOrKey();
    Lex parseString();
    Lex parseInt();
    Lex parseChar();
    Lex parse();
};

} // end namespace C0