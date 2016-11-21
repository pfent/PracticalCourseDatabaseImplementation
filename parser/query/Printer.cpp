#include "Printer.h"
#include <sstream>

using namespace std;

Printer::Printer(Operator &input, std::vector<IU *> fields) : input(input), fields(fields) {
    input.setConsumer(this);
}


Printer::~Printer() {
    // NOP
}

std::string Printer::consume(Operator &) {
    stringstream res;
    for (const auto &field : fields) {
        res << "cout << " << field->getName() << " << '\\t';\n";
    }
    res << "cout << '\\n';";
    return res.str();
}

std::string Printer::produce() {
    stringstream res;
    res <<  "#include \"Types.h\"\n"
            "#include \"Database.h\"\n"
            "#include <algorithm>\n"
            "#include <iostream>\n"
            "#include <unordered_map>\n"
            "\n"
            "using namespace std;\n"
            "extern \"C\" void dynamicQuery() {\n"
            "auto& db = Database::instance();\n";
    res << input.produce();
    res << "cout << flush;";
    res << "}\n";
    return res.str();
}

std::vector<IU *> Printer::getProduced() {
    throw runtime_error {"nothing to produce here"};
}

std::vector<IU *> Printer::getRequired() {
    std::vector<IU *> res;
    for (auto &field : fields) {
        res.push_back(field);
    }
    return res;
}
