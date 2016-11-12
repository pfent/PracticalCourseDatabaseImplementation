#ifndef OPERATOR_H
#define OPERATOR_H
#include <vector>
#include <string>
#include "../Schema.hpp"

struct Operator {
    using IU = Schema::Relation::Attribute;
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
};

#endif // OPERATOR_H
