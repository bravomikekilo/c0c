//
// Created by baomingkun on 18-11-22.
//

#ifndef C0_CONTROLNODE_H
#define C0_CONTROLNODE_H

#include "ControlBase.h"
#include "ValueBase.h"
#include "common.h"

namespace C0 {

class Start : public ControlNode {

};

class End : public ControlNode {

};

class Region: public ControlNode {
public:
    vector<ControlE> pred;

};

class If: public ControlNode {
    ValueE predict;
    ControlE up;
};

class IfProject: public ControlNode {
    If *up;
    bool branch;
};





};

#endif //C0_CONTROLNODE_H
