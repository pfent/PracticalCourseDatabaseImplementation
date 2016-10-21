#include <iostream>
#include <string>
#include "neworderrandom.h"
#include "Database.h"

using namespace std;

int main() {
    using namespace string_literals;
    auto & database = Database::instance();
    database.importDatabaseFromPath("../tbls/"s);

    cout << "Read WareHouses lines: " << database.getWareHouses().size() << '\n';
    cout << "Read Districts lines: " << database.getDistricts().size() << '\n';
    cout << "Read Customer lines: " << database.getCustomers().size() << '\n';
    cout << "Read Historys lines: " << database.getHistorys().size() << '\n';
    cout << "Read NewOrders lines: " << database.getNewOrders().size() << '\n';
    cout << "Read Orders lines: " << database.getOrders().size() << '\n';
    cout << "Read OrderLines lines: " << database.getOrderLines().size() << '\n';
    cout << "Read Items lines: " << database.getItems().size() << '\n';
    cout << "Read Stocks lines: " << database.getStocks().size() << '\n';

    newOrderRandom();
    return 0;
}
