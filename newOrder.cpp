#include "newOrder.h"
#include "Database.h"
#include <tuple>
#include <algorithm>

template<typename T, typename U>
T getElemForKey(Table<T> table, U key) {
    const auto index = T::index[key];
    return table[index];
}

using namespace std;
void newOrder(int32_t w_id, int32_t d_id, int32_t c_id, int32_t items, int32_t supware[], int32_t itemid[], int32_t qty[], Timestamp datetime) {
    auto& db = Database::instance();
//     select w_tax from warehouse w where w.w_id = w_id;
    const auto w_tax = getElemForKey(db.wareHouses, w_id).w_tax;
//     select c_discount from customer c where c_w_id = w_id and c_d_id = d_id and c.c_id = c_id;
    const auto c_discount = getElemForKey(db.customers, make_tuple(w_id, d_id, c_id)).c_discount;

//     select d_next_o_id as o_id, d_tax from district d where d_w_id = w_id and d.d_id = d_id;
    auto districtLine = getElemForKey(db.districts, make_tuple(w_id, d_id));
    const auto o_id = districtLine.d_next_o_id;
    const auto d_tax = districtLine.d_tax;
//     update district set d_next_o_id = o_id + 1 where d_w_id = w_id and district.d_id = d_id;
    districtLine.d_next_o_id = o_id + 1;
    db.districts.update(districtLine);

//     var integer all_local = 1;
//     forsequence(index between 0 and items - 1) {
//         if (w_id<>supware[index])
//             all_local = 0;
//     }
    const auto all_local = all_of(supware, supware + items, [&](int32_t item) {
        return w_id == item;
    });
//     insert into "order" values(o_id, d_id, w_id, c_id, datetime, 0, items, all_local);
    db.orders.insert({o_id, d_id, w_id, c_id, datetime, 0, items, all_local});
//     insert into neworder values(o_id, d_id, w_id);
    db.newOrders.insert({o_id, d_id, w_id});
//     forsequence(index between 0 and items - 1) {
    for (int index = 0; index < items; ++index) {
//         select i_price from item where i_id = itemid[index];
        const auto i_price = getElemForKey(db.items, itemid[index]).i_price;
//     select s_quantity, s_remote_cnt, s_order_cnt, case d_id when 1 then s_dist_01 when 2 then s_dist_02 when 3 then s_dist_03 when 4 then s_dist_04 when 5 then s_dist_05 when 6 then s_dist_06 when 7 then s_dist_07 when 8 then s_dist_08 when 9 then s_dist_09 when 10 then s_dist_10 end as s_dist from stock where s_w_id=supware[index] and s_i_id=itemid[index];
        const auto stockLine = getElemForKey(db.stocks, make_tuple(supware[index], itemid[index]));
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
//         if (s_quantity > qty[index]) {
//             update stock set s_quantity = s_quantity - qty[index] where s_w_id = supware[index] and s_i_id = itemid[index];
//         } else {
//             update stock set s_quantity = s_quantity + 91 - qty[index] where s_w_id = supware[index] and s_i_id = itemid[index];
//         }
        {
            auto stockLine = getElemForKey(db.stocks, make_tuple(supware[index], itemid[index]));
            if (s_quantity > qty[index]) {
                stockLine.s_quantity = s_quantity - qty[index];
            } else {
                stockLine.s_quantity = s_quantity + 91 - qty[index];
            }
            db.stocks.update(stockLine);
        }
//         if (supware[index]<>w_id) {
//             update stock set s_remote_cnt = s_remote_cnt + 1 where s_w_id = w_id and s_i_id = itemid[index];
//         } else {
//             update stock set s_order_cnt = s_order_cnt + 1 where s_w_id = w_id and s_i_id = itemid[index];
//         }
        {
            auto stockLine = getElemForKey(db.stocks, make_tuple(w_id, itemid[index]));
            if (supware[index] != w_id) {
                stockLine.s_remote_cnt = s_remote_cnt + 1;
            } else {
                stockLine.s_order_cnt = s_order_cnt + 1;
            }
            db.stocks.update(stockLine);
        }
//         var numeric(6, 2) ol_amount = qty[index] * i_price * (1.0 + w_tax + d_tax) * (1.0 - c_discount);
        const auto tax = (Numeric<4, 4>(1.0) + w_tax + d_tax) * (Numeric<4, 4>(1.0) - c_discount);
        const auto price = Numeric<5, 2>(qty[index]) * i_price;
        const auto unrounded_ol_amount = price.castP2().castP2() * tax.castS<5>();
        const Numeric<6, 2> ol_amount = unrounded_ol_amount.castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>().castM2<6>();
//         insert into orderline values(o_id, d_id, w_id, index + 1, itemid[index], supware[index], 0, qty[index], ol_amount, s_dist);
        db.orderLines.insert({o_id, d_id, w_id, index + 1, itemid[index], supware[index], 0, qty[index], ol_amount, s_dist});
//     }
    }
//
//     commit;
}
