#pragma once

#ifndef TYPE_H_
#define TYPE_H_

#include <unordered_map>
#include <string>


namespace {
    using umap = std::unordered_map<std::string, std::string>;

    class type_map_t : public umap
    {
    public:
    
        using key_type = umap::key_type;
        using mapped_type = umap::mapped_type;
        type_map_t(std::initializer_list<value_type> init) : umap(init) {}

        mapped_type at(const key_type& _k) const noexcept
        try {
            return umap::at(_k);
        } catch (const std::out_of_range&) {
            return _k;
        }

        private:
            mutable std::size_t pos_opening{ 0 };
    };
}


const type_map_t type_map = {
    {"INTEGER", "int"},
    {"STRING", "_1_STRING"},
    {"REAL", "double"},
    {"CHAR", "char"},
    {"BOOLEAN", "bool"}
};


#endif // !TYPE_H_