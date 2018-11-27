
#pragma once

#include "common.h"
#include "Reg.h"
#include "Inst.h"

#define FMT_HEADER_ONLY
#include "fmt/format.h"

namespace C0 {

class MemInst : public Inst {
private:
    optional<string> label;
    unique_ptr<Reg> reg;
    int offset;
    unique_ptr<Reg> src;

protected:
    virtual string op() = 0;

public:
    string toString() override;
    MemInst(unique_ptr<Reg>&& src, string label, unique_ptr<Reg>&& reg, int offset)
        :label({label}), reg(std::move(reg)), src(std::move(src)), offset(offset)  { } 

    MemInst(unique_ptr<Reg>&& src, unique_ptr<Reg>&& reg, int offset)
        :label({}), reg(std::move(reg)), src(std::move(src)), offset(offset)  { } 

};



class LaInst : public MemInst {

protected:
    string op() override {
        return "la";
    }

public:
    LaInst(unique_ptr<Reg>&& dst, string label, unique_ptr<Reg>&& reg = nullptr, int offset = 0)
        :MemInst(std::move(dst), std::move(label), std::move(reg),  offset)  { }

    LaInst(unique_ptr<Reg>&& dst, unique_ptr<Reg>&& reg, int offset = 0)
        :MemInst(std::move(dst), std::move(reg), offset)  { }

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

    StoreInst(unique_ptr<Reg>&& src, string label, unique_ptr<Reg>&& reg, int offset)
        :MemInst(std::move(src), label, std::move(reg),  offset)  { } 

    StoreInst(unique_ptr<Reg>&& src, unique_ptr<Reg>&& reg, int offset)
        :MemInst(std::move(src), std::move(reg), offset)  { } 
};

class SwInst : public StoreInst {

protected:
    char width() override { return 'w'; }

public:
    SwInst(unique_ptr<Reg>&& src, string label, unique_ptr<Reg>&& reg = nullptr, int offset = 0)
        :StoreInst(std::move(src), label, std::move(reg),  offset)  { } 

    SwInst(unique_ptr<Reg>&& src, unique_ptr<Reg>&& reg, int offset = 0)
        :StoreInst(std::move(src), std::move(reg), offset)  { } 


};

class SbInst : public StoreInst {

protected:
    char width() override { return 'b'; }

public:
    SbInst(unique_ptr<Reg>&& src, string label, unique_ptr<Reg>&& reg = nullptr, int offset = 0)
        :StoreInst(std::move(src), std::move(label), std::move(reg),  offset)  { }

    SbInst(unique_ptr<Reg>&& src, unique_ptr<Reg>&& reg, int offset = 0)
        :StoreInst(std::move(src), std::move(reg), offset)  { } 


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

    LoadInst(unique_ptr<Reg>&& dst, string label, unique_ptr<Reg>&& reg, int offset)
        :MemInst(std::move(dst), std::move(label), std::move(reg),  offset)  { }

    LoadInst(unique_ptr<Reg>&& dst, unique_ptr<Reg>&& reg, int offset)
        :MemInst(std::move(dst), std::move(reg), offset)  { } 


};

class LwInst : public LoadInst {

protected:
    char width() override { return 'w'; }

public:

    LwInst(unique_ptr<Reg>&& dst, string label, unique_ptr<Reg>&& reg = nullptr, int offset = 0)
        :LoadInst(std::move(dst), std::move(label), std::move(reg),  offset)  { }

    LwInst(unique_ptr<Reg>&& dst, unique_ptr<Reg>&& reg, int offset = 0)
        :LoadInst(std::move(dst), std::move(reg), offset)  { } 



};

class LbInst : public LoadInst {

protected:
    char width() override { return 'b'; }

public:

    LbInst(unique_ptr<Reg>&& dst, string label, unique_ptr<Reg>&& reg = nullptr, int offset = 0)
        :LoadInst(std::move(dst), std::move(label), std::move(reg),  offset)  { }

    LbInst(unique_ptr<Reg>&& dst, unique_ptr<Reg>&& reg, int offset = 0)
        :LoadInst(std::move(dst), std::move(reg), offset)  { } 

};


} // end namespace C0

