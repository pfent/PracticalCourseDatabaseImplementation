#ifndef TABLESCAN_H
#define TABLESCAN_H
#include "Operator.h"
#include "../Schema.hpp"
#include <string>
#include <vector>

struct TableScan : Operator {
    TableScan (Schema::Relation& relation);
    ~TableScan() override;
    std::string produce() override;
    std::string consume (Operator& what) override;
    std::vector<IU*> getProduced() override;
    std::vector<IU*> getRequired() override;
    IU* getIU(const std::string& what);
private:
    const Schema::Relation& relation;
    std::vector<IU> ius;
};

#endif // TABLESCAN_H
