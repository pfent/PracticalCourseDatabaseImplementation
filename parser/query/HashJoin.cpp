#include "HashJoin.h"
#include <sstream>

using namespace std;

HashJoin::HashJoin::HashJoin (Operator& left, Operator& right, std::vector<std::tuple<std::string, std::string>> conditions) : left (left), right (right), conditions (conditions) {
    left.setConsumer (this);
    right.setConsumer (this);
}

HashJoin::~HashJoin() {
    // NOP
}

string HashJoin::consume (Operator& what) {
    stringstream res;

    if (&what == &left) {
        res << "//TODO: store t in HT;\n";
    } else if (&what == &right) {
        res << "TODO for tc in HT.lookup\n";
        res << consumer->consume (*this);
    }

    return res.str();
}

std::string HashJoin::produce() {
    stringstream res;
    res << "auto hashTable = unordered_multimap<TODO>{};\n";
    res << left.produce();
    res << right.produce();
    return res.str();
}

std::vector<Operator::IU*> HashJoin::getProduced() {
    vector<Operator::IU*> res;
    res.insert (res.end(), left.getProduced().begin(), left.getProduced().end());
    res.insert (res.end(), right.getProduced().begin(), right.getProduced().end());
    return res;
}

std::vector<Operator::IU*> HashJoin::getRequired() {
    vector<Operator::IU*> res;
    auto transitives = consumer->getRequired();
    res.insert (res.end(), transitives.begin(), transitives.end());
    // TODO add conditions
    return res;
}
