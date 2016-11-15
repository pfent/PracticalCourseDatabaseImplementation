#include <atomic>
#include <iostream>
#include <string>
#include <chrono>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "neworderrandom.h"
#include "tpc_c.h"
#include "Database.h"

using namespace std;

#ifdef NDEBUG
constexpr auto iterations = 1'000'000;
#else
constexpr auto iterations = 100000;
#endif //NDEBUG

static void runQuery();

atomic<bool> childRunning;

static void SIGCHLD_handler(int /*sig*/) {
    int status;
    wait(&status);
    // now the child with process id childPid is dead
    childRunning = false;
}

int main() {
    using namespace string_literals;
    auto& db = Database::instance();
    db.importDatabaseFromPath("../tbls/"s);

    cout << "Imported database:" << '\n';
    cout << "NewOrders lines: " << db.neworder.size << '\n';
    cout << "Orders lines: " << db.order.size << '\n';
    cout << "OrderLines lines: " << db.orderline.size << endl;

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIGCHLD_handler;
    sigaction(SIGCHLD, &sa, NULL);

    cout << "Running transactions …" << endl;
    const auto start = chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        pid_t pid = ~0;
        const auto start = chrono::steady_clock::now();
        if (!childRunning) {
            childRunning = true;
            pid = fork();
        }
        if (pid) { // parent
            oltp(Timestamp(0));
        } else { // forked child
            const auto end = chrono::steady_clock::now();
            cout << "fork took " << chrono::duration<double, milli>(end - start).count() << "ms" << endl;
            runQuery();
            return 0; // child is finished
        }
    }
    const auto end = chrono::steady_clock::now();
    while(childRunning); // wait for child finishing
    const auto s = chrono::duration<double>(end - start).count();

    cout << s << "s spent" << '\n';
    cout << iterations / s  << " Transactions per second" << '\n';

    cout << "Database after a million random Orders" << '\n';
    cout << "NewOrders lines: " << db.neworder.size << '\n';
    cout << "Orders lines: " << db.order.size << '\n';
    cout << "OrderLines lines: " << db.orderline.size << endl;

    return 0;
}

static void runQuery() {
    double executionTime = 0;
    Numeric<12, 4> printSum;
    constexpr int iterations = 10;
    for (int i = 0; i < iterations; ++i) {
        const auto start = chrono::steady_clock::now();
        printSum = customHashTblJoinQuery();
        const auto end = chrono::steady_clock::now();
        executionTime += chrono::duration<double, milli>(end - start).count();
    }
    cout << "sum = " << printSum << endl;
    cout << "query took avg. " << executionTime / iterations << "ms" << endl;
}
