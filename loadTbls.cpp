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
    readElement(fileStream, tmp.w_ytd, '\n'); // last element is not | delimited
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
    readElement(fileStream, tmp.d_next_o_id, '\n'); // last element is not | delimited
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
    readElement(fileStream, tmp.c_data, '\n'); // last element is not | delimited
    return tmp;
}

History History::read(std::string& line) {
    auto fileStream = std::stringstream(line);
    auto tmp = History {};
    readElement(fileStream, tmp.h_c_id);
    readElement(fileStream, tmp.h_c_d_id);
    readElement(fileStream, tmp.h_c_w_id);
    readElement(fileStream, tmp.h_d_id);
    readElement(fileStream, tmp.h_w_id);
    readElement(fileStream, tmp.h_date);
    readElement(fileStream, tmp.h_amount);
    readElement(fileStream, tmp.h_data, '\n'); // last element is not | delimited
    return tmp;
}

NewOrder NewOrder::read(std::string& line) {
    auto fileStream = std::stringstream(line);
    auto tmp = NewOrder {};
    readElement(fileStream, tmp.no_o_id);
    readElement(fileStream, tmp.no_d_id);
    readElement(fileStream, tmp.no_w_id, '\n'); // last element is not | delimited
    return tmp;
}

Order Order::read(std::string& line) {
    auto fileStream = std::stringstream(line);
    auto tmp = Order {};
    readElement(fileStream, tmp.o_id);
    readElement(fileStream, tmp.o_d_id);
    readElement(fileStream, tmp.o_w_id);
    readElement(fileStream, tmp.o_c_id);
    readElement(fileStream, tmp.o_entry_d);
    readElement(fileStream, tmp.o_carrier_id);
    readElement(fileStream, tmp.o_ol_cnt);
    readElement(fileStream, tmp.o_all_local, '\n'); // last element is not | delimited
    return tmp;
}

OrderLine OrderLine::read(std::string& line) {
    auto fileStream = std::stringstream(line);
    auto tmp = OrderLine {};
    readElement(fileStream, tmp.ol_o_id);
    readElement(fileStream, tmp.ol_d_id);
    readElement(fileStream, tmp.ol_w_id);
    readElement(fileStream, tmp.ol_number);
    readElement(fileStream, tmp.ol_i_id);
    readElement(fileStream, tmp.ol_supply_w_id);
    readElement(fileStream, tmp.ol_delivery_d);
    readElement(fileStream, tmp.ol_quantity);
    readElement(fileStream, tmp.ol_amount);
    readElement(fileStream, tmp.ol_dist_info, '\n'); // last element is not | delimited
    return tmp;
}

Item Item::read(std::string& line) {
    auto fileStream = std::stringstream(line);
    auto tmp = Item {};
    readElement(fileStream, tmp.i_id);
    readElement(fileStream, tmp.i_im_id);
    readElement(fileStream, tmp.i_name);
    readElement(fileStream, tmp.i_price);
    readElement(fileStream, tmp.i_data, '\n'); // last element is not | delimited
    return tmp;
}

Stock Stock::read(std::string& line) {
    auto fileStream = std::stringstream(line);
    auto tmp = Stock {};
    readElement(fileStream, tmp.s_i_id);
    readElement(fileStream, tmp.s_w_id);
    readElement(fileStream, tmp.s_quantity);
    readElement(fileStream, tmp.s_dist_01);
    readElement(fileStream, tmp.s_dist_02);
    readElement(fileStream, tmp.s_dist_03);
    readElement(fileStream, tmp.s_dist_04);
    readElement(fileStream, tmp.s_dist_05);
    readElement(fileStream, tmp.s_dist_06);
    readElement(fileStream, tmp.s_dist_07);
    readElement(fileStream, tmp.s_dist_08);
    readElement(fileStream, tmp.s_dist_09);
    readElement(fileStream, tmp.s_dist_10);
    readElement(fileStream, tmp.s_ytd);
    readElement(fileStream, tmp.s_order_cnt);
    readElement(fileStream, tmp.s_remote_cnt);
    readElement(fileStream, tmp.s_data, '\n'); // last element is not | delimited
    return tmp;
}

