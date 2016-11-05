#include <iostream>
#include <string>
#include <chrono>
#include "neworderrandom.h"
#include "tpc_c.h"
#include "Database.h"

using namespace std;

#ifdef NDEBUG
constexpr auto iterations = 1'000'000;
#else
constexpr auto iterations = 10000;
#endif //NDEBUG

void runTransactions();
void runQuery();

int main() {
    using namespace string_literals;
    auto& db = Database::instance();
    db.importDatabaseFromPath("../tbls/"s);

    cout << "Imported database:" << '\n';
    cout << "NewOrders lines: " << db.neworder.size << '\n';
    cout << "Orders lines: " << db.order.size << '\n';
    cout << "OrderLines lines: " << db.orderline.size << std::endl;

    //    cout << "Press enter to continue …" << std::endl;
    //    cin.ignore();
    runQuery();

    //    runTransactions();
    cout << "Database after a million random Orders" << '\n';
    cout << "NewOrders lines: " << db.neworder.size << '\n';
    cout << "Orders lines: " << db.order.size << '\n';
    cout << "OrderLines lines: " << db.orderline.size << std::endl;

    return 0;
}

void runQuery() {
    double executionTime = 0;
    constexpr int iterations = 10;
    for (int i = 0; i < iterations; ++i) {
        const auto start = std::chrono::steady_clock::now();
        const auto sum = joinQuery();
        cout << "sum = " << sum << std::endl;
        const auto end = std::chrono::steady_clock::now();
        executionTime += std::chrono::duration<double, std::milli>(end - start).count();
    }
    cout << "query took avg. " << executionTime / iterations << "ms" << std::endl;
}

void runTransactions() {
    cout << "Running transactions …" << std::endl;
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        oltp(Timestamp(0));
    }
    const auto end = std::chrono::steady_clock::now();
    const auto s = std::chrono::duration<double>(end - start).count();

    cout << s << "s spent" << '\n';
    cout << iterations / s  << " Transactions per second" << '\n';
}
