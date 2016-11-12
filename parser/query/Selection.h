#ifndef SELECTION_H
#define SELECTION_H
#include "Operator.h"
#include <tuple>

struct Selection :  Operator {
    Selection (Operator& relation, std::vector<std::tuple<IU*, std::string>> conditions);
    ~Selection() override;
    std::string produce() override;
    std::string consume (Operator&) override;
    std::vector<IU*> getProduced() override;
    std::vector<IU*> getRequired() override;

private:
    Operator& input;
    std::vector<std::tuple<IU*, std::string>> conditions;
};

#endif // SELECTION_H
