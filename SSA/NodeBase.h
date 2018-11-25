//
// Created by baomingkun on 18-11-22.
//

#ifndef C0_NODEBASE_H
#define C0_NODEBASE_H


namespace C0 {

class NodeBase {
public:
    void *payload = nullptr;

    template <typename T>
    T *Payload() {
        return (T *)payload;
    }

};






}

#endif //C0_NODEBASE_H
