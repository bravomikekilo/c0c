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

class StringTable {
    unordered_map<string, int> id_table;
    unordered_map<int, string> index_table;
public:

    int insertStr(const string &str);

    const unordered_map<int, string> &getTable() {
        return index_table;
    }

    string findStr(int id);


};

class SymTable {
    shared_ptr<SymTable> prev;
    vector<SymTerm> bulk;
    unordered_map<string, size_t> name_map;
    unordered_map<VarID , size_t> id_map;
    unique_ptr<StringTable> str_table;

public:

    static shared_ptr<SymTable> createGlobalTable();

    explicit SymTable(shared_ptr<SymTable> prev, bool withStrTable=false)
        :prev(std::move(prev)), str_table(nullptr) {
        if(withStrTable) {
            str_table = make_unique<StringTable>();
        }
    };

    optional<VarID> findVarByName(const string &name);

    const SymTerm *findVarByID(VarID id);

    void insert(SymTerm term);

    string findStr(int id) {
        if(str_table == nullptr) {
            if(prev == nullptr) return "";
            return prev->findStr(id);
        } else {
            return str_table->findStr(id);
        }
    }

    int addString(const string &str) {
        if(str_table == nullptr) {
            return prev->addString(str);
        } else {
            return str_table->insertStr(str);
        }
    }


};

}

#endif //C0_SYMTABLE_H
