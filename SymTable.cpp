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

shared_ptr<SymTable> SymTable::createGlobalTable() {
    return make_shared<SymTable>(nullptr, true);
}

int StringTable::insertStr(const string &str) {
    if(id_table.count(str) == 1) {
        return id_table[str];
    } else {
        int index = id_table.size();
        id_table.insert(pair(str, index));
        index_table.insert(pair(index, str));
        return index;
    }
}

string StringTable::findStr(int id) {
    if(index_table.count(id) == 1) {
        return index_table[id];
    } else {
        return "";
    }
}

}
