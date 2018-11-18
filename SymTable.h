//
// Created by baomingkun on 18-11-17.
//

#ifndef C0_SYMTABLE_H
#define C0_SYMTABLE_H

#include "common.h"
#include "Type.h"

namespace C0 {

using VarID = int;

struct SymTerm {
    VarID id;
    Type type;
    string name;
    bool isGlobal;
    optional<int> val;
    bool isConst() const {
        return val.has_value();
    }
};

class SymTable {
    shared_ptr<SymTable> prev;
    vector<SymTerm> bulk;
    unordered_map<string, size_t> name_map;
    unordered_map<VarID , size_t> id_map;

public:

    explicit SymTable(shared_ptr<SymTable> prev)
        :prev(std::move(prev)) {};

    optional<VarID> findVarByName(const string &name);

    const SymTerm *findVarByID(VarID id);

    void insert(SymTerm term);


};

}

#endif //C0_SYMTABLE_H
