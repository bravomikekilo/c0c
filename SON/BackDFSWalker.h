//
// Created by baomingkun on 18-12-5.
//

#ifndef C0_BACKBFSWALKER_H
#define C0_BACKBFSWALKER_H

#include "SON.h"

namespace C0 {

class BackDFSWalker {

protected:

    unordered_set<UseE> visited_set;

    void walk(UseE stop);

    virtual void visit(UseE node) = 0;


};

}


#endif //C0_BACKBFSWALKER_H
