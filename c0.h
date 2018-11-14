#pragma once

namespace C0 {


enum class Op {
    Ind,  // virtual op for index operation
    Add,  // +
    Sub,  // -
    Mul,  // *
    Div,  // /
};

enum class Cmp {
    Greater, // >
    Less,    // <
    GreaterEq,  // >=
    LessEq,     // <=
    Equal,      // ==
    UnEqual,    // !=
};

enum class Sep {
    LPar,  // (
    RPar,  // )
    LBar,  // [
    RBar,  // ]
    LCur,  // {
    RCur,  // }
    Comma, // ,
    Colon, // :
    Semicolon, // ;
    Assign,    // =
};

enum class Keyword {
    CONST,
    IF,
    ELSE,
    DO,
    WHILE,
    FOR,
    SWITCH,
    CASE,
    SCANF,
    PRINTF,
    RETURN,
    INT,
    CHAR,
    VOID,
};


}

