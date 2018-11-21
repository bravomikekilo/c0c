//
// Created by baomingkun on 18-11-20.
//

#include "ExprAST.h"
#include "FuncAST.h"

C0::Type C0::CallExpr::outType(std::shared_ptr<C0::SymTable> table) {
    auto func = table->findFunc(name);
    if(!func.has_value()) {
        return Type(BaseTypeK::Error);
    }

    return func.value()->retType;
}
