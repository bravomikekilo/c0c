#pragma once

#include "NodeBase.h"
#include "ValueBase.h"
#include <exception>

#define FMT_HEADER_ONLY
#include "fmt/format.h"


namespace C0 {
    
enum class Cop {
    Start,
    End,
    Region,
    If,
    IfProj,
};

string copToString(Cop op) {
    switch(op) {
        case Cop::Start:
            return "Start";
        case Cop::End:
            return "End";
        case Cop::Region:
            return "Region";
        case Cop::If:
            return "If";
        case Cop::IfProj:
            return "IfProj";
    }
}

class ControlNode;

using ControlE = ControlNode *;

class ControlNode : public NodeBase {
protected:
    Cop op;

public:
    explicit ControlNode(Cop op): op(op) {}

    Cop getOp() {
        return op;
    }

    size_t num_in() override {
        return num_cin() + num_vin();
    };

    NodeBase *in(size_t index) override {
        if(index < num_cin()) return cin(index);

        return vin(index - num_cin());
    }

    virtual size_t num_cin() = 0;
    virtual ControlE &cin(size_t index) = 0;

    virtual size_t num_vin() {
        return 0;
    };

    virtual ValueE &vin(size_t index) {
        throw std::out_of_range(fmt::format("{} has not used value node", copToString(op));
    };


};


} // end namespace C0


