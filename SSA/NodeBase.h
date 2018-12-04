//
// Created by baomingkun on 18-11-22.
//

#ifndef C0_NODEBASE_H
#define C0_NODEBASE_H


#include "common.h"

namespace C0 {

class NodeBase {
public:
    void *payload = nullptr;

    template <typename T>
    T *Payload() {
        return (T *)payload;
    }

    virtual size_t num_in() = 0;
    virtual NodeBase *in(size_t index) = 0;

};






}

#endif //C0_NODEBASE_H
