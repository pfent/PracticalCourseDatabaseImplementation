#include <iostream>
#include "neworderrandom.h"
#include "loadTbls.h"
#include <string>


int main() {
    using namespace std::string_literals;
    
    auto table = readTableFromFile<WareHouse>("../tbls/tpcc_warehouse.tbl"s);
    std::cout << "Read " << table.size() << " WareHouses" << std::endl;
    auto districts = readTableFromFile<District>("../tbls/tpcc_district.tbl"s);
    std::cout << "Read " << districts.size() << " Districts" << std::endl;
    newOrderRandom();
    return 0;
}
