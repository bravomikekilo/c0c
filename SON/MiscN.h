//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_MISCN_H
#define C0_MISCN_H

#include "Node.h"

namespace C0 {

/// represent initial value of World
class InitWorldN : public Node {
public:
    explicit InitWorldN(UseE start_region): Node(Nop::InitWorld, 1) {
        uses[0] = start_region;
    }

    void SCCPType() override;
};

/// represent initial value of a global, so it can't be a constant
class InitGlobalN : public Node {
private:
    string name;
public:
    explicit InitGlobalN(UseE start_region, string name)
        : Node(Nop::InitGlobal, 1), name(std::move(name)) {
        uses[0] = start_region;
    }

    string str() override {
        return fmt::format("{}:{}", Node::str(), name);
    }

    void SCCPType() override;
};

/// represent initial value of a argument, so it can't be a constant
class ProjArgN : public Node {

private:
    size_t n;

public:
    explicit ProjArgN(UseE start_region, size_t n): Node(Nop::ProjArg, 1), n(n) {
        uses[0] = start_region;
    }

    size_t getN() {return n;}

    string str() override {
        return Node::str() + ":" + std::to_string(n);
    }

    void SCCPType() override;

    string exprAsUse() override;

};


/// phi function
class PhiN : public Node {

public:
    PhiN(UseE region, const vector<UseE> &ups): Node(Nop::Phi, ups.size() + 1) {
        uses[0] = region;
        int j = 1;
        for(int i = 0; i < ups.size(); ++i, ++j) {
            uses[j] = ups[i];
        }
    }

    PhiN(UseE region, UseE left, UseE right): Node(Nop::Phi, 3) {
        uses[0] = region;
        uses[1] = left;
        uses[2] = right;
    }

    explicit PhiN(UseE region): Node(Nop::Phi, 1) {
        uses[0] = region;
    }

    void SCCPType() override;

    UseE SCCPIdentity(Sea &sea) override;

};


class UndefN : public Node {

public:
    explicit UndefN(UseE region) : Node(Nop::Undef, 0) { }
    explicit UndefN() : Node(Nop::Undef, 0) { }

    void SCCPType() override;

    bool asCode() override;

    string exprAsUse() override;

};

// copy will not show up before sccp
// it is only used in the register allocation phrase
class CopyN : public Node {
public:
    CopyN(UseE region, UseE src) : Node(Nop::Copy, 2) {
        uses[0] = region;
        uses[1] = src;
    }

};


}

#endif //C0_MISCN_H
