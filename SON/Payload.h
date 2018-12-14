//
// Created by baomingkun on 18-12-12.
//

#ifndef C0_PAYLOAD_H
#define C0_PAYLOAD_H

#include "common.h"

namespace C0 {


struct Payload {
    enum Kind {
        BuildContext,
        SCCP
    } kind;
    virtual string toStr() {
        return "";
    };

    Payload(Kind kind): kind(kind) {}
};







}

#endif //C0_PAYLOAD_H
