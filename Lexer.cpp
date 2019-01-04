#include "Lexer.h"
#include "util.h"
#include <cctype>


namespace C0 {

void Lexer::next() {
    do {
        currLex = parse();
        if (currLex.isError()) {
            addError(currLex.toString());
        }
    } while (currLex.isError());
}

optional<Lex> Lexer::skipWhitespace() {
    auto ch = stream.peek();
    while (isspace(ch) || ch == '/') {

        if (ch == '/') {
            stream.get();
            head_pos = stream.getPos();
            if (stream.peek() == '/') {
                stream.get();
                ch = stream.peek();
                while (ch != '\n' && ch != EOF) {
                    stream.get();
                    ch = stream.peek();
                }
                if (ch == '\n') {
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
    head_pos = stream.getPos();
    auto c = stream.peek();
    if (c == EOF) {
        err += " unexpected end of file";
        addError(err);
        // return Lex::Error(err);
        char ch = -1;
        return Lex(ch);
    }
    char ch = static_cast<char>(c);
    stream.get();
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '_' || isdigit(ch) || isalpha(ch)) {
        invalid_char = false;
    }

    err.push_back(ch);
    auto last = stream.peek();
    if (last != '\'' || invalid_char) {
        for (; last != EOF && last != '\n' && last != '\''; last = stream.peek()) {
            err.push_back(last);
            stream.get();
        }
        if (last == '\'') {
            stream.get();
            err.push_back('\'');
        }
        err += " invalid char";
        addError(err);
        // return Lex::Error(err);
    } else {
        stream.get();
    }
    return Lex(ch);
}

Lex Lexer::parseString() {
    bool invalid_char = false;
    string raw;
    stream.get();
    head_pos = stream.getPos();
    auto head = stream.peek();
    for (; head != EOF && head != '\n' && head != '"'; head = stream.peek()) {
        if (head < 32 || head > 126) {
            invalid_char = true;
        }
        raw.push_back(static_cast<char>(head));
        stream.get();
    }
    if (head == '"' && !invalid_char) {
        stream.get();
    } else {
        raw += "   invalid char in string";
        addError(raw);
    }
    return Lex::String(raw);
    // return Lex::String(raw);
}

Lex Lexer::parseIdentOrKey() {
    std::string raw;
    head_pos = stream.peekPos();
    for (auto head = stream.peek(); isalnum(head) || head == '_'; head = stream.peek()) {
        raw.push_back(static_cast<char>(head));
        stream.get();
    }
    auto maybekey = isKeyword(raw);
    if (maybekey.has_value()) {
        return Lex(maybekey.value());
    } else {
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

    head_pos = stream.peekPos();
    while (stream.peek() == '0') {
        chunk.push_back(static_cast<char>(stream.peek()));
        stream.get();
        if (!hasZero) {
            hasZero = true;
            continue;
        }
        if (hasZero) { multiZero = true; }
    }

    while (isdigit(stream.peek())) {
        /*
        if (stream.peek() == '0') {
            if (!hasZero) hasZero = true;
            else multiZero = true;
        }
        */

        int prev = ret;
        chunk.push_back(static_cast<char>(stream.peek()));
        ret *= 10;
        ret += stream.get() - '0';
        if (prev > ret) {
            overflow = true;
        }
    }
    if (overflow || multiZero || (hasZero && ret != 0)) {
        addError(chunk + " multiple zero");
    }
    return Lex(sign * ret);

}

Lex Lexer::parse() {
    auto maybe = skipWhitespace();

    if (maybe.has_value()) {
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
            head_pos = stream.getPos();
            return Lex(Sep::RPar);
        case '(':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Sep::LPar);
        case ']':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Sep::RBar);
        case '[':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Sep::LBar);
        case '}':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Sep::RCur);
        case '{':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Sep::LCur);
        case ';':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Sep::Semicolon);
        case ':':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Sep::Colon);
        case ',':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Sep::Comma);
        case '+':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Op::Add);
        case '-':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Op::Sub);
        case '*':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Op::Mul);

        case '!': {
            stream.get();
            head_pos = stream.getPos();
            auto next = stream.peek();
            if (next == '=') {
                stream.get();
            } else {
                // return Lex::Error("!");
                addError("! without =");
            }
            return Lex(Cmp::UnEqual);
        }

        case '/':
            stream.get();
            head_pos = stream.getPos();
            return Lex(Op::Div);
        case '>':
            stream.get();
            head_pos = stream.getPos();
            if (stream.peek() == '=') {
                stream.get();
                return Lex(Cmp::GreaterEq);
            } else {
                return Lex(Cmp::Greater);
            }

        case '<':
            stream.get();
            head_pos = stream.getPos();
            if (stream.peek() == '=') {
                stream.get();
                return Lex(Cmp::LessEq);
            } else {
                return Lex(Cmp::Less);
            }
        case '=':
            stream.get();
            head_pos = stream.getPos();
            if (stream.peek() == '=') {
                stream.get();
                return Lex(Cmp::Equal);
            } else {
                return Lex(Sep::Assign);
            }
        default: {
            string err;
            head_pos = stream.peekPos();
            while (!isspace(stream.peek())) {
                err.push_back(static_cast<char>(stream.get()));
            }
            err += " unknown token";
            return Lex::Error(err);
        }

    }
}


} // end namespace C0
