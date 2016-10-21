#include "Database.h"
#include <algorithm>
#include <iterator>

using namespace std;

template <typename S>
void buildIndex(Table<S> table) {
    auto size = table.size();
    S::index.reserve(size);
    for (size_t i = 0; i < size; i++) {
        S::index[table[i].getKey()] = i;
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
    wareHouses.table = readTableFromFile<WareHouse>(path + "tpcc_warehouse.tbl"s);
    buildIndex(wareHouses);

    districts.table = readTableFromFile<District>(path + "tpcc_district.tbl"s);
    buildIndex(districts);

    customers.table = readTableFromFile<Customer>(path + "tpcc_customer.tbl"s);
    buildIndex(customers);

    historys.table = readTableFromFile<History>(path + "tpcc_history.tbl"s);
    // no primary key for histroy in schema

    newOrders.table = readTableFromFile<NewOrder>(path + "tpcc_neworder.tbl"s);
    buildIndex(newOrders);

    orders.table = readTableFromFile<Order>(path + "tpcc_order.tbl"s);
    buildIndex(orders);

    orderLines.table = readTableFromFile<OrderLine>(path + "tpcc_orderline.tbl"s);
    buildIndex(orderLines);

    items.table = readTableFromFile<Item>(path + "tpcc_item.tbl"s);
    buildIndex(items);

    stocks.table = readTableFromFile<Stock>(path + "tpcc_stock.tbl"s);
    buildIndex(stocks);
}
