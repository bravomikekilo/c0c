//
// Created by baomingkun on 18-12-12.
//

#ifndef C0_REG_H
#define C0_REG_H

#include "common.h"

namespace C0 {



class Reg {
public:
    enum RegK {
        I,
        A, // argument register
        S, // saved register
        T,
        V,
        Z,
        At,
        Ra,
        Sp
    };

private:
    RegK k;
    int val;

    Reg(int number, RegK k): val(number), k(k) {}

public:

    const int NumS = 9;
    const int NumT = 10;
    const int NumA = 4;
    const int NumV = 2;

    string toStr();


    RegK getK() {return k;}

    bool isInt() {return k == I;}

    int getN() {return val;}

    static Reg AReg(int n) {
        return Reg(4 + n, RegK::A);
    }

    static Reg SReg(int n) {
        return Reg(16 + n, RegK::S);
    }

    static Reg TReg(int n) {
        if(n < 8) {
            return Reg(8 + n, RegK::T);
        } else {
            return Reg(24 + n - 8, RegK::T);
        }
    }

    static Reg VReg(int n) {
        return Reg(n + 2, RegK::V);
    }

    static Reg SpReg() {
        return Reg(30, RegK::Sp);
    }

    static Reg RaReg() {
        return Reg(31, RegK::Ra);
    }

    static Reg R(int number);

    static Reg IReg(int number) {
        return Reg(number, RegK::I);
    }


};


}


#endif //C0_REG_H
