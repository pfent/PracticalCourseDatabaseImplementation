#include "HashJoin.h"
#include <set>

using namespace std;

HashJoin::HashJoin::HashJoin(Operator &left, Operator &right, std::vector<std::tuple<IU *, IU *>> conditions) : left(
        left), right(right), conditions(conditions) {
    left.setConsumer(this);
    right.setConsumer(this);
}

HashJoin::~HashJoin() {
    // NOP
}

string HashJoin::consume(Operator &what) {
    stringstream res;

    if (&what == &left) {
        res << "hashTable" << uuid << ".emplace(";
        res << "{";
        for_each(conditions.begin(), conditions.end() - 1, [&](auto &condition) {
            res << get<0>(condition)->getName() << ", ";
        });
        res << get<0>(conditions.back())->getName();
        res << "}, {";
        auto leftResult = getRequiredFor(left.getProduced());
        for_each(leftResult.begin(), leftResult.end() - 1, [&](auto &r) {
            res << r->getName() << ",";
        });
        res << leftResult.back()->getName();
        res << "}";
        res << ");\n";
    } else if (&what == &right) {
        res << "auto range = hashTable" << uuid << ".equal_range({";
        for_each(conditions.begin(), conditions.end() - 1, [&](auto &condition) {
            res << get<1>(condition)->getName() << ", ";
        });
        res << get<1>(conditions.back())->getName();
        res << "});\n"
                "for_each(range.first, range.second, [&]{";
        res << consumer->consume(*this);
        res << "});\n";
    }

    return res.str();
}

std::string HashJoin::produce() {
    stringstream res;
    res << "auto hashTable" << uuid << " = std::unordered_multimap<";
    res << "std::tuple<";
    for_each(conditions.begin(), conditions.end() - 1, [&](auto &condition) {
        res << get<0>(condition)->getType() << ", ";
    });
    res << get<0>(conditions.back())->getType();
    res << ">, std::tuple<";
    auto leftResult = getRequiredFor(left.getProduced());
    for_each(leftResult.begin(), leftResult.end() - 1, [&](auto &r) {
        res << r->getType() << ",";
    });
    res << leftResult.back()->getType();
    res << ">";
    res << ">{};\n";
    res << left.produce();
    res << right.produce();
    return res.str();
}

std::vector<IU *> HashJoin::getProduced() {
    set<IU *> res;
    auto lefts = left.getProduced();
    auto rights = right.getProduced();
    res.insert(lefts.begin(), lefts.end());
    res.insert(rights.begin(), rights.end());
    return vector<IU *>(res.begin(), res.end());
}

std::vector<IU *> HashJoin::getRequired() {
    set<IU *> res;
    auto transitives = consumer->getRequired();
    res.insert(transitives.begin(), transitives.end());
    for (auto &condition : conditions) {
        res.insert(std::get<0>(condition));
        res.insert(std::get<1>(condition));
    }
    return vector<IU *>(res.begin(), res.end());
}

std::vector<IU *> HashJoin::getRequiredFor(std::vector<IU *> produced) {
    std::vector<IU *> v1 = produced;
    std::vector<IU *> v2 = consumer->getRequired();
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    std::vector<IU *> v_intersection;
    std::set_intersection(v1.begin(), v1.end(),
                          v2.begin(), v2.end(),
                          std::back_inserter(v_intersection));
    return v_intersection;
}
