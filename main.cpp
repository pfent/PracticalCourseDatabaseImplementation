#include <iostream>
#include <string>
#include "neworderrandom.h"
#include "Database.h"

using namespace std;

int main() {
    using namespace string_literals;
    auto & database = Database::instance();
    database.importDatabaseFromPath("../tbls/"s);

    cout << "Read WareHouses lines: " << database.wareHouses.getView().size() << '\n';
    cout << "Read Districts lines: " << database.districts.getView().size() << '\n';
    cout << "Read Customer lines: " << database.customers.getView().size() << '\n';
    cout << "Read Historys lines: " << database.historys.getView().size() << '\n';
    cout << "Read NewOrders lines: " << database.newOrders.getView().size() << '\n';
    cout << "Read Orders lines: " << database.orders.getView().size() << '\n';
    cout << "Read OrderLines lines: " << database.orderLines.getView().size() << '\n';
    cout << "Read Items lines: " << database.items.getView().size() << '\n';
    cout << "Read Stocks lines: " << database.stocks.getView().size() << std::endl;

    newOrderRandom();
    return 0;
}
