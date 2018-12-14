//
// Created by baomingkun on 18-12-12.
//

#ifndef C0_INSTBASE_H
#define C0_INSTBASE_H

#include "common.h"

namespace C0 {

class InstBase {
public:
    virtual string toString(const unordered_map<string, int> &funcs) = 0;

};
}


#endif //C0_INSTBASE_H
