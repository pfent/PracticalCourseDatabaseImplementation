#include "Selection.h"
#include <sstream>

using namespace std;

Selection::Selection(Operator &input, std::vector<std::tuple<std::string, std::string>> conditions) : input(input),
                                                                                                      conditions(
                                                                                                              conditions) {
    input.setConsumer(this);
}

Selection::~Selection() {
    // NOP
}


std::string Selection::produce() {
    return input.produce();
}


std::string Selection::consume(Operator &) {
    stringstream res;
    res << "if( TODO: CONDITIONS) {\n";
    res << consumer->consume(*this);
    res << "}";
    return res.str();
}

std::vector<Operator::IU *> Selection::getProduced() {
    return input.getProduced();
}

std::vector<Operator::IU *> Selection::getRequired() {
    vector<Operator::IU *> res;
    auto transitive = consumer->getRequired();
    res.insert(res.end(), transitive.begin(), transitive.end());
    // TODO add conditions
    return res;
}

