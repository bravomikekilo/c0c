#pragma once
#include "common.h"
#include "util.h"
#include <variant>

namespace C0 {


enum class BaseTypeK {
    Int,
    Char,
    Void,
    Num,
    Error,
};

size_t baseTypeSize(BaseTypeK);

string baseTypeToString(BaseTypeK t);


class Type {
public:
    Type(BaseTypeK base, size_t length): base_type(base), length(length) {};
    explicit Type(BaseTypeK base): base_type(base), length(0) {};

    bool isArray() const {
        return length != 0;
    }

    bool isBase() const {
        return length == 0;
    }

    bool operator==(const Type &other) const {
        return other.length == length
            && other.base_type == base_type;
    }

    bool operator!=(const Type &other) const {
        return !(*this == other);
    }

    bool is(BaseTypeK base) const {
        if(length != 0) return false;
        else return base_type == base;
    }

    Type getBase() const {
        return Type(base_type);
    }

    /// get length of array, return 0 if is scalar type
    size_t getLength() const {
        return length;
    }

    bool isError() const {
        return base_type == BaseTypeK::Error;
    }

    string toString() const;

    size_t sizeOf() const {
        if(length == 0) return baseTypeSize(base_type);
        else return length * baseTypeSize(base_type);
    }

private:

    BaseTypeK base_type;
    size_t length;


};


}

