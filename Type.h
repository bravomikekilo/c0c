#pragma once
#include "common.h"

namespace C0 {

enum class TypeK {
    Int,
    Char,
    Void,
    Pointer,
    Array,
    Error,
};

class Type
{
public:
    virtual string toString() const = 0;
    virtual bool is(TypeK kind) const = 0;
    virtual bool isConst() const = 0;
    ~Type() = default;
};

class ErrorType :public Type {
    string toString() const override {
        return "errorT";
    }

    bool is(TypeK type) const override {
        return type == TypeK::Error;
    }

    bool isConst() const override {return false;}
};

class BaseType : public Type {
public:
    BaseType(TypeK t, bool c)
        :t(t), c(c) {};
    static shared_ptr<BaseType> Int();
    static shared_ptr<BaseType> Char();
    static shared_ptr<BaseType> Void();
    

    bool is(TypeK type) const override {
        return t == type;
    }

    bool isConst() const override {
        return c;
    }

    string toString() const override {
        switch (t)
        {
        case C0::TypeK::Int:
            return "int";
        case C0::TypeK::Char:
            return "char";
        case C0::TypeK::Void:
            return "void";
        default:
            return "interal type error";
        }
    }

private:
    TypeK t;
    bool c;
};

class ArrayType :public Type {
protected:
    ArrayType(shared_ptr<BaseType> base, size_t sz): base(std::move(base)), sz(sz) {}

    bool is(TypeK type) const override {
        return type == TypeK::Pointer;
    }

    bool isConst() const override { return false; };

    string toString() const override {
        auto b = base->toString();
        return b + "[" + std::to_string(sz) + "]";
    }

public:
    shared_ptr<BaseType> base;
    size_t sz;
};





}

