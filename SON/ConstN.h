//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_CONSTN_H
#define C0_CONSTN_H

#include "Node.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class ConstN: public Node {
public:
    explicit ConstN(Nop op, UseE region): Node(op, 1) {
        uses[0] = region;
    }
};

class ConstIntN: public ConstN {
private:
    int v;
public:
    explicit ConstIntN(UseE region, int val): ConstN(Nop::Int, region), v(val) {}
    int getV() {return v;}

    string str() override {
        auto base = Node::str();
        return fmt::format("{}:{}", base, v);
    }

};

class ConstCharN: public ConstN {
private:
    char v;
public:
    explicit ConstCharN(UseE region, char ch): ConstN(Nop::Char, region), v(ch) {}

    char getV() {return v;}

    string str() override {
        auto base = Node::str();
        return fmt::format("{}:{}", base, v);
    }

};

class GlobalAddrN: public ConstN {
private:
    int offset;
public:
    explicit GlobalAddrN(UseE region, int offset)
        :ConstN(Nop::GlobalAddr, region), offset(offset) {}

    int getV() {return offset;}

    string str() override {
        auto base = Node::str();
        return fmt::format("{}:{}", base, offset);
    }

};

class StackSlotN: public ConstN {
private:
    int offset;
public:
    explicit StackSlotN(UseE region, int offset)
        :ConstN(Nop::StackSlot, region), offset(offset) {}

    int getV() {return offset;}

    string str() override {
        auto base = Node::str();
        return fmt::format("{}:{}", base, offset);
    }

};

}

#endif //C0_CONSTN_H