//
// Created by baomingkun on 18-12-4.
//

#ifndef C0_SEA_H
#define C0_SEA_H

#include "common.h"
#include <list>

namespace C0 {

/// arena for nodes
class Sea {
private:
    std::list<char *> sections;
    const size_t section_size;
    char *curr_section;
    size_t water_mark = 0;
public:

    explicit Sea(size_t section_size=4096): section_size(section_size) {
        curr_section = new char[section_size];
    }
    /*
    Sea(Sea &&other) noexcept
        : section_size(other.section_size), sections(std::move(other.sections)){
        curr_section = other.curr_section;
        water_mark = other.water_mark;
        other.curr_section = nullptr;
        other.water_mark = 0;
    }
    */

    template <typename T, typename...Args>
    T *alloc(Args&&... args) {
        auto sz = sizeof(T);
        if(sz > section_size) return nullptr;
        if(sz + water_mark > section_size) {
            sections.push_back(curr_section);
            curr_section = new char[section_size];
            water_mark = 0;
        }

        auto ret = new(curr_section + water_mark) T(std::forward<Args>(args)...);
        water_mark += sz;
        return ret;
    }

    ~Sea() {
        for(auto ptr: sections) {
            delete[] ptr;
        }

        delete curr_section;
    }
};

}

#endif //C0_SEA_H
