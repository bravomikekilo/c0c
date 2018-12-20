#pragma once
#include "common.h"
#include "util.h"
#include <variant>

namespace C0 {


enum class BaseTypeK {
    Int,
    Char,
    Void,
    Error,
};

size_t baseTypeSize(BaseTypeK);

string baseTypeToString(BaseTypeK t);

/*
struct ArrayT {
    BaseTypeK base;
    size_t length;
    inline size_t sizeOf() const {
        return length * baseTypeSize(base);
    }

    bool operator==(const ArrayT other) const {
        return base == other.base && length == other.length;
    }

    bool operator!=(const ArrayT other) const {
        return base != other.base || length != other.length;
    }

    string toString() const ;

};
*/

/*
class Type {
public:
    using ValType = std::variant<ArrayT, BaseTypeK>;


    Type(BaseTypeK base, size_t length)
        :val(ArrayT{base, length}){}



    explicit Type(BaseTypeK base)
        :val(base){}




    bool isArray() const {
        return std::holds_alternative<ArrayT>(val);
    }

    bool isBase() const {
        return std::holds_alternative<BaseTypeK>(val);
    }

    bool operator==(const Type &other) {
       return (val == other.val);
    }

    bool operator!=(const Type &other) {
        return (val != other.val);
    }

    bool is(BaseTypeK base) const {
        if(std::holds_alternative<BaseTypeK>(val)) {
            return std::get<BaseTypeK>(val) == base;
        } else {
            return false;
        }
    }

    Type getBase() const {
        if(isBase()) {
            return Type(std::get<BaseTypeK>(val));
        } else {
            return Type(std::get<ArrayT>(val).base);
        }
    }

    bool isError() const {
        return std::holds_alternative<BaseTypeK>(val) 
            && std::get<BaseTypeK>(val) == BaseTypeK::Error;
    }


    string toString() const {
        if(isArray()) {
            return std::get<ArrayT>(val).toString();
        } else {
            return baseTypeToString(std::get<BaseTypeK>(val));
        }
    }


    size_t sizeOf() const {
        return std::visit(overloaded {
            [](BaseTypeK arg) -> size_t { return baseTypeSize(arg); },
            [](ArrayT arg) -> size_t {return arg.sizeOf(); },
            }, val);
    }

    optional<string> canAssignTo(const Type& other) const {
        if (isArray() || other.isArray()) return {"array value can't be assigned"};
        if (isError()) return {};
        const auto& base = std::get<BaseTypeK>(val);
        const auto& other_base = std::get<BaseTypeK>(other.getVal());
        if (base != other_base) {
            return "can't assign different type";
        } else {
            return {};
        }

    }

    const ValType &getVal() const {
        return val;
    }


private:
    ValType val;

    
};
*/

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

