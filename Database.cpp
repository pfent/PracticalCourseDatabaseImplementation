#include "Database.h"
#include <algorithm>
#include <iterator>

using namespace std;

template <typename S, typename Func>
void buildIndex(vector<S> container, Func&&  generateKey) {
    auto size = container.size();
    S::index.reserve(size);
    for (size_t i = 0; i < size; i++) {
        S::index[generateKey(container[i])] = i;
    }
}

Database& Database::instance() {
    static Database instance;
    return instance;
}

void Database::importDatabaseFromPath(const std::string&& path) {
    importDatabaseFromPath(path);
}

void Database::importDatabaseFromPath(const std::string& path) {
    using namespace string_literals;
    wareHouses = readTableFromFile<WareHouse>(path + "tpcc_warehouse.tbl"s);
    buildIndex(wareHouses, [&](const WareHouse & x) {
        return x.w_id;
    });

    districts = readTableFromFile<District>(path + "tpcc_district.tbl"s);
    buildIndex(districts, [&](const District & x) {
        return make_tuple(x.d_w_id, x.d_id);
    });

    customers = readTableFromFile<Customer>(path + "tpcc_customer.tbl"s);
    buildIndex(customers, [&](const Customer & x) {
        return make_tuple(x.c_w_id, x.c_d_id, x.c_id);
    });

    historys = readTableFromFile<History>(path + "tpcc_history.tbl"s);
    // no primary key for histroy in schema

    newOrders = readTableFromFile<NewOrder>(path + "tpcc_neworder.tbl"s);
    buildIndex(newOrders, [&](const NewOrder & x) {
        return make_tuple(x.no_w_id, x.no_d_id, x.no_o_id);
    });

    orders = readTableFromFile<Order>(path + "tpcc_order.tbl"s);
    buildIndex(orders, [&](const Order & x) {
        return make_tuple(x.o_w_id, x.o_d_id, x.o_id);
    });

    orderLines = readTableFromFile<OrderLine>(path + "tpcc_orderline.tbl"s);
    buildIndex(orderLines, [&](const OrderLine & x) {
        return make_tuple(x.ol_w_id, x.ol_d_id, x.ol_o_id, x.ol_number);
    });

    items = readTableFromFile<Item>(path + "tpcc_item.tbl"s);
    buildIndex(items, [&](const Item & x) {
        return x.i_id;
    });

    stocks = readTableFromFile<Stock>(path + "tpcc_stock.tbl"s);
    buildIndex(stocks, [&](const Stock & x) {
        return make_tuple(x.s_w_id, x.s_i_id);
    });


    // TODO indices
}

const std::vector<WareHouse>& Database::getWareHouses() {
    return wareHouses;
}
const std::vector<District>& Database::getDistricts() {
    return districts;
}
const std::vector<Customer>& Database::getCustomers() {
    return customers;
}
const std::vector<History>& Database::getHistorys() {
    return historys;
}
const std::vector<NewOrder>& Database::getNewOrders() {
    return newOrders;
}
const std::vector<Order>& Database::getOrders() {
    return orders;
}
const std::vector<OrderLine>& Database::getOrderLines() {
    return orderLines;
}
const std::vector<Item>& Database::getItems() {
    return items;
}
const std::vector<Stock>& Database::getStocks() {
    return stocks;
}
