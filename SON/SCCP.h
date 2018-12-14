//
// Created by baomingkun on 18-12-6.
//

#ifndef C0_OPT_H
#define C0_OPT_H

#include "SON.h"

namespace C0 {

    /// the SCCP optimization, must be performed when def-use chain is usable

class SCCPOptimizer {

public:
    struct T {
        enum {Top, Constant, Bottom} height;
        enum {Pointer, Label, Value} type;
        int constant;

        bool operator==(const T &other) const {
            if(height != other.height) return false;
            if(height == Constant) {
                return type == other.type && constant == other.constant;
            } else {
                return true;
            }
        }

        bool operator!=(const T &other) const {
            return !(*this == other);
        }
    };

private:
    vector<T*> payloads;

    void initialize(StopN *stop);
    void freePayloads();

public:
    void opt(pair<RegionN *, StopN *> graph);

    ~SCCPOptimizer() {
        freePayloads();
    }
};


}

#endif //C0_OPT_H
