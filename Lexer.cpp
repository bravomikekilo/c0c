#include "Lexer.h"
#include "util.h"
#include <cctype>


namespace C0 {

void Lexer::next() {
    do {
        currLex = parse();
        if (currLex.isError()) {
            errors.push_back(currLex);
        }
    } while (currLex.isError());
}

optional<Lex> Lexer::skipWhitespace() {
    auto ch = stream.peek();
    while (isspace(ch) || ch == '/') {

        if(ch == '/') {
            stream.get();
            if(stream.peek() == '/') {
                stream.get();
                ch = stream.peek();
                while(ch != '\n' && ch != EOF) {
                    stream.get();
                    ch = stream.peek();
                }
                if(ch == '\n') {
                    stream.get();
                    ch = stream.peek();
                }
            } else {
                return {Lex(Op::Div)};
            }
        } else {
            stream.get();
            ch = stream.peek();
        }

    }

    return {};
}

Lex Lexer::parseChar() {
    bool invalid_char = true;
    string err = "'";
    stream.get();
    auto c = stream.peek();
    if (c == EOF) {
        return Lex::Error(err);
    }
    char ch = static_cast<char>(c);
    stream.get();
    if(ch == '+' || ch == '-' || ch == '_' || isdigit(ch) || isalpha(ch)) {
        invalid_char = false;
    }

    err.push_back(ch);
    auto last = stream.peek();
    if (last != '\'' || invalid_char) {
        for (;last != EOF && last != '\n' && last != '\''; last = stream.peek()) {
            err.push_back(last);
            stream.get();
        }
        if (last == '\'') {
            stream.get();
            err.push_back('\'');
        }
        return Lex::Error(err);
    } else {
        stream.get();
        return Lex(ch);
    }
}

Lex Lexer::parseString() {
    bool invalid_char = false;
    string raw;
    stream.get();
    auto head = stream.peek();
    for (; head != EOF && head != '\n' && head != '"'; head = stream.peek()) {
        if(head < 32 || head > 126) {
            invalid_char = true;
        }
        raw.push_back(static_cast<char>(head));
        stream.get();
    }
    if (head == '"' && !invalid_char) {
        stream.get();
        return Lex::String(raw);
    } else {
        return Lex::Error(raw);
    }
    // return Lex::String(raw);
}

Lex Lexer::parseIdentOrKey() {
    std::string raw;
    for (auto head = stream.peek(); isalnum(head) || head == '_'; head = stream.peek()) {
        raw.push_back(static_cast<char>(head));
        stream.get();
    }
    auto maybekey = isKeyword(raw);
    if (maybekey.has_value()) {
        return Lex(maybekey.value());
    }
    else {
        return Lex::Ident(raw);
    }
}

Lex Lexer::parseInt() {
    string chunk;
    bool overflow = false;
    bool hasZero = false;
    bool multiZero = false;
    auto head = stream.peek();
    int ret = 0;
    int sign = 1;
    if (!isdigit(head)) {
        if (head == '-') sign = -1;
        stream.get();
    }
    while (isdigit(stream.peek())) {
        if (stream.peek() == '0') {
            if (!hasZero) hasZero = true;
            else multiZero = true;
        }

        int prev = ret;
        chunk.push_back(static_cast<char>(stream.peek()));
        ret *= 10;
        ret += stream.get() - '0';
        if(prev > ret) {
            overflow = true;
        }
    }
    if(overflow || multiZero) {
        return Lex::Error(chunk);
    } else {
        return Lex(sign * ret);
    }
}

Lex Lexer::parse() {
    auto maybe = skipWhitespace();

    if(maybe.has_value()) {
        return maybe.value();
    }

    auto head = stream.peek();
    if (head == EOF) return Lex();
    if (isdigit(head)) {
        return parseInt();
    }
    if (head == '"') {
        return parseString();
    }

    if (head == '\'') {
        return parseChar();
    }

    if (head == '_' || isalpha(head)) {
        return parseIdentOrKey();
    }

    switch (head) {
    case ')':
        stream.get();
        return Lex(Sep::RPar);
    case '(':
        stream.get();
        return Lex(Sep::LPar);
    case ']':
        stream.get();
        return Lex(Sep::RBar);
    case '[':
        stream.get();
        return Lex(Sep::LBar);
    case '}':
        stream.get();
        return Lex(Sep::RCur);
    case '{':
        stream.get();
        return Lex(Sep::LCur);
    case ';':
        stream.get();
        return Lex(Sep::Semicolon);
    case ':':
        stream.get();
        return Lex(Sep::Colon);
    case ',':
        stream.get();
        return Lex(Sep::Comma);
    case '+': 
        stream.get();
        return Lex(Op::Add);
    case '-':
        stream.get();
        return Lex(Op::Sub);
    case '*':
        stream.get();
        return Lex(Op::Mul);

    case '!': {
        stream.get();
        auto next = stream.peek();
        if(next == '=') {
            stream.get();
            return Lex(Cmp::UnEqual);
        } else {
            return Lex::Error("!");
        }
    }

    case '/':
        stream.get();
        return Lex(Op::Div);
    case '>':
        stream.get();
        if (stream.peek() == '=') {
            stream.get();
            return Lex(Cmp::GreaterEq);
        } else {
            return Lex(Cmp::Greater);
        }

    case '<':
        stream.get();
        if (stream.peek() == '=') {
            stream.get();
            return Lex(Cmp::LessEq);
        } else {
            return Lex(Cmp::Less);
        }
    case '=':
        stream.get();
        if (stream.peek() == '=') {
            stream.get();
            return Lex(Cmp::Equal);
        } else {
            return Lex(Sep::Assign);
        }
    default: {
        string err;
        while (!isspace(stream.peek())) {
            err.push_back(static_cast<char>(stream.get()));
        }
        return Lex::Error(err);
    }

    }
}


} // end namespace C0
