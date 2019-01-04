#pragma once

#include "common.h"
#include "Lex.h"
#include <sstream>
#include "SourceStream.h"

namespace C0 {

class Lexer {
public:
    explicit Lexer(const std::string &source)
            :stream(source), errors(vector<string>()), head_pos(stream.getPos()) {
        next();
    };

    const Lex &peek() const { return currLex; }

    Pos headPos() const { return head_pos;}

    const vector<string> &getErrors() const { return errors; }

    void next();

private:
    vector<string> errors;

    optional<Lex> skipWhitespace();

    SourceStream stream;

    Lex currLex;

    Pos head_pos; // head pos of currLex

    Lex parseIdentOrKey();

    Lex parseString();

    Lex parseInt();

    Lex parseChar();

    Lex parse();

    void addError(const string &str) {
        errors.push_back(head_pos.toStr() + ": " + str);
    }
};

} // end namespace C0