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
    vector<T> vars;
    unordered_map<K, size_t> index_map;

    shared_ptr<StackTable<T>> prev;

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

    optional<T> get(const K &name) const {
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

};

using SymTable = StackTable<string, Type>;

}

#endif //C0_SYMTABLE_H
