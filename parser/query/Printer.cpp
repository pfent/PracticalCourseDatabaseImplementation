#include "Printer.h"
#include <sstream>

using namespace std;
Printer::Printer (Operator& input, std::vector<IU*> fields) : input (input), fields (fields) {
    input.setConsumer(this);
}


Printer::~Printer() {
    // NOP
}

std::string Printer::consume (Operator&) {
    stringstream res;
    for (const auto& field : fields) {
        res << "cout << " << field->getName() << ";\n";
    }

    res << "}\n";
    return res.str();
}

std::string Printer::produce() {
    stringstream res;
    res << "#pragma once\n"
            "\n"
            "#include <unordered_map>\n"
            "#include <tuple>\n"
            "#include \"Types.h\"\n"
            "#include \"Database.h\"\n"
            "\n"
            "int main() {\n"
            "auto& db = Database::instance();\n";
    res << input.produce();
    return res.str();
}

std::vector<IU*> Printer::getProduced() {
    throw runtime_error {"nothing to produce here"};
}

std::vector<IU*> Printer::getRequired() {
    std::vector<IU*> res;
    for(auto& field : fields) {
        res.push_back(field);
    }
    return res;
}
