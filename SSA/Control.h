//
// Created by baomingkun on 18-11-22.
//

#ifndef C0_CONTROLNODE_H
#define C0_CONTROLNODE_H

#include "ControlBase.h"
#include "ValueBase.h"
#include "common.h"
#include <exception>

namespace C0 {

class Start : public ControlNode {
public:

    Start(): ControlNode(Cop::Start) {}
    size_t num_cin() override {
        return 0;
    }

    ControlE &cin(size_t index) override {
        throw std::out_of_range("start has no in edge at " + std::to_string(index));
    }


};


class Region: public ControlNode {
protected:
    vector<ControlE> pred;

public:
    explicit Region(vector<ControlE>&& pred): ControlNode(Cop::Region), pred(std::move(pred)) {}

    size_t num_cin() override {
        return pred.size();
    }

    ControlE &cin(size_t index) override {
        if(index < pred.size()) {
            return pred[index];
        }
        throw std::out_of_range("region node overflow " + std::to_string(index));
    }

};

class End : public Region {
protected:
    vector<ValueE> globals;
    ValueE world;
    ValueE ret;

public:
    explicit End(vector<ControlE> &&pred, ValueE world, ValueE ret=nullptr)
        :Region(std::move(pred)), world(world), ret(ret) {}

    ValueE getRet() {return ret;}

    bool hasRet() {return ret != nullptr;}
    ValueE getWorld() {return world;}

    const vector<ValueE> &getGlobals() {return globals;}

    size_t num_vin() override {
        return globals.size() + 1 + (ret == nullptr ? 0 : 1);
    }

    ValueE &vin(size_t index) override {
        if(index >= num_vin()) {
            throw std::out_of_range(fmt::format("End node overflow in valueE at index {}", index));
        }

        if(index < globals.size()) {
            return globals[index];
        }
        index -= globals.size();
        if(index) {
            return ret;
        } else {
            return world;
        }
    }

};

class If: public ControlNode {
    ValueE predict;
    ControlE up;
public:

    If(ValueE predict, ControlE up)
        :ControlNode(Cop::If),
         predict(predict),
         up(up) {}

    ValueE &pred() {return predict;}

    size_t num_cin() override {
        return 1;
    }

    ControlE &cin(size_t index) override {
        if(index == 0) {
            return up;
        }
        throw std::out_of_range("if node ControlE overflow " + std::to_string(index));
    }

    size_t num_vin() override {return 1;}
    ValueE &vin(size_t index) override {
        if(index == 0) {
            return predict;
        } else {
            throw std::out_of_range(fmt::format("{} ValueE overflow at {}", copToString(op), index));
        }
    }
};

class IfProj: public ControlNode {
    ControlE up;
    bool branch;

public:

    IfProj(If *up, bool v)
        :ControlNode(Cop::IfProj),
         up(up), branch(v) {}

    size_t num_cin() override {return 1;}

    ControlE &cin(size_t index) override {
        if(index == 0) {
            return up;
        }
        throw std::out_of_range("if projection overflow " + std::to_string(index));
    }
};


};

#endif //C0_CONTROLNODE_H
