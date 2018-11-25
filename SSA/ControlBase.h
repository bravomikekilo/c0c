#pragma once

#include "NodeBase.h"

namespace C0 {
    
enum class COp {
    Start,
    End,
    Region,
    If,
    IfProject,
};

class ControlNode : public NodeBase {
    COp op;
};

using ControlE = ControlNode *;







} // end namespace C0


