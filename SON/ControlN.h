//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_CONTROLN_H
#define C0_CONTROLN_H

#include "Node.h"
#include "common.h"

namespace C0 {

class RegionN : public Node {
public:
    explicit RegionN(const vector<UseE> &use) :Node(Nop::Region, use.size()) {
       for(int i = 0; i < num_uses; ++i) {
           uses[i] = use[i];
       }
    }

    RegionN() : Node(Nop::Region, 0) {}

    explicit RegionN(UseE use) :Node(Nop::Region, 1) {
        uses[0] = use;
    }

    explicit RegionN(UseE one, UseE two) :Node(Nop::Region, 2) {
        uses[0] = one;
        uses[1] = two;
    }
};

class IfN : public Node {
public:
    IfN(UseE up, UseE predict) :Node(Nop::If, 2) {
        uses[0] = up;
        uses[1] = predict;
    }
};

class IfProjN : public Node {
private:
    bool branch;

public:
    explicit IfProjN(UseE up, bool branch) :Node(Nop::IfProj, 1), branch(branch) {
        uses[0] = up;
    }
};


class StopN : public Node {
public:
    explicit StopN(UseE last_block, const vector<UseE> &globals, UseE world)
        :Node(Nop::Stop,  globals.size() + 2) {
        int j = 2;
        uses[0] = last_block;
        uses[1] = world;

        for(int i = 0; i < globals.size(); ++i, ++j) {
            uses[j] = globals[i];
        }

    }

    explicit StopN(UseE last_block, const vector<UseE> &globals, UseE world, UseE ret)
        :Node(Nop::Stop, globals.size() + 3) {
        int j = 3;
        uses[0] = last_block;
        uses[1] = ret;
        uses[2] = world;

        for(int i = 0; i < globals.size(); ++i, ++j) {
            uses[j] = globals[i];
        }

    }

};





}

#endif //C0_CONTROLN_H
