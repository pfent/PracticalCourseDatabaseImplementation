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
constexpr auto iterations = 10'000;
#endif //NDEBUG

int main() {
    using namespace string_literals;
    auto& db = Database::instance();
    db.importDatabaseFromPath("../tbls/"s);

    cout << "Imported database:" << '\n';
    cout << "NewOrders lines: " << db.neworder.size << '\n';
    cout << "Orders lines: " << db.order.size << '\n';
    cout << "OrderLines lines: " << db.orderline.size << std::endl;

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        oltp(Timestamp(0));
    }
    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration<double, std::milli>(end - start).count();
    cout << ms << "ms spent" << '\n';
    cout << iterations / (ms / 1'000)  << " Transactions per second" << '\n';
    cout << "Database after a million random Orders" << '\n';
    cout << "NewOrders lines: " << db.neworder.size << '\n';
    cout << "Orders lines: " << db.order.size << '\n';
    cout << "OrderLines lines: " << db.orderline.size << std::endl;

    return 0;
}
