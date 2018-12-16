//
// Created by baomingkun on 18-12-6.
//

#ifndef C0_OPT_H
#define C0_OPT_H

#include "NLoad.h"


namespace C0 {

class RegionN;
class StopN;
class Node;

class Sea;

    /// the SCCP optimization, must be performed when def-use chain is usable

class SCCPOptimizer {

public:
    class T : public NLoad {
    public:
        enum Height {Top, Constant, Bottom};
        enum Height height;
        enum CType {Pointer, Label, Value};
        enum CType type;
        optional<string> label;
        int constant;

        T(Height h, CType t, int c)
            :height(h), type(t), constant(c) {}

        T(Height h, CType t, int c, const string &label)
            :height(h), type(t), constant(c), label(label) {}

        bool operator==(const T &other) const {
            if(height != other.height) return false;
            if(height == Constant) {
                bool base = type == other.type && constant == other.constant;
                if(type == CType::Label) {
                    return base && label == other.label;
                } else {
                    return base;
                }
            } else {
                return true;
            }
        }

        bool operator!=(const T &other) const {
            return !(*this == other);
        }

        string toStr() override;
    };

private:
    vector<Node *> payloads;

    void initialize(StopN *stop);

public:
    void analysis(pair<RegionN *, StopN *> graph);
    void transform(pair<RegionN *, StopN *> graph, Sea &sea);
    void freePayloads();

    ~SCCPOptimizer() {
        freePayloads();
    }
};


}

#endif //C0_OPT_H
