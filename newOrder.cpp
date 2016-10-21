#include "newOrder.h"
#include "Database.h"
#include <tuple>
#include <algorithm>

using namespace std;
void newOrder(int32_t w_id, int32_t d_id, int32_t c_id, int32_t items, int32_t supware[], int32_t itemid[], int32_t qty[], Timestamp datetime) {
    auto& db = Database::instance();
    const auto w_tax = db.wareHouses.getElemForKey(w_id).w_tax;
    const auto c_discount = db.customers.getElemForKey(make_tuple(w_id, d_id, c_id)).c_discount;

    auto districtLine = db.districts.getElemForKey(make_tuple(w_id, d_id));
    const auto o_id = districtLine.d_next_o_id;
    const auto d_tax = districtLine.d_tax;
    districtLine.d_next_o_id = o_id + 1;
    db.districts.update(districtLine);

    const auto all_local = all_of(supware, supware + items, [&](int32_t item) {
        return w_id == item;
    });
    db.orders.insert({o_id, d_id, w_id, c_id, datetime, 0, items, all_local});
    db.newOrders.insert({o_id, d_id, w_id});
    for (int index = 0; index < items; ++index) {
        const auto i_price = db.items.getElemForKey(itemid[index]).i_price;
        const auto stockLine = db.stocks.getElemForKey(make_tuple(supware[index], itemid[index]));
        const auto s_quantity = stockLine.s_quantity;
        const auto s_remote_cnt = stockLine.s_remote_cnt;
        const auto s_order_cnt = stockLine.s_order_cnt;
        const auto s_dist = [&stockLine, d_id]() {
            switch (d_id) {
            default:
            case  1: return stockLine.s_dist_01;
            case  2: return stockLine.s_dist_02;
            case  3: return stockLine.s_dist_03;
            case  4: return stockLine.s_dist_04;
            case  5: return stockLine.s_dist_05;
            case  6: return stockLine.s_dist_06;
            case  7: return stockLine.s_dist_07;
            case  8: return stockLine.s_dist_01;
            case  9: return stockLine.s_dist_09;
            case 10: return stockLine.s_dist_10;
            }
        }();
        {
            auto stockLine = db.stocks.getElemForKey(make_tuple(supware[index], itemid[index]));
            if (s_quantity > qty[index]) {
                stockLine.s_quantity = s_quantity - qty[index];
            } else {
                stockLine.s_quantity = s_quantity + 91 - qty[index];
            }
            db.stocks.update(stockLine);
        }
        {
            auto stockLine = db.stocks.getElemForKey(make_tuple(w_id, itemid[index]));
            if (supware[index] != w_id) {
                stockLine.s_remote_cnt = s_remote_cnt + 1;
            } else {
                stockLine.s_order_cnt = s_order_cnt + 1;
            }
            db.stocks.update(stockLine);
        }
        const auto tax = (Numeric<4, 4>(1.0) + w_tax + d_tax) * (Numeric<4, 4>(1.0) - c_discount);
        const auto price = Numeric<5, 2>(qty[index]) * i_price;
        const auto unrounded_ol_amount = price.castP2().castP2() * tax.castS<5>();
        const Numeric<6, 2> ol_amount = unrounded_ol_amount.castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>();
        db.orderLines.insert({o_id, d_id, w_id, index + 1, itemid[index], supware[index], 0, qty[index], ol_amount, s_dist});
    }
}
