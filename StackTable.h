//
// Created by baomingkun on 18-11-9.
//

#ifndef C0_SYMTABLE_H
#define C0_SYMTABLE_H
#include "Type.h"
#include "common.h"

namespace C0 {

template <typename K, typename V>
class StackTable {
public:
    vector<V> vars;
    unordered_map<K, size_t> index_map;

    shared_ptr<StackTable<K, V>> prev;

    bool hasPrev() const {
        return prev == nullptr;
    }

    bool has(const K &name) const {
        if(index_map.count(name) == 1) {
            return true;
        }

        auto ptr = prev;
        while(ptr != nullptr) {
            if(ptr->index_map.count(name) == 1){
                return true;
            }
            ptr = ptr->prev;
        }
        return false;
    }

    optional<V> get(const K &name) const {
        if(index_map.count(name) == 1) {
            return {index_map[name]};
        }

        auto ptr = prev;
        while(ptr != nullptr) {
            if(ptr->index_map.count(name) == 1) {
                return {index_map[name]};
            }
            ptr = ptr->prev;
        }

        return {};
    }

    void insert(const K &key, const V val) const {
        vars.push_back(val);
        index_map[key] = vars.size() - 1;
    }

};

using SymTable = StackTable<string, Type>;

}

#endif //C0_SYMTABLE_H
