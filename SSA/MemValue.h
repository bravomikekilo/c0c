//
// Created by baomingkun on 18-11-29.
//

#ifndef C0_MEMVALUE_H
#define C0_MEMVALUE_H

#include "ValueBase.h"

namespace C0 {

class SetV : public ValueNode {
protected:
    ValueE index;
    ValueE val;

public:

    SetV(Vop op, Region *region, ValueE index, ValueE val)
            : ValueNode(region, op), index(index), val(val) {}

};


class SetIntV : public SetV {

public:
    SetIntV(Region *region, ValueE index, ValueE val)
            : SetV(Vop::SetInt, region, index, val) {}

};


class SetCharV : public SetV {

public:
    SetCharV(Region *region, ValueE index, ValueE val)
            : SetV(Vop::SetChar, region, index, val) {}

};

class GetV : public ValueNode {
protected:
    ValueE arr;
    ValueE index;

public:

    GetV(Vop op, Region *region, ValueE arr, ValueE index)
            : ValueNode(region, op), arr(arr), index(index) {}

};

class GetIntV : public GetV {

public:

    GetIntV(Region *region, ValueE arr, ValueE index)
            : GetV(Vop::GetInt, region, arr, index) {}

};


class GetCharV : public GetV {

public:
    GetCharV(Region *region, ValueE arr, ValueE index)
            : GetV(Vop::GetChar, region, arr, index) {}

};

}
#endif //C0_MEMVALUE_H
