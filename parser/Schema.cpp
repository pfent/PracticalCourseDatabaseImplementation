#include "Schema.hpp"

#include <sstream>

static std::string type(const Schema::Relation::Attribute& attr) {
    Types::Tag type = attr.type;
    switch (type) {
    case Types::Tag::Integer:
        return "Integer";
    case Types::Tag::Timestamp:
        return "Timestamp";
    case Types::Tag::Numeric: {
       std::stringstream ss;
       ss << "Numeric(" << attr.len1 << ", " << attr.len2 << ")";
       return ss.str();
    }
    case Types::Tag::Char: {
        std::stringstream ss;
        ss << "Char(" << attr.len1 << ")";
        return ss.str();
    }
    }
    throw;
}

std::string Schema::toString() const {
    std::stringstream out;
    for (const Schema::Relation& rel : relations) {
        out << rel.name << std::endl;
        out << "\tPrimary Key:";
        for (unsigned keyId : rel.primaryKey)
            out << ' ' << rel.attributes[keyId].name;
        out << "\n\tIndices:\n";
        for (auto& index : rel.indices) {
            out << "\t\t" << index.name << " on";
            for(auto& fieldId : index.keys) {
                out << ' ' << rel.attributes[fieldId].name;
            }
            out << '\n';
        }
        out << std::endl;
        for (const auto& attr : rel.attributes)
            out << '\t' << attr.name << '\t' << type(attr) << (attr.notNull ? " not null" : "") << std::endl;
    }
    return out.str();
}
