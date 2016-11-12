#ifndef OPERATOR_H
#define OPERATOR_H
#include <vector>
#include <string>
#include "../Schema.hpp"
#include "InformationUnit.h"

struct Operator {
    virtual ~Operator(){};
    virtual std::string produce() = 0;
    virtual std::string consume(Operator& what) = 0;
    virtual std::vector<IU*> getProduced() = 0;
    virtual std::vector<IU*> getRequired() = 0;
    void setConsumer(Operator* consumer) {
        this->consumer = consumer;
    }
    
protected:
    Operator* consumer;
    size_t uuid = rand();
};

#endif // OPERATOR_H
