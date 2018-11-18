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

