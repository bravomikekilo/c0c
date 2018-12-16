//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_CALL_H
#define C0_CALL_H

#include "Node.h"

namespace C0 {

class CallN : public Node {
private:
    string func_name;

public:
    CallN(string func, UseE region, UseE world, const vector<UseE> &args, const vector<UseE> &globals)
            : Node(Nop::Call, args.size() + globals.size() + 2), func_name(std::move(func)) {
        int j = 2;

        uses[0] = region;
        uses[1] = world;

        for (int i = 0; i < args.size(); ++i, ++j) {
            uses[j] = args[i];
        }

        for (int i = 0; i < globals.size(); ++i, ++j) {
            uses[j] = globals[i];
        }

    }

    CallN(string func, UseE region, UseE world, const vector<UseE> &args)
            : Node(Nop::Call, args.size() + 2), func_name(std::move(func)) {
        int j = 2;

        uses[0] = region;
        uses[1] = world;

        for (int i = 0; i < num_uses; ++i, ++j) {
            uses[j] = args[i];
        }
    }

    CallN(string func, UseE region, UseE world)
            : Node(Nop::Call, 2), func_name(std::move(func)) {
        uses[0] = region;
        uses[1] = world;
    }

    string str() override {
        return fmt::format("{}:{}", Node::str(), func_name);
    }

    void SCCPType() override;

    void SCCPType(ProjN *n) override;

};


class PrintN: public Node {
private:
    optional<int> str_id;

public:
    PrintN(Nop op, UseE region, optional<int> str_id, UseE world): Node(op, 2), str_id(std::move(str_id)) {
        uses[0] = region;
        uses[1] = world;
    }

    PrintN(Nop op, UseE region, optional<int> str_id, UseE world, UseE arg): Node(op, 3), str_id(std::move(str_id)) {
        uses[0] = region;
        uses[1] = world;
        uses[2] = arg;
    }

    void SCCPType() override;

    UseE SCCPIdentity(Sea &sea) override;
};

class PrintIntN: public PrintN {
public:
    PrintIntN(UseE region, optional<int> str_id, UseE world)
        :PrintN(Nop::PrintInt, region, std::move(str_id), world) {}

    PrintIntN(UseE region, optional<int> str_id,  UseE world, UseE arg)
        :PrintN(Nop::PrintInt, region, std::move(str_id), world, arg) {}
};

class PrintCharN: public PrintN {
public:
    PrintCharN(UseE region, optional<int> str_id, UseE world)
        :PrintN(Nop::PrintChar, region, std::move(str_id), world) {}

    PrintCharN(UseE region, optional<int> str_id, UseE world, UseE arg)
        :PrintN(Nop::PrintChar, region, std::move(str_id), world, arg) {}
};


class ReadN: public Node {

public:
    ReadN(Nop op, UseE region, UseE world): Node(op, 2) {
        uses[0] = region;
        uses[1] = world;
    }

    void SCCPType() override;

    void SCCPType(ProjN *n) override;

    UseE SCCPIdentity(Sea &sea) override;

    UseE SCCPIdentity(Sea &sea, ProjN *projection) override;

};


class ReadIntN: public ReadN {
public:
    explicit ReadIntN(UseE region, UseE world): ReadN(Nop::ReadInt, region, world) {}
};


class ReadCharN: public ReadN {
public:
    explicit ReadCharN(UseE region, UseE world): ReadN(Nop::ReadChar, region, world) {}
};

}

#endif //C0_CALL_H
