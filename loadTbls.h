#ifndef LOADTBLS_H
#define LOADTBLS_H
#include "Types.h"
#include <string>
#include <vector>
#include <fstream>
#include <limits>

template<typename T>
T readLineFromFile(std::ifstream& fileName);

template<typename T>
std::vector<T> readTableFromFile(std::string fileName);

struct WareHouse {
    Integer w_id;// primary key
    Char<10> w_name;
    Char<20> w_street_1;
    Char<20> w_street_2;
    Char<20> w_city;
    Char<2> w_state;
    Char<9> w_zip;
    Numeric<4, 4> w_tax;
    Numeric<12, 2> w_ytd;

    static WareHouse read(std::string& line);
};

struct District {
    Integer d_id; // primary key
    Integer d_w_id; // primary key
    Char<10> d_name;
    Char<20> d_street_1;
    Char<20> d_street_2;
    Char<20> d_city;
    Char<2> d_state;
    Char<9> d_zip;
    Numeric<4, 4> d_tax;
    Numeric<12, 2> d_ytd;
    Integer d_next_o_id;

    static District read(std::string& line);
};

struct Customer {
    Integer c_id; // primary key
    Integer c_d_id; // primary key
    Integer c_w_id; // primary key
    Char<16> c_first;
    Char<2> c_middle;
    Char<16> c_last;
    Char<20> c_street_1;
    Char<20> c_street_2;
    Char<20> c_city;
    Char<2> c_state;
    Char<9> c_zip;
    Char<16> c_phone;
    Timestamp c_since;
    Char<2> c_credit;
    Numeric<12, 2> c_credit_lim;
    Numeric<4, 4> c_discount;
    Numeric<12, 2> c_balance;
    Numeric<12, 2> c_ytd_paymenr;
    Numeric<4, 0> c_payment_cnt;
    Numeric<4, 0> c_delivery_cnt;
    Char<500> c_data;

    static Customer read(std::string& line);

};

struct History {
    Integer h_c_id;
    Integer h_c_d_id;
    Integer h_c_w_id;
    Integer h_d_id;
    Integer h_w_id;
    Timestamp h_date;
    Numeric<6, 2> h_amount;
    Char<24> h_data;

    static History read(std::string& line);
};

struct NewOrder {
    Integer no_o_id; // primary key
    Integer no_d_id; // primary key
    Integer no_w_id; // primary key

    static NewOrder read(std::string& line);
};

struct Order {
    Integer o_id; // primary key
    Integer o_d_id; // primary key
    Integer o_w_id; // primary key
    Integer o_c_id;
    Timestamp o_entry_d;
    Integer o_carrier_id;
    Numeric<2, 0> o_ol_cnt;
    Numeric<1, 0> o_all_local;

    static Order read(std::string& line);
};

struct OrderLine {
    Integer ol_o_id; // primary key
    Integer ol_d_id; // primary key
    Integer ol_w_id; // primary key
    Integer ol_number; // primary key
    Integer ol_i_id;
    Integer ol_supply_w_id;
    Timestamp ol_delivery_d;
    Numeric<2, 0> ol_quantity;
    Numeric<6, 2> ol_amount;
    Char<24> ol_dist_info;

    static OrderLine read(std::string& line);
};

struct Item {
    Integer i_id; // primary key
    Integer i_im_id;
    Char<24> i_name;
    Numeric<5, 2> i_price;
    Char<50> i_data;

    static Item read(std::string& line);
};

struct Stock {
    Integer s_i_id; // primary key
    Integer s_w_id; // primary key
    Numeric<4, 0> s_quantity;
    Char<24> s_dist_01;
    Char<24> s_dist_02;
    Char<24> s_dist_03;
    Char<24> s_dist_04;
    Char<24> s_dist_05;
    Char<24> s_dist_06;
    Char<24> s_dist_07;
    Char<24> s_dist_08;
    Char<24> s_dist_09;
    Char<24> s_dist_10;
    Numeric<8, 0> s_ytd;
    Numeric<4, 0> s_order_cnt;
    Numeric<4, 0> s_remote_cnt;
    Char<50> s_data;

    static Stock read(std::ifstream& line);
};

template<typename T>
std::vector<T> readTableFromFile(std::string name) {
    auto table = std::vector<T> {};
    auto fileStream = std::ifstream {};
    fileStream.open(name);
    if (!fileStream.is_open()) {
        return table;
    }
    for (std::string line; std::getline(fileStream, line, '\n');) {
        table.push_back(T::read(line));
    }
    return table;
}


#endif // LOADTBLS_H
