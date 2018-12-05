//
// Created by baomingkun on 18-12-5.
//

#ifndef C0_DEFUSEBUILDER_H
#define C0_DEFUSEBUILDER_H

#include "SON.h"
#include "BackDFSWalker.h"


namespace C0 {

class DefUseBuilder : public BackDFSWalker {

private:
    vector<UseE> &container;
    int phrase = 0;
    size_t total_sum = 0;
    vector<UseE>::iterator head;
    unordered_map<UseE, size_t> pos_map;

protected:

    void visit(UseE node) override;
    void countUse(UseE node);
    void sumUse(UseE node) ;
    void setUse(UseE node) ;

public:
    vector<UseE> buildUseDef(StopN *stop);
    DefUseBuilder(vector<UseE> &container): container(container) {}


};

}


#endif //C0_DEFUSEBUILDER_H
