#pragma once
#include "ASTVisitor.h"

namespace C0 {


class ASTBase
{
public:
    virtual void accept(ASTVisitor &visitor) = 0;

    virtual ~ASTBase() = default;
};

}

