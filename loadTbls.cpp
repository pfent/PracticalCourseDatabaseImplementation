#include "loadTbls.h"
#include <fstream>
#include <limits>
#include <sstream>

template<typename T>
void readElement(std::istream& stream, T& field, char delim = '|') {
    std::string tmp;
    getline(stream, tmp, delim);
    field = T::castString(tmp.c_str(), tmp.length());
}

WareHouse WareHouse::read(std::string& line) {
    auto fileStream = std::stringstream(line);
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

District District::read(std::string& line) {
    auto fileStream = std::stringstream(line);
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

Customer Customer::read(std::string& line) {
    auto fileStream = std::stringstream(line);
    auto tmp = Customer {};
    readElement(fileStream, tmp.c_id);
    readElement(fileStream, tmp.c_d_id);
    readElement(fileStream, tmp.c_w_id);
    readElement(fileStream, tmp.c_first);
    readElement(fileStream, tmp.c_middle);
    readElement(fileStream, tmp.c_last);
    readElement(fileStream, tmp.c_street_1);
    readElement(fileStream, tmp.c_street_2);
    readElement(fileStream, tmp.c_city);
    readElement(fileStream, tmp.c_state);
    readElement(fileStream, tmp.c_zip);
    readElement(fileStream, tmp.c_phone);
    readElement(fileStream, tmp.c_since);
    readElement(fileStream, tmp.c_credit);
    readElement(fileStream, tmp.c_credit_lim);
    readElement(fileStream, tmp.c_discount);
    readElement(fileStream, tmp.c_balance);
    readElement(fileStream, tmp.c_ytd_paymenr);
    readElement(fileStream, tmp.c_payment_cnt);
    readElement(fileStream, tmp.c_delivery_cnt);
    readElement(fileStream, tmp.c_data, '\n'); // this is not | delimited
    return tmp;
}

History History::read(std::string& line) {
}

NewOrder NewOrder::read(std::string& line) {
}

Order Order::read(std::string& line) {
}

OrderLine OrderLine::read(std::string& line) {
}

Item Item::read(std::string& line) {
}

Stock Stock::read(std::string& line) {
}

