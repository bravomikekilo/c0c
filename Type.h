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




}

