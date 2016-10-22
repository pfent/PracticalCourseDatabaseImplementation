#include <iostream>
#include <string>
#include <chrono>
#include "neworderrandom.h"
#include "Database.h"

using namespace std;

constexpr auto iterations = 1'000'000;

int main() {
    using namespace string_literals;
    auto& db = Database::instance();
    db.importDatabaseFromPath("../tbls/"s);

    cout << "Imported database:" << '\n';
    cout << "NewOrders lines: " << db.newOrders.getView().size() << '\n';
    cout << "Orders lines: " << db.orders.getView().size() << '\n';
    cout << "OrderLines lines: " << db.orderLines.getView().size() << std::endl;

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        newOrderRandom();
    }
    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration<double, std::milli>(end - start).count();
    cout << ms << "ms spent" << '\n';
    cout << iterations / (ms / 1'000)  << " Transactions per second" << '\n';
    cout << "Database after a million random Orders" << '\n';
    cout << "NewOrders lines: " << db.newOrders.getView().size() << '\n';
    cout << "Orders lines: " << db.orders.getView().size() << '\n';
    cout << "OrderLines lines: " << db.orderLines.getView().size() << std::endl;

    return 0;
}
