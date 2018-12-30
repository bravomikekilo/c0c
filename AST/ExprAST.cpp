//
// Created by baomingkun on 18-11-20.
//

#include "ExprAST.h"
#include "FuncAST.h"

namespace C0 {


C0::Type C0::CallExpr::outType(std::shared_ptr<C0::SymTable> table) {
    auto func = table->findFunc(name);
    if (!func.has_value()) {
        return Type(BaseTypeK::Error);
    }

    return func.value()->retType;
}


Type PareExpr::outType(shared_ptr<SymTable> table) {
    auto sub_type = exp->outType(table);
    if(sub_type.isArray()) {
        return sub_type;
    } else {
        if(sub_type.is(BaseTypeK::Int) || sub_type.is(BaseTypeK::Char)) {
            return Type(BaseTypeK::Num);
        } else {
            return sub_type;
        }
    }
}


}
