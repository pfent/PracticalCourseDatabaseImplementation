#include <iostream>
#include <string>
#include <chrono>
#include <memory>
#include "Database.h"
#include "parser/Parser.hpp"
#include "parser/Schema.hpp"
#include "parser/QueryParser.hpp"

using namespace std;

auto& db = Database::instance();

static void loadDatabase(const string& location) {
    cout << "Initializing database… " << flush;
    const auto start = chrono::steady_clock::now();
    db.importDatabaseFromPath(location);
    const auto end = chrono::steady_clock::now();
    cout << "took " << chrono::duration<double, milli>(end - start).count() << "ms\n" << endl;
}

int main() {
    loadDatabase("../tbls/");
    unique_ptr<Schema> schema = Parser("../schema.sql").parse();

    cout << "input a SQL query or finish execution by EOF (^D / Ctrl-D)" << endl;
    for (string line; getline(cin, line);) {
        cout << "you entered " << line << endl;
        unique_ptr<Query> query;
        try {
            query = QueryParser(*schema, line).parse();
        } catch (QueryParserError& ex) {
            cerr << ex.what() << endl;
            continue;
        }
        cout << query->build() << endl;
    }

    return 0;
}
