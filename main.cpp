#include <iostream>
#include <string>
#include <chrono>
#include "neworderrandom.h"
#include "Database.h"

using namespace std;

int main() {
    using namespace string_literals;
    auto & database = Database::instance();
    database.importDatabaseFromPath("../tbls/"s);

    cout << "Imported database:" << '\n';
    cout << "NewOrders lines: " << database.newOrders.getView().size() << '\n';
    cout << "Orders lines: " << database.orders.getView().size() << '\n';
    cout << "OrderLines lines: " << database.orderLines.getView().size() << std::endl;

    auto start = std::chrono::steady_clock::now();
    for(int i = 0; i < 1'000'000; ++i) {
        newOrderRandom();
    }
    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration<double, std::milli>(end-start).count();
    cout << ms << "ms spent" << '\n';
    cout << 1'000'000 / (ms / 1'000)  << " Transactions per second" << '\n';
    cout << "Database after a million random Orders" << '\n';
    cout << "NewOrders lines: " << database.newOrders.getView().size() << '\n';
    cout << "Orders lines: " << database.orders.getView().size() << '\n';
    cout << "OrderLines lines: " << database.orderLines.getView().size() << std::endl;
    return 0;
}
