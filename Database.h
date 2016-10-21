#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include "loadTbls.h"

template <typename T>
struct Table {
    friend struct Database;
    const std::vector<T>& getView();
    void insert(T&&);
    void remove(size_t);
private:
    std::vector<T> table;
};

struct Database {
    static Database& instance();
    void importDatabaseFromPath(const std::string& path);
    void importDatabaseFromPath(const std::string&& path);

    const std::vector<WareHouse>& getWareHouses();
    const std::vector<District>& getDistricts();
    const std::vector<Customer>& getCustomers();
    const std::vector<History>& getHistorys();
    const std::vector<NewOrder>& getNewOrders();
    const std::vector<Order>& getOrders();
    const std::vector<OrderLine>& getOrderLines();
    const std::vector<Item>& getItems();
    const std::vector<Stock>& getStocks();

    std::vector<WareHouse> wareHouses;
    std::vector<District> districts;
    std::vector<Customer> customers;
    std::vector<History> historys;
    std::vector<NewOrder> newOrders;
    std::vector<Order> orders;
    std::vector<OrderLine> orderLines;
    std::vector<Item> items;
    std::vector<Stock> stocks;
private:
    Database(){};
    Database(const Database&) = delete; // no copying or moving of the database plz
    Database& operator = (const Database&) = delete;
};

#endif // DATABASE_H
