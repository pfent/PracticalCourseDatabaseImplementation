#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include "loadTbls.h"

template <typename T>
struct Table {
    friend struct Database; // the database should directly update the table and indices
    const std::vector<T>& getView() const {
        return table;
    }

    void insert(T&& element) {
        const auto index = table.size();
        table.push_back(element);
        T::index[element.getKey()] = index;
    }

    void update(T& element) {
        const auto index = T::index[element.getKey()];
        table[index] = element;
    }

    void remove(size_t); //TODO
    T& operator[](size_t index) {
        return table[index];
    }

    const T& operator[](size_t index) const {
        return table[index];
    }

    size_t size() const {
        return table.size();
    }
private:
    std::vector<T> table;
};

struct Database {
    static Database& instance();
    void importDatabaseFromPath(const std::string& path);
    void importDatabaseFromPath(const std::string&& path);

    Table<WareHouse> wareHouses;
    Table<District> districts;
    Table<Customer> customers;
    Table<History> historys;
    Table<NewOrder> newOrders;
    Table<Order> orders;
    Table<OrderLine> orderLines;
    Table<Item> items;
    Table<Stock> stocks;
private:
    Database() {};
    Database(const Database&) = delete; // no copying or moving of the database plz
    Database& operator = (const Database&) = delete;
};

#endif // DATABASE_H
