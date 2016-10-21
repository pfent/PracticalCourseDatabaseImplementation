#include "Database.h"
#include <algorithm>
#include <iterator>

using namespace std;

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
    wareHouses.buildIndex();

    districts.table = readTableFromFile<District>(path + "tpcc_district.tbl"s);
    districts.buildIndex();

    customers.table = readTableFromFile<Customer>(path + "tpcc_customer.tbl"s);
    customers.buildIndex();

    historys.table = readTableFromFile<History>(path + "tpcc_history.tbl"s);
    // no primary key for histroy in schema

    newOrders.table = readTableFromFile<NewOrder>(path + "tpcc_neworder.tbl"s);
    newOrders.buildIndex();

    orders.table = readTableFromFile<Order>(path + "tpcc_order.tbl"s);
    orders.buildIndex();

    orderLines.table = readTableFromFile<OrderLine>(path + "tpcc_orderline.tbl"s);
    orderLines.buildIndex();

    items.table = readTableFromFile<Item>(path + "tpcc_item.tbl"s);
    items.buildIndex();

    stocks.table = readTableFromFile<Stock>(path + "tpcc_stock.tbl"s);
    stocks.buildIndex();
}
