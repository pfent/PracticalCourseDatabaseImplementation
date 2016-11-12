#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include <string>
#include "Parser.hpp"
#include "Schema.hpp"
#include "query/TableScan.h"
#include "query/HashJoin.h"
#include "query/Printer.h"

using namespace std;

void task4Query (Schema* schema) {
    auto customer = TableScan (schema->find ("customer"));
    auto order = TableScan (schema->find ("\"order\""));
    auto orderline = TableScan (schema->find ("orderline"));

    auto firstHash = HashJoin (customer, order, {{"c_id", "o_c_id"}, {"c_d_id", "o_d_id"}});
    auto secondHash = HashJoin (firstHash, orderline, {{"o_w_id", "ol_w_id"}, {"o_id", "ol_o_id"}});

    auto res = Printer (secondHash, {"c_first"});
    
    cout << res.produce();
}

int main (int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <schema file>" << std::endl;
        return -1;
    }

    Parser p (argv[1]);

    try {
        std::unique_ptr<Schema> schema = p.parse();
        task4Query(schema.get());

    } catch (ParserError& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
