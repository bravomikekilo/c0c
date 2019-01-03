//
// Created by baomingkun on 18-12-17.
//

#ifndef C0_DOMINANCE_H
#define C0_DOMINANCE_H

#include "SON.h"

namespace C0 {

vector<RegionN *> computeDominanceTree(pair<RegionN *, StopN *> graph);

vector<RegionN *> getDominanceOrder(const vector<RegionN *> &relation, const vector<RegionN *> &order, int start_index);


}

#endif //C0_DOMINANCE_H
