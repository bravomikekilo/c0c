#pragma once

#include "common.h"

namespace C0 {

class Reg {
public:
    virtual string toString() = 0;
    virtual ~Reg() = default;
    virtual Reg *clone() = 0;
    virtual optional<int> constVal() {
        return {};
    }

};


class FpReg : public Reg {
public:
    string toString() override {
        return "$fp";
    }

    Reg *clone() override {
        return new FpReg();
    }
};

class SpReg : public Reg {
public:
    string toString() override {
        return "$sp";
    }

    Reg *clone() override {
        return new SpReg();
    }
};


class TReg : public Reg {
private:
    size_t num;

public:
    TReg(size_t n) : num(n) {}

    string toString() override {
        string ret = "$t";
        ret.push_back(num + '0');
        return ret;
    }

    Reg *clone() override {
        return new TReg(*this);
    }
};

class SReg : public Reg {
private:
    size_t num;
public:
    SReg(size_t n) : num(n) {}
    string toString() override {
        string ret = "$s";
        ret.push_back(num + '0');
        return ret;
    }

    Reg *clone() override {
        return new SReg(*this);
    }
};

class VReg : public Reg {
private:
    size_t num;

public:
    VReg(size_t n) : num(n) {}
    string toString() override {
        string ret = "$v";
        ret.push_back(num + '0');
        return ret;
    }

    Reg *clone() override {
        return new VReg(*this);
    }
};

class AReg : public Reg {
private:
    size_t num;

public:
    AReg(size_t n) : num(n) {}
    string toString() override {
        string ret = "$a";
        ret.push_back(num + '0');
        return ret;
    }
    Reg *clone() override {
        return new AReg(*this);
    }
};

class ZReg : public Reg {
public:
    string toString() override {
        return "$0";
    }

    Reg *clone() override {
        return new ZReg(*this);
    }
};

class RaReg : public Reg {
public:
    string toString() override {
        return "$ra";
    }

    Reg *clone() override {
        return new RaReg();
    }
};

class IntReg : public Reg {
protected:
    int val;

public:
    string toString() override {
        return std::to_string(val);
    }
    
    optional<int> constVal() override {
        return { val };
    }

    Reg *clone() override {
        return new IntReg(*this);
    }

    IntReg(int x): val(x) {}

};



}

