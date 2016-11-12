#ifndef H_Schema_hpp
#define H_Schema_hpp

#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "Types.hpp"
#include <sstream>

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

            Index(const std::string &name) : name(name) {}
        };

        std::string name;
        std::vector<Schema::Relation::Attribute> attributes;
        std::vector<unsigned> primaryKey;
        std::vector<Index> indices;

        Relation(const std::string &name) : name(name) {}
    };

    std::vector<Schema::Relation> relations;

    Schema::Relation &find(const std::string &name) {
        for (auto &relation : relations) {
            if (relation.name == name)
                return relation;
        }
        throw std::runtime_error("no such relation " + name);
    }

    std::string toString() const;

    std::string toCpp() const;

    static std::string cppType(const Schema::Relation::Attribute &attr) {
        Types::Tag type = attr.type;
        switch (type) {
            case Types::Tag::Integer:
                return "Integer";
            case Types::Tag::Timestamp:
                return "Timestamp";
            case Types::Tag::Numeric: {
                std::stringstream ss;
                ss << "Numeric<" << attr.len1 << ", " << attr.len2 << ">";
                return ss.str();
            }
            case Types::Tag::Char: {
                std::stringstream ss;
                ss << "Char<" << attr.len1 << ">";
                return ss.str();
            }
        }
        throw;
    }

    static std::string toCppName(const std::string &name) {
        std::string res;
        if (name[0] == '\"' && name[name.size() - 1] == '\"') {
            res = name.substr(1, name.size() - 2);
        } else {
            res = name;
        }
        return res;
    }
};

#endif
