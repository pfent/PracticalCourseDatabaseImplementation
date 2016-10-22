#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include "loadTbls.h"

template <typename T>
struct Table {
    friend struct Database; // the database should directly update the table and indices
    using KeyType = decltype(((T*)nullptr)->getKey());
    const std::vector<T>& getView() const {
        return table;
    }

    T getElemForKey(KeyType key) {
        return table[index[key]];
    }

    void insert(T&& element) {
        const auto i = table.size();
        table.push_back(element);
        index[element.getKey()] = i;
    }

    void update(T& element) {
        const auto i = index[element.getKey()];
        table[i] = element;
    }

    void buildIndex() {
        const auto size = table.size();
        index.reserve(size);
        for (size_t i = 0; i < size; i++) {
            index[table[i].getKey()] = i;
        }
    }

    T& operator[](size_t i) {
        return table[i];
    }

    size_t size() const {
        return table.size();
    }
private:
    std::vector<T> table;
    std::unordered_map<KeyType, size_t> index;
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
