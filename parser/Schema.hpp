#ifndef H_Schema_hpp
#define H_Schema_hpp

#include <vector>
#include <string>
#include "Types.hpp"

struct Schema {
    struct Relation {
        struct Attribute {
            std::string name;
            Types::Tag type;
            unsigned len1 = ~0;
            unsigned len2 = ~0;
            bool notNull = true;
        };
        struct Index {
            std::string name;
            std::vector<unsigned> keys;
            Index(const std::string& name) : name(name) {}
        };
        std::string name;
        std::vector<Schema::Relation::Attribute> attributes;
        std::vector<unsigned> primaryKey;
        std::vector<Index> indices;
        Relation(const std::string& name) : name(name) {}
    };
    std::vector<Schema::Relation> relations;
    std::string toString() const;
};
#endif
