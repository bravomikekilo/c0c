//
// Created by baomingkun on 18-12-26.
//

#ifndef C0_LSECT_H
#define C0_LSECT_H

#include "common.h"
#include <list>

namespace C0 {

class LSect {
private:

    std::list<pair<size_t, size_t>> sections;

public:

    /// add a section to interval
    void add(size_t a, size_t b);


    /// merge, merge two interval
    LSect merge(const LSect &other);


    /// has intersect with other live interval
    bool intersect(const LSect &other);







};

}


#endif //C0_LSECT_H
