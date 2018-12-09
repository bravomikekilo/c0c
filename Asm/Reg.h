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

    enum Kind {
        Fp,
        Sp,
        T,
        S,
        A,
        V,
        Z,
        Ra,
        Int,
    };

    static string KindToStr(Kind k) {
        switch (k) {
            case Fp:
                return "Fp";
            case Sp:
                return "Sp";
            case T:
                return "T";
            case S:
                return "S";
            case A:
                return "A";
            case V:
                return "V";
            case Z:
                return "Z";
            case Ra:
                return "Ra";
            case Int:
                return "Int";
        }
    }

    virtual Kind getKind() = 0;

};


class FpReg : public Reg {
public:
    string toString() override {
        return "$fp";
    }

    Reg *clone() override {
        return new FpReg();
    }

    Kind getKind() override { return Reg::Kind::Fp; }
};

class SpReg : public Reg {
public:
    string toString() override {
        return "$sp";
    }

    Reg *clone() override {
        return new SpReg();
    }

    Kind getKind() override { return Reg::Kind::Sp; }
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

    Kind getKind() override { return Reg::Kind::T; }
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

    Kind getKind() override { return Reg::Kind::S; }
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

    Kind getKind() override { return Reg::Kind::V; }
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

    Kind getKind() override { return Kind::A; }
};

class ZReg : public Reg {
public:
    string toString() override {
        return "$0";
    }

    Reg *clone() override {
        return new ZReg(*this);
    }

    Kind getKind() override { return Kind::Z; }
};

class RaReg : public Reg {
public:
    string toString() override {
        return "$ra";
    }

    Reg *clone() override {
        return new RaReg();
    }

    Kind getKind() override { return Kind::Ra; }
};

class IntReg : public Reg {
protected:
    int val;

public:
    string toString() override {
        return std::to_string(val);
    }

    optional<int> constVal() override {
        return {val};
    }

    Reg *clone() override {
        return new IntReg(*this);
    }

    IntReg(int x) : val(x) {}

    Kind getKind() override { return Kind::Int; }

};


}

