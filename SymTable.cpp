//
// Created by baomingkun on 18-11-17.
//

#include "SymTable.h"

namespace C0 {

optional<VarID> SymTable::findVarByName(const string &name) {

    if(name_map.count(name) == 1) {
        return {bulk[name_map[name]].id};
    }

    if(prev == nullptr) {
        return {};
    }

    return prev->findVarByName(name);
}

const SymTerm *SymTable::findVarByID(VarID id) {
    if(id_map.count(id) == 1) {
        return &bulk[id_map[id]];
    }

    if(prev == nullptr) {
        return {};
    }

    return prev->findVarByID(id);
}

void SymTable::insert(SymTerm term) {
    auto index = bulk.size();
    bulk.push_back(term);
    name_map.insert(pair(term.name, index));
    id_map.insert(pair(term.id, index));
}

}
