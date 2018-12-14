//
// Created by baomingkun on 18-12-12.
//

#include "Reg.h"

namespace C0 {

string RegKtoStr(Reg::RegK reg) {
    switch (reg) {
        case Reg::I:
            return "Int";
        case Reg::A:
            return "a";
        case Reg::S:
            return "s";
        case Reg::T:
            return "t";
        case Reg::V:
            return "v";
        case Reg::At:
            return "at";
        case Reg::Ra:
            return "ra";
        case Reg::Sp:
            return "sp";
        case Reg::Z:
            return "0";
    }
}

std::string C0::Reg::toStr() {
    if (k == I) {
        return std::to_string(val);
    }

    string base = "$" + RegKtoStr(k);
    if(k == RegK::Ra || k == RegK::Sp || k == RegK::At || k == RegK::Z) {
        return base;
    } else {
        return base + std::to_string(val);
    }
}

Reg::RegK RegKfromN(int n) {
    if(n == 0) {
        return Reg::RegK::Z;
    } else if(n == 1) {
        return Reg::RegK::At;
    } else if(n <= 3) {
        return Reg::RegK::V;
    } else if(n <= 7) {
        return Reg::RegK::A;
    } else if(n <= 15) {
        return Reg::RegK::T;
    } else if(n <= 23) {
        return Reg::RegK::S;
    } else if(n <= 25) {
        return Reg::RegK::T;
    } else if(n <= 29) {
        return Reg::RegK::Sp;
    } else if(n <= 30) {
        return Reg::RegK::S;
    } else if(n <= 31) {
        return Reg::RegK::Ra;
    } else {
        return Reg::RegK::I;
    }
}

Reg Reg::R(int number) {
    auto k = RegKfromN(number);
    return Reg(number, k);
}

}
