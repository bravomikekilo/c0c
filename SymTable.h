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

class FuncAST;

class FuncTable {
    unordered_map<string, shared_ptr<FuncAST>> table;

public:
    void insertFunc(shared_ptr<FuncAST> func);

    bool hasFunc(const string &name) {
        return table.count(name) != 0;
    }

    optional<shared_ptr<FuncAST>> findFunc(const string &name) {
        if(hasFunc(name)) {
            return {table[name]};
        } else {
            return {};
        }
    }

};

class StringTable {
    unordered_map<string, int> id_table;
    unordered_map<int, string> index_table;
public:

    int insertStr(const string &str);

    const unordered_map<int, string> &getTable() const {
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
    unique_ptr<FuncTable> func_table;

public:

    static shared_ptr<SymTable> createGlobalTable();

    explicit SymTable(shared_ptr<SymTable> prev, bool isGlobal=false)
        :prev(std::move(prev)), str_table(nullptr) {
        if(isGlobal) {
            str_table = make_unique<StringTable>();
            func_table = make_unique<FuncTable>();
        }
    };

    bool hasVarInScope(const string &name);

    optional<VarID> findVarByName(const string &name);

    const SymTerm *findVarByID(VarID id) const ;

    const vector<SymTerm> &getVarInScope() {
        return bulk;
    }

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

    optional<shared_ptr<FuncAST>> findFunc(const string &name) {
        if(func_table == nullptr) {
            return prev->findFunc(name);
        } else {
            return func_table->findFunc(name);
        }
    }

    bool hasFunc(const string &name) {
        if(func_table == nullptr) {
            return prev->hasFunc(name);
        } else {
            return func_table->hasFunc(name);
        }
    }

    void addFunc(shared_ptr<FuncAST> func) {
        if(func_table == nullptr) {
            return prev->addFunc(func);
        } else {
            return func_table->insertFunc(func);
        }

    }

    vector<pair<int, string>> getStringList() const ;
    vector<pair<string, Type>> getGlobalList() const ;
    

};

}

#endif //C0_SYMTABLE_H
