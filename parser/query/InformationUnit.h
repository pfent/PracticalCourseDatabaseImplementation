#ifndef PRACTICALCOURSEDATABASEIMPLEMENTATION_INFORMATIONUNIT_H
#define PRACTICALCOURSEDATABASEIMPLEMENTATION_INFORMATIONUNIT_H

#include "../Schema.hpp"
#include <string>

struct IU {
    Schema::Relation& relation;
    Schema::Relation::Attribute& attribute;

    std::string getName() {
        return Schema::toCppName(relation.name) + "_" + Schema::toCppName(attribute.name);
    }

    std::string getType()  {
        return Schema::cppType(attribute);
    }
};

#endif //PRACTICALCOURSEDATABASEIMPLEMENTATION_INFORMATIONUNIT_H
