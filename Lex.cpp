#include "Lex.h"

namespace C0 {
Lex Lex::String(std::string& str) {
    return Lex(str, LexKind::String);
}

Lex Lex::Ident(std::string& str) {
    return Lex(str, LexKind::Ident);
}

Lex Lex::Error(std::string str) {
    return Lex(str, LexKind::Error);
}

}
