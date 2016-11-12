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
#include "query/Selection.h"

using namespace std;

void task4Query(Schema *schema) {
    auto customer = TableScan(schema->find("customer"));
    auto order = TableScan(schema->find("\"order\""));
    auto orderline = TableScan(schema->find("orderline"));

    auto cSelection = Selection(customer, {{customer.getIU("c_id"), "322"},
                                           {customer.getIU("c_w_id"), "1"},
                                           {customer.getIU("c_d_id"), "1"}});

    auto firstHash = HashJoin(customer, order, {{customer.getIU("c_w_id"), order.getIU("o_w_id")},
                                                {customer.getIU("c_d_id"), order.getIU("o_d_id")},
                                                {customer.getIU("c_id"),   order.getIU("o_c_id")}});
    auto secondHash = HashJoin(firstHash, orderline, {{order.getIU("o_w_id"), orderline.getIU("ol_w_id")},
                                                      {order.getIU("o_d_id"), orderline.getIU("ol_d_id")},
                                                      {order.getIU("o_id"),   orderline.getIU("ol_o_id")}});

    auto res = Printer(secondHash, {customer.getIU("c_first"), customer.getIU("c_last"), order.getIU("o_all_local"),
                                    orderline.getIU("ol_amount")});

    cout << res.produce();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <schema file>" << std::endl;
        return -1;
    }

    Parser p(argv[1]);

    try {
        std::unique_ptr<Schema> schema = p.parse();
        task4Query(schema.get());

    } catch (ParserError &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
