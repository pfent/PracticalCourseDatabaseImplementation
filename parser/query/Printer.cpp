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

    return res.str();
}

std::string Printer::produce() {
    return input.produce();
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
