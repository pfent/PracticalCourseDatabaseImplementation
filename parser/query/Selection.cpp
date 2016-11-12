#include "Selection.h"
#include <set>

using namespace std;

Selection::Selection(Operator &input, std::vector<std::tuple<IU *, string>> conditions) :
        input(input), conditions(conditions) {
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
    res << "if( ";

    for_each(conditions.begin(), conditions.end() - 1, [&](auto& condition) {
        res << get<0>(condition)->getName() << " == " << get<1>(condition) << " && ";
    });
    res << get<0>(conditions.back())->getName() << " == " << get<1>(conditions.back());
    res <<" ) {\n";

    res << consumer->consume(*this);
    res << "}";
    return res.str();
}

std::vector<IU *> Selection::getProduced() {
    return input.getProduced();
}

std::vector<IU *> Selection::getRequired() {
    set<IU *> res;
    auto transitive = consumer->getRequired();
    res.insert(transitive.begin(), transitive.end());
    for(auto& condition : conditions) {
        res.insert(std::get<0>(condition));
    }
    return std::vector<IU *>(res.begin(), res.end());
}

