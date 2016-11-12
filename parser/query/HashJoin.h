#ifndef HASHJOIN_H
#define HASHJOIN_H
#include "Operator.h"
#include <tuple>

struct HashJoin :  Operator {
    HashJoin (Operator& left, Operator& right, std::vector<std::tuple<IU*, IU*>> conditions);
    ~HashJoin() override;
    std::string produce() override;
    std::string consume (Operator& what) override;
    std::vector<IU*> getProduced() override;
    std::vector<IU*> getRequired() override;
    
private:
    Operator& left;
    Operator& right;
    std::vector<std::tuple<IU*, IU*>> conditions;
    std::vector<IU*> getRequiredFor(std::vector<IU*> produced);
};

#endif // HASHJOIN_H
