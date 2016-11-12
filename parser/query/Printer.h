#ifndef PRINTER_H
#define PRINTER_H
#include "Operator.h"

struct Printer : Operator {
    Printer (Operator& input, std::vector<IU*> fields);
    ~Printer() override;
    std::string produce() override;
    std::string consume (Operator&) override;
    std::vector<IU*> getProduced() override;
    std::vector<IU*> getRequired() override;
private:
    Operator& input;
    std::vector<IU*> fields;
};

#endif // PRINTER_H
