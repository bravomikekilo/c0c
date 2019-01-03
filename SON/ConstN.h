//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_CONSTN_H
#define C0_CONSTN_H

#include "Node.h"
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "SCCP.h"
#include "Sea.h"

namespace C0 {

class ConstN: public Node {
public:
    explicit ConstN(Nop op, UseE region): Node(op, 1) {
        uses[0] = region;
    }

    UseE SCCPIdentity(Sea &sea) override;

    string asText() override;

    bool needReg() override;


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

    // methods for optimization

    // methods for SCCP
    void SCCPType() override;

    string exprAsUse() override;

    bool asCode() override;

    bool same(const Node &other) override;


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

    // methods for SCCP
    void SCCPType() override;

    bool asCode() override;

    string exprAsUse() override;

    bool same(const Node &other) override;

};

class GlobalAddrN: public ConstN {
private:
    int offset;
    string label;

public:
    explicit GlobalAddrN(UseE region, int offset, string name)
        :ConstN(Nop::GlobalAddr, region),
         offset(offset), label(std::move(name)) {}

    int getV() {return offset;}

    string str() override {
        auto base = Node::str();
        return fmt::format("{}:{}+{}", base, label, offset);
    }

    void SCCPType() override;

    UseE sameLabel(int offset, Sea &sea);

    bool asCode() override;

    string exprAsUse() override;

    bool same(const Node &other) override;

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

    void SCCPType() override;

    bool asCode() override;

    string exprAsUse() override;

    bool same(const Node &other) override;

};


}

#endif //C0_CONSTN_H
