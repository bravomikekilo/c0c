#pragma once
#include "ASTVisitor.h"
#include "Pos.h"

namespace C0 {


class ASTBase
{
protected:
    Pos pos;
public:
    Pos getPos() {return pos;}
    ASTBase(Pos pos): pos(pos) {}
    virtual void accept(ASTVisitor &visitor) = 0;
    virtual ~ASTBase() = default;
};

}

