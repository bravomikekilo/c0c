//
// Created by baomingkun on 18-12-20.
//

#include "SourceStream.h"

namespace C0 {

int SourceStream::get() {
    auto ch = stream.get();
    if(at_end) {
        at_end = false;
        curr_pos.col = 0;
        curr_pos.ln++;
    }

    if(ch == '\n') {
        at_end = true;
    }
    curr_pos.col++;
    return ch;
}


int SourceStream::peek() {
    return stream.peek();
}

Pos SourceStream::peekPos() const {
    auto ret = curr_pos;

    if(at_end) {
        ret.col = 1;
        ret.ln++;
    } else {
        ret.col++;
    }

    return ret;
}



}