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
    size_t uuid = getUuid();
private:
    static size_t getUuid() {
        static size_t opUuid = 0;
        return opUuid++;
    }
};

#endif // OPERATOR_H
