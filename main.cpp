#include <iostream>
#include "neworderrandom.h"
#include "loadTbls.h"
#include <string>


int main() {
    using namespace std::string_literals;
    std::cout << "Hello, world!" << std::endl;
    auto table = readTableFromFile<WareHouse>("../tbls/tpcc_warehouse.tbl"s);
    auto districts = readTableFromFile<District>("../tbls/tpcc_district.tbl"s);
    newOrderRandom();
    return 0;
}
