//
// Created by baomingkun on 18-12-11.
//


#include "ControlN.h"

using std::list;

void C0::RegionN::Linearization() {
    list<UseE> phi;
    for(auto user: getUser()) {
        auto user_op = user->getOp();
        if(user_op != Nop::Region && user_op != Nop::IfProj) {
            
        }
    }
}
