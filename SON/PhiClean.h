//
// Created by baomingkun on 18-12-5.
//

#ifndef C0_PHICLEAN_H
#define C0_PHICLEAN_H

#include "SON.h"
#include "Sea.h"
#include "BackDFSWalker.h"
#include <stack>

namespace C0 {

class PhiCleaner : public BackDFSWalker {
private:

    Sea &sea;

    std::stack<pair<PhiN *, UseE>> worklist;

    optional<UseE> trivial(PhiN *);

    UseE undef;

protected:
    void visit(UseE node) override;

public:

    PhiCleaner(Sea &sea): sea(sea) {
        undef = sea.alloc<UndefN>();
        undef->initDefUse();
    }

    void optimize(StopN *stop);

};


}


#endif //C0_PHICLEAN_H
