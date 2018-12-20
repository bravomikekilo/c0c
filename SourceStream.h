//
// Created by baomingkun on 18-12-20.
//

#ifndef C0_SOURCESTREAM_H
#define C0_SOURCESTREAM_H

#include "common.h"
#include <sstream>
#include "Pos.h"

namespace C0 {


class SourceStream {
private:
    std::stringstream stream;
    Pos curr_pos;
    bool at_end = false;
public:

    explicit SourceStream(const string &src): stream(src), curr_pos{1, 0} {}

    Pos getPos() const {return curr_pos;}
    Pos peekPos() const ;
    int get();
    int peek();
};

}


#endif //C0_SOURCESTREAM_H
