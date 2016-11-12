#include "TableScan.h"
#include <sstream>
#include <iostream>

using namespace std;

TableScan::TableScan(Schema::Relation &relation) : relation(relation) {
    for (auto &a : relation.attributes) {
        ius.push_back({relation, a});
    }
}

TableScan::~TableScan() {
    // NOP
}


string TableScan::produce() {
    stringstream res;
    res << "for (size_t i = 0; i < db." << Schema::toCppName(relation.name) << ".size; ++i) {\n";

    for (const auto &required : getRequired()) {
        res << "const auto " << required->getName() << " = db." << Schema::toCppName(required->relation.name) << "." << Schema::toCppName(required->attribute.name)
            << "[i];\n";
    }

    res << consumer->consume(*this);

    res << "}\n";
    return res.str();
}

string TableScan::consume(Operator &) {
    throw runtime_error {"Tablescan does not consume anything"};
}

std::vector<IU *> TableScan::getProduced() {
    std::vector<IU *> res;
    for (auto &iu : ius) {
        res.push_back(&iu);
    }
    return res;
}

std::vector<IU *> TableScan::getRequired() {
    std::vector<IU *> v1 = getProduced();
    std::vector<IU *> v2 = consumer->getRequired();
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::vector<IU *> v_intersection;

    std::set_intersection(v1.begin(), v1.end(),
                          v2.begin(), v2.end(),
                          std::back_inserter(v_intersection));
    return v_intersection;
}

IU *TableScan::getIU(const std::string &what) {
    for (auto &iu : ius) {
        if (iu.attribute.name == what) {
            return &iu;
        }
    }
    throw runtime_error {"No such field: " + what};
}
