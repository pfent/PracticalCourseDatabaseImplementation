#include "loadTbls.h"
#include <fstream>
#include <limits>
#include <iostream>
#include <typeinfo>

template<typename T>
void readElement(std::ifstream& fileStream, T& field, char delim = '|') {
    std::string tmp;
    getline(fileStream, tmp, delim);
    try {
        field = T::castString(tmp.c_str(), tmp.length());
    } catch (const char* exception) {
        std::cout << exception << std::endl;
        std::cout << tmp << " is not a valid " << typeid(T).name() << std::endl;
    }
}

WareHouse WareHouse::read(std::ifstream& fileStream) {
    auto tmp = WareHouse {};
    readElement(fileStream, tmp.w_id);
    readElement(fileStream, tmp.w_name);
    readElement(fileStream, tmp.w_street_1);
    readElement(fileStream, tmp.w_street_2);
    readElement(fileStream, tmp.w_city);
    readElement(fileStream, tmp.w_state);
    readElement(fileStream, tmp.w_zip);
    readElement(fileStream, tmp.w_tax);
    readElement(fileStream, tmp.w_ytd, '\n'); // this is not | delimited

    return tmp;
}

District District::read(std::ifstream& fileStream) {
    auto tmp = District {};
    readElement(fileStream, tmp.d_id);
    readElement(fileStream, tmp.d_w_id);
    readElement(fileStream, tmp.d_name);
    readElement(fileStream, tmp.d_street_1);
    readElement(fileStream, tmp.d_street_2);
    readElement(fileStream, tmp.d_city);
    readElement(fileStream, tmp.d_state);
    readElement(fileStream, tmp.d_zip);
    readElement(fileStream, tmp.d_tax);
    readElement(fileStream, tmp.d_ytd);
    readElement(fileStream, tmp.d_next_o_id, '\n'); // this is not | delimited
    return tmp;
}

/*
template<>
Customer readLineFromFile<Customer>(std::ifstream& ) {
    return Customer{};
}

template<>
History readLineFromFile<History>(std::ifstream& fileStream);

template<>
NewOrder readLineFromFile<NewOrder>(std::ifstream& fileStream);

template<>
Order readLineFromFile<Order>(std::ifstream& fileStream);

template<>
OrderLine readLineFromFile<OrderLine>(std::ifstream& fileStream);

template<>
Item readLineFromFile<Item>(std::ifstream& fileStream);

template<>
Stock readLineFromFile<Stock>(std::ifstream& fileStream);
*/
