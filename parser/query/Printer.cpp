#include "Printer.h"
#include <sstream>

using namespace std;
Printer::Printer (Operator& input, std::vector<std::string> fields) : input (input), fields (fields) {
    input.setConsumer(this);
}


Printer::~Printer() {
    // NOP
}

std::string Printer::consume (Operator&) {
    stringstream res;

    for (const auto& field : fields) {
        res << "cout << " << field << ";\n";
    }

    return res.str();
}

std::string Printer::produce() {
    return input.produce();
}

std::vector<Operator::IU*> Printer::getProduced() {
    throw runtime_error {"nothing to produce here"};
}

std::vector<Operator::IU*> Printer::getRequired() {
    std::vector<Operator::IU*> res;
    // TODO: add fields
    return res;
}
