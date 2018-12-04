//
// Created by baomingkun on 18-11-29.
//

#ifndef C0_MISCVALUE_H
#define C0_MISCVALUE_H

#include "ControlBase.h"
#include "ValueBase.h"
#include "common.h"

namespace C0 {


class UndefV : public ValueNode {
public:
    explicit UndefV(Region *region)
        :ValueNode(region, Vop::Undef) {}

    size_t num_vin() override {
        return 0;
    }

    bool needReg() override {
        return false;
    }

    ValueNode *&vin(size_t index) override {
        throw std::out_of_range("undef has no using");
    }

    string str() override {
        return "undef";
    }
};


class CopyV : public ValueNode {
private:
    ValueE src;
public:
    CopyV(Region *region, ValueE src)
            : ValueNode(region, Vop::Copy), src(src) {}

    size_t num_vin() override {
        return 1;
    }

    ValueNode *&vin(size_t index) override {
        if(index == 0) return src;
        throw index;
    }

    bool needReg() override {
        return true;
    }

    string str() override {
        return "Copy";
    }

};


class PhiV: public ValueNode {
protected:
    vector<ValueE> values;

public:
    PhiV(Region *region, vector<ValueE> &&vals)
        :ValueNode(region, Vop::Phi), values(std::move(vals)) {}

    size_t num_vin() override {
        return values.size();
    }

    ValueE &vin(size_t index) override {
        return values[index];
    }

    void addValue(ValueE val) {
        values.push_back(val);
    }

    string str() override {
        return "Phi";
    }

};


class SelectV : public ValueNode {
protected:
    vector <ValueE> values;
public:
    SelectV(Region *region, vector <ValueE> &&vals)
            : ValueNode(region, Vop::Select), values(std::move(vals)) {}

    size_t num_vin() override {
        return values.size();
    }

    ValueNode *&vin(size_t index) override {
        return values[index];
    }

    bool needReg() override {
        return false;
    }

    string str() override {
        return "Select";
    }

};


class ComposeV : public ValueNode {
protected:
    vector<SelectV *> selects;

public:
    ComposeV(Region *region, vector<SelectV *> &&selects)
            : ValueNode(region, Vop::Compose), selects(std::move(selects)) {}

    string str() override {
        return "Compose";
    }
};


class ProjWorld : public ValueNode {
private:
    ValueE up;

public:
    ProjWorld(Region *region, ValueE up)
        : ValueNode(region, Vop::ProjWorld), up(up) {}

    string str() override {
        return "ProjWorld";
    }
};

class InitWorld : public ValueNode {

public:
    explicit InitWorld(Region *start)
        : ValueNode(start, Vop::InitWorld) {}

    string str() override {
        return "InitWorld";
    }
};


class ProjArgN : public ValueNode {

public:
    explicit ProjArgN(Region *start)
        :ValueNode(start, Vop::ProjArg) {}

    string str() override {
        return "ProjArg";
    }
};


class ProjRetN : public ValueNode {
private:
    ValueE call;

public:
    ProjRetN(Region *region, ValueE call)
        :ValueNode(region, Vop::ProjRet), call(call) {}

    string str() override {
        return "ProjRetN";
    }

    size_t num_vin() override {
        return 1;
    }

    bool needReg() override {
        return true;
    }

    ValueNode *&vin(size_t index) override {
        return <#initializer#>;
    }
};

}
#endif //C0_MISCVALUE_H
