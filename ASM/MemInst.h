
#pragma once

#include "common.h"
#include "Reg.h"
#include "InstBase.h"

#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class MemInst : public InstBase {
private:
    optional<string> label;
    optional<Reg> reg;
    int offset;
    Reg src;

protected:
    virtual string op() = 0;

public:
    string toString(const unordered_map<string, int> &funcs) override;
    MemInst(Reg src, string label, optional<Reg> reg, int offset)
        :label({label}), reg(std::move(reg)), src(src), offset(offset)  { }

    MemInst(Reg src, optional<Reg> reg, int offset)
        :label({}), reg(std::move(reg)), src(src), offset(offset)  { }

};



class LaInst : public MemInst {

protected:
    string op() override {
        return "la";
    }

public:
    LaInst(Reg dst, string label, optional<Reg> reg = optional<Reg>(), int offset = 0)
        :MemInst(dst, std::move(label), std::move(reg),  offset)  { }

    LaInst(Reg dst, optional<Reg> reg, int offset = 0)
        :MemInst(dst, std::move(reg), offset)  { }

};


    
class StoreInst : public MemInst {

protected:
    virtual char width() = 0;

    string op() override {
        string ret = "s";
        ret.push_back(width());
        return ret;
    }

public:

    StoreInst(Reg src, string label, optional<Reg> reg = optional<Reg>(), int offset = 0)
        :MemInst(src, std::move(label), std::move(reg),  offset)  { }

    StoreInst(Reg src, optional<Reg> reg, int offset)
        :MemInst(src, std::move(reg), offset)  { }
};

class SwInst : public StoreInst {

protected:
    char width() override { return 'w'; }

public:
    SwInst(Reg src, string label, optional<Reg> reg = optional<Reg>(), int offset = 0)
        :StoreInst(src, std::move(label), std::move(reg),  offset)  { }

    SwInst(Reg src, optional<Reg> reg, int offset = 0)
        :StoreInst(src, std::move(reg), offset)  { }


};

class SbInst : public StoreInst {

protected:
    char width() override { return 'b'; }

public:
    SbInst(Reg src, string label, optional<Reg> reg = optional<Reg>(), int offset = 0)
        :StoreInst(src, std::move(label), std::move(reg),  offset)  { }

    SbInst(Reg src, optional<Reg> reg, int offset = 0)
        :StoreInst(src, std::move(reg), offset)  { }


};


class LoadInst : public MemInst {

protected:
    virtual char width() = 0;

    string op() override {
        string ret = "l";
        ret.push_back(width());
        return ret;
    }


public:

    LoadInst(Reg dst, string label, optional<Reg> reg, int offset)
        :MemInst(dst, std::move(label), std::move(reg),  offset)  { }

    LoadInst(Reg dst, optional<Reg> reg, int offset)
        :MemInst(dst, std::move(reg), offset)  { }


};

class LwInst : public LoadInst {

protected:
    char width() override { return 'w'; }

public:

    LwInst(Reg dst, string label, optional<Reg> reg = optional<Reg>(), int offset = 0)
        :LoadInst(dst, std::move(label), std::move(reg),  offset)  { }

    LwInst(Reg dst, optional<Reg> reg, int offset = 0)
        :LoadInst(dst, std::move(reg), offset)  { }



};

class LbInst : public LoadInst {

protected:
    char width() override { return 'b'; }

public:

    LbInst(Reg dst, string label, optional<Reg> reg = optional<Reg>(), int offset = 0)
        :LoadInst(dst, std::move(label), std::move(reg),  offset)  { }

    LbInst(Reg dst, optional<Reg> reg, int offset = 0)
        :LoadInst(dst, std::move(reg), offset)  { }

};


} // end namespace C0

