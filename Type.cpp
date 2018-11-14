#include "Type.h"

namespace C0 {



shared_ptr<BaseType> BaseType::Int() {
    auto *ret = new BaseType(TypeK::Int, false);
    return shared_ptr<BaseType>(ret);
}

shared_ptr<BaseType> BaseType::Char() {
    auto *ret = new BaseType(TypeK::Char, false);
    return shared_ptr<BaseType>(ret);
}

shared_ptr<BaseType> BaseType::Void() {
    auto *ret = new BaseType(TypeK::Void, false);
    return shared_ptr<BaseType>(ret);
}


}
