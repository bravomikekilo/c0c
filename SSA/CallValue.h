//
// Created by baomingkun on 18-11-29.
//

#ifndef C0_CALLVALUE_H
#define C0_CALLVALUE_H

#include "ValueBase.h"
#include "common.h"
#define FMT_HEADER_ONLY

#include "fmt/format.h"

namespace C0 {


class CallV: public ValueNode {
private:
    string func_name;
    vector<ValueE> args;  /// all arg value
    ValueE world;
    vector<ValueE> globals; /// all global virtual value

public:
    CallV(Region *region, string func_name, ValueE world, vector<ValueE>&& args)
        :ValueNode(region, Vop::Call) ,func_name(std::move(func_name)), world(world), args(std::move(args)) {}

    bool needReg() override {
        return false;
    }

    size_t num_vin() override {
        return 0;
    }

    ValueNode *&vin(size_t index) override {
        return <#initializer#>;
    }

    string str() override {
        return std::__cxx11::string();
    }

};

class RetV: public ValueNode {
private:
    vector<ValueE> globals;
    optional<ValueE> ret;
    ValueE world;

public:

    RetV(Region *region, optional<ValueE> ret,  vector<ValueE> &&globals)
        :ValueNode(region, Vop::Return), globals(std::move(globals)), ret(std::move(ret)) {}

};

class ReadV : public ValueNode {
protected:
    ValueE world;

public:
    ReadV(Vop op, Region *region, ValueE world)
        :ValueNode(region, op), world(world) {}

    size_t num_vin() override {
        return 1;
    }

    bool needReg() override {
        return false;
    }

    ValueNode *&vin(size_t index) override {
        if(index == 1) {
            return world;
        } else {
            throw std::out_of_range(fmt::format("{} overflow at index {}", vopToString(op), index));
        }
    }


};


class ReadIntV: public ReadV {

public:
    ReadIntV(Region *region, ValueE world)
        :ReadV(Vop::ReadInt, region, world) {}
};




class ReadCharV: public ReadV {

public:
    ReadCharV(Region *region, ValueE world)
        :ReadV(Vop::ReadChar, region, world) {}

};


class PrintV : public ValueNode {
protected:
    int str_id;
    ValueE val;
    ValueE world;
public:

    PrintV(Vop op, Region *region, ValueE world, ValueE val, int id)
        :ValueNode(region, op), world(world), val(val), str_id(id) {}

    size_t num_vin() override {
        return 1;
    }

    bool needReg() override {
        return false;
    }

    ValueNode *&vin(size_t index) override {
        if(index == 0) {
            return val;
        } else if(index == 1) {
            return world;
        } else {
            throw std::out_of_range(fmt::format("{} overflow at index {}", vopToString(op), index));
        }
    }

};

class PrintIntV: public PrintV {

public:
    PrintIntV(Region *region, ValueE world, ValueE val, int id = -1)
        :PrintV(Vop::PrintInt, region, world, val, id) {}

};

class PrintCharV: public PrintV {

public:
    PrintCharV(Region *region, ValueE world, ValueE val, int id = -1)
        :PrintV(Vop::PrintChar, region, world, val, id) {}

};


}

#endif //C0_CALLVALUE_H
