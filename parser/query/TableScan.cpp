#include "TableScan.h"
#include <sstream>

using namespace std;

TableScan::TableScan (Schema::Relation& relation) : relation (relation) {
    for (auto& a : relation.attributes) {
        ius.push_back (&a);
    }
}

TableScan::~TableScan() {
    // NOP
}


string TableScan::produce() {
    stringstream res;
    res << "for (size_t i = 0; i < db." << relation.name << ".size; ++i) {\n";

    for (const auto& required : getRequired()) {
        res << "const auto " << relation.name << "_" << required->name << " = db." << required->name << ";\n";
    }

    res << consumer->consume (*this);

    res << "}\n";
    return res.str();
}

string TableScan::consume (Operator&) {
    throw runtime_error {"Tablescan does not consumre anything"};
}

std::vector<Operator::IU*> TableScan::getProduced() {
    return ius;
}

std::vector<Operator::IU*> TableScan::getRequired() {
    return consumer->getRequired();
}
