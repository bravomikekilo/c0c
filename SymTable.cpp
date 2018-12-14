//
// Created by baomingkun on 18-11-17.
//

#include "SymTable.h"
#include "AST/FuncAST.h"

namespace C0 {
bool SymTable::hasVarInScope(const string & name)
{
    return name_map.count(name) == 1;
}

optional<VarID> SymTable::findVarByName(const string &name) {

    if(name_map.count(name) == 1) {
        return {bulk[name_map[name]].id};
    }

    if(prev == nullptr) {
        return {};
    }

    return prev->findVarByName(name);
}

const SymTerm *SymTable::findVarByID(VarID id) const {
    if(id_map.count(id) == 1) {
        return &bulk.at(id_map.at(id));
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

vector<pair<int, string>> SymTable::getStringList() const {
    if (str_table == nullptr) return {};
    vector<pair<int, string>> ret;
    const auto &str_tab = str_table->getTable();
    auto count = str_tab.size();
    for (int i = 0; i < count; ++i) {
        ret.emplace_back(i, str_tab.at(i));
    }
    return ret;
}

vector<pair<string, Type>> SymTable::getGlobalList() const
{
    vector<pair<string, Type>> ret;
    for (const auto &term : bulk) {
        if (term.isGlobal && !term.isConst()) {
            ret.emplace_back(term.name, term.type);
        }
    }
    return ret;
}

shared_ptr<SymTable> SymTable::createGlobalTable() {
    return make_shared<SymTable>(nullptr, true);
}

shared_ptr<SymTable> SymTable::getGlobalTable() const {
    if(prev == nullptr) return nullptr;
    else if(prev->prev == nullptr) return prev;
    else return prev->getGlobalTable();
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

void FuncTable::insertFunc(shared_ptr<FuncAST> func) {
    auto name = func->name;
    auto args = func->args;
    auto ast = make_shared<FuncAST>(func->name, func->retType, args);
    table.insert(pair(name, ast));
}

}
