#pragma once

#include "common.h"

namespace C0 {

class Reg {
public:
    virtual string toString() = 0;
    virtual ~Reg() = default;
    virtual optional<int> constVal() {
        return {};
    }
};


class FpReg : public Reg {
public:
    string toString() {
        return "$fp";
    }
};

class SpReg : public Reg {
public:
    string toString() {
        return "$sp";
    }
};


class TReg : public Reg {
private:
    int num;

public:
    TReg(int n) : num(n) {}

    string toString() override {
        string ret = "$t";
        ret.push_back(num + '0');
        return ret;
    }
};

class SReg : public Reg {
private:
    int num;
public:
    SReg(int n) : num(n) {}
    string toString() override {
        string ret = "$s";
        ret.push_back(num + '0');
        return ret;
    }
};

class VReg : public Reg {
private:
    int num;

public:
    VReg(int n) : num(n) {}
    string toString() override {
        string ret = "$v";
        ret.push_back(num + '0');
        return ret;
    }
};

class AReg : public Reg {
private:
    int num;
public:
    AReg(int n) : num(n) {}
    string toString() override {
        string ret = "$a";
        ret.push_back(num + '0');
        return ret;
    }
};

class ZReg : public Reg {
public:
    string toString() override {
        return "$0";
    }
};

class RaReg : public Reg {
public:
    string toString() override {
        return "$ra";
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
};



}

