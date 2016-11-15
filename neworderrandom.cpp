#include <cstdint>
#include "tpc_c.h"
#include "Database.h"
#include <iostream>
#include <tuple>

using namespace std;

namespace {
    const int32_t warehouses = 5;

    int32_t urand(int32_t min, int32_t max) {
        return (random() % (max - min + 1)) + min;
    }

    int32_t urandexcept(int32_t min, int32_t max, int32_t v) {
        if (max <= min)
            return min;
        int32_t r = (random() % (max - min)) + min;
        if (r >= v)
            return r + 1;
        else
            return r;
    }

    int32_t nurand(int32_t A, int32_t x, int32_t y) {
        return ((((random() % A) | (random() % (y - x + 1) + x)) + 42) % (y - x + 1)) + x;
    }
}

void newOrderRandom(Timestamp now) {
    int32_t w_id = urand(1, warehouses);
    int32_t d_id = urand(1, 10);
    int32_t c_id = nurand(1023, 1, 3000);
    int32_t ol_cnt = urand(5, 15);

    int32_t supware[15];
    int32_t itemid[15];
    int32_t qty[15];
    for (int32_t i = 0; i < ol_cnt; i++) {
        if (urand(1, 100) > 1)
            supware[i] = w_id;
        else
            supware[i] = urandexcept(1, warehouses, w_id);
        itemid[i] = nurand(8191, 1, 100000);
        qty[i] = urand(1, 10);
    }

    newOrder(w_id, d_id, c_id, ol_cnt, supware, itemid, qty, now);
}

void deliveryRandom(Timestamp now) {
    delivery(urand(1, warehouses), urand(1, 10), now);
}

void oltp(Timestamp now) {
    int rnd = urand(1, 100);
    if (rnd <= 10) {
        deliveryRandom(now);
    } else {
        newOrderRandom(now);
    }
}

template<typename Key, typename Value>
struct HashTbl {
    const size_t bitmask;
    const size_t size;
    size_t *hashes;
    pair<Key, Value> *values;

    HashTbl(const vector<pair<Key, Value>> &datas) :
            bitmask(getNextPow2(datas.size()) - 1),
            size(getNextPow2(datas.size())) {
        hashes = new size_t[size]();
        values = new pair<Key, Value>[size]();
        for (const auto &data : datas) {
            insert(data);
        }
    }

    ~HashTbl() {
        delete[](hashes);
        delete[](values);
    }

    void insert(const pair<Key, Value> &data) {
        auto toInsert = data;
        auto hash = hashKey(data.first);
        auto i = position(hash);

        // Now linear probing at i
        for (size_t distance = 0;; ++distance) {
            if (hashes[i] == 0) { // empty slot
                hashes[i] = hash;
                values[i] = toInsert;
                return;
            }
            const auto existingProbeDistance = hashDistance(hashes[i], i);
            if (existingProbeDistance < distance) {
                // Robin hood: take from the good located, give the bad located
                values[i].swap(toInsert);
                swap<size_t>(hashes[i], hash);
                distance = existingProbeDistance;
            }
            i = (i + 1) & bitmask; // wraparound
        }
    }

    auto find(Key key) const {
        const auto hash = hashKey(key);
        auto i = position(hash);
        for (size_t distance = 0;; ++distance) {
            if (hashes[i] == 0) { // uninitialized
                return values + size;
            }
            if (distance > hashDistance(hashes[i], i)) {
                // if we encounter an element with higher distance, we know
                // the element we are looking for would had been swapped
                return values + size;
            }
            if (hashes[i] == hash && values[i].first == key) {
                return values + i;
            }
            i = position(i + 1); // wraparound
        }
    };

    auto end() const {
        return values + size;
    }

    static inline size_t getNextPow2(const size_t size) {
        size_t pow = 1;
        while (pow < size) {
            pow *= 2;
        }
        return pow;
    }

    static inline size_t hashKey(const Key key) {
        size_t h = std::hash<Key>()(key);
        // never return 0 as hash, as it is reserved for uninitialized values
        h |= h == 0;
        return h;
    }

    inline size_t position(const size_t hash) const {
        return hash & bitmask;
    }

    inline size_t hashDistance(const size_t hash, const size_t i) const {
        return (i + (bitmask + 1) - position(hash)) & bitmask;
    }
};

Numeric<12, 4> joinQuery() {
    auto &db = Database::instance();
    auto customerOrderHashTbl = std::unordered_map<std::tuple<Integer, Integer, Integer> /*c_w_id, c_d_id, c_id*/, size_t> {};
    const auto customerOrderHashTblExpectedSize = db.customer.size * (0.12801);
    customerOrderHashTbl.reserve(
            customerOrderHashTblExpectedSize); // avoid rehashing, "like 'B%'" should have a selectivity of ~1 in 26 but in our dataset it's ~1/8
    for (size_t i = 0; i < db.customer.size; ++i) {
        if (db.customer.c_last[i].value[0] == 'B') { // like B%
            customerOrderHashTbl[{db.customer.c_w_id[i], db.customer.c_d_id[i], db.customer.c_id[i]}] = i;
        }
    }

    auto customerOrderOrderlineHashTbl = std::unordered_map<std::tuple<Integer, Integer, Integer> /*ol_w_id, ol_d_id, ol_o_id*/, std::tuple<size_t, size_t>> {};
    const auto customerOrderOrderlineHashTblExpectedSize = customerOrderHashTbl.size() * db.order.size * 0.0000086;
    customerOrderOrderlineHashTbl.reserve(
            customerOrderOrderlineHashTblExpectedSize); // avoid rehashing, orderline has a selectivity of around 1/2^17 in our dataset
    for (size_t i = 0; i < db.order.size; ++i) {
        const auto pos = customerOrderHashTbl.find({db.order.o_w_id[i], db.order.o_d_id[i], db.order.o_c_id[i]});

        if (pos != customerOrderHashTbl.end()) {
            // found a join partner
            const auto cIndex = pos->second;
            customerOrderOrderlineHashTbl[{db.order.o_w_id[i], db.order.o_d_id[i], db.order.o_id[i]}] = {cIndex, i};
        }
    }

    Numeric<12, 4> sum = 0;
    for (size_t i = 0; i < db.orderline.size; ++i) {
        const auto pos = customerOrderOrderlineHashTbl.find(
                {db.orderline.ol_w_id[i], db.orderline.ol_d_id[i], db.orderline.ol_o_id[i]});

        if (pos != customerOrderOrderlineHashTbl.end()) {
            const auto cIndex = std::get<0>(pos->second);
            const auto oIndex = std::get<1>(pos->second);

            // sum += ol_quantity*ol_amount-c_balance*o_ol_cnt
            sum += (db.orderline.ol_quantity[i].castP2().castS<6>() * db.orderline.ol_amount[i]).castS<12>() -
                   db.customer.c_balance[cIndex] * db.order.o_ol_cnt[oIndex].castP2().castS<12>();
        }
    }
    return sum;
//     select sum(ol_quantity*ol_amount-c_balance*o_ol_cnt)
//     from customer, "order", orderline
//     where o_w_id = c_w_id
//     and o_d_id = c_d_id
//     and o_c_id = c_id
//     and o_w_id = ol_w_id
//     and o_d_id = ol_d_id
//     and o_id = ol_o_id
//     and c_last like 'B%'
}

Numeric<12, 4> customHashTblJoinQuery() {
    auto &db = Database::instance();
    auto buffer1 = std::vector<pair<std::tuple<Integer, Integer, Integer>, size_t>>{};
    for (size_t i = 0; i < db.customer.size; ++i) {
        if (db.customer.c_last[i].value[0] == 'B') { // like B%
            buffer1.push_back({{db.customer.c_w_id[i], db.customer.c_d_id[i], db.customer.c_id[i]}, i});
        }
    }
    const auto customerOrderHashTbl = HashTbl<std::tuple<Integer, Integer, Integer> /*c_w_id, c_d_id, c_id*/, size_t>(
            buffer1);

    auto buffer2 = std::vector<std::pair<std::tuple<Integer, Integer, Integer> /*ol_w_id, ol_d_id, ol_o_id*/, std::tuple<size_t, size_t>>> {};
    for (size_t i = 0; i < db.order.size; ++i) {
        const auto pos = customerOrderHashTbl.find({db.order.o_w_id[i], db.order.o_d_id[i], db.order.o_c_id[i]});

        if (pos != customerOrderHashTbl.end()) {
            // found a join partner
            const auto cIndex = pos->second;
            buffer2.push_back({{db.order.o_w_id[i], db.order.o_d_id[i], db.order.o_id[i]},
                               {cIndex,             i}});
        }
    }
    const auto customerOrderOrderlineHashTbl = HashTbl<std::tuple<Integer, Integer, Integer> /*ol_w_id, ol_d_id, ol_o_id*/, std::tuple<size_t, size_t>>(
            buffer2);

    Numeric<12, 4> sum = 0;
    for (size_t i = 0; i < db.orderline.size; ++i) {
        const auto pos = customerOrderOrderlineHashTbl.find(
                {db.orderline.ol_w_id[i], db.orderline.ol_d_id[i], db.orderline.ol_o_id[i]});

        if (pos != customerOrderOrderlineHashTbl.end()) {
            const auto cIndex = std::get<0>(pos->second);
            const auto oIndex = std::get<1>(pos->second);

            // sum += ol_quantity*ol_amount-c_balance*o_ol_cnt
            sum += (db.orderline.ol_quantity[i].castP2().castS<6>() * db.orderline.ol_amount[i]).castS<12>() -
                   db.customer.c_balance[cIndex] * db.order.o_ol_cnt[oIndex].castP2().castS<12>();
        }
    }
    return sum;
}







