#include <iostream>
#include <string>
#include "neworderrandom.h"
#include "Database.h"

#ifdef NDEBUG
#undef assert
#define assert(X) if(!(X)) throw std::runtime_error("\nassertion failed:\n" #X);
#endif

using namespace std;

template <typename T, typename Func>
void testIndexIntegrity(T table, Func&& assertion) {
    for (size_t i = 0; i < table.size; ++i) {
        auto elem = table.getRow(i);
        auto key = elem.getKey();
        auto elemTest = table.getRowForKey(key);
        assert(assertion(elem, elemTest));
    }
}

void testIndices(Database& db);
void testUpdate(Database& db);

int main() {
    using namespace string_literals;
    auto& db = Database::instance();
    db.importDatabaseFromPath("../tbls/"s);

    testIndices(db);

    for (int i = 0; i < 1'000'000; ++i) {
        newOrderRandom(Timestamp(0));
    }

    testIndices(db);

    testUpdate(db);

    cout << "Everything looking good" << endl;

    return 0;
}

void testIndices(Database& db) {
    testIndexIntegrity(db.warehouse, [](auto a, auto b) {
        return
            a.w_id == b.w_id &&
            a.w_name == b.w_name &&
            a.w_street_1 == b.w_street_1 &&
            a.w_street_2 == b.w_street_2 &&
            a.w_city == b.w_city &&
            a.w_state == b.w_state &&
            a.w_zip == b.w_zip &&
            a.w_tax == b.w_tax &&
            a.w_ytd == b.w_ytd;
    });
    testIndexIntegrity(db.district, [](auto a, auto b) {
        return
            a.d_id == b.d_id &&
            a.d_w_id == b.d_w_id &&
            a.d_name == b.d_name &&
            a.d_street_1 == b.d_street_1 &&
            a.d_street_2 == b.d_street_2 &&
            a.d_city == b.d_city &&
            a.d_state == b.d_state &&
            a.d_zip == b.d_zip &&
            a.d_tax == b.d_tax &&
            a.d_ytd == b.d_ytd &&
            a.d_next_o_id == b.d_next_o_id;
    });
    testIndexIntegrity(db.customer, [](auto a, auto b) {
        return
            a.c_id == b.c_id &&
            a.c_d_id == b.c_d_id &&
            a.c_w_id == b.c_w_id &&
            a.c_first == b.c_first &&
            a.c_middle == b.c_middle &&
            a.c_last == b.c_last &&
            a.c_street_1 == b.c_street_1 &&
            a.c_street_2 == b.c_street_2 &&
            a.c_city == b.c_city &&
            a.c_state == b.c_state &&
            a.c_zip == b.c_zip &&
            a.c_phone == b.c_phone &&
            a.c_since == b.c_since &&
            a.c_credit == b.c_credit &&
            a.c_credit_lim == b.c_credit_lim &&
            a.c_discount == b.c_discount &&
            a.c_balance == b.c_balance &&
            a.c_ytd_paymenr == b.c_ytd_paymenr &&
            a.c_payment_cnt == b.c_payment_cnt &&
            a.c_delivery_cnt == b.c_delivery_cnt &&
            a.c_data == b.c_data;
    });
    testIndexIntegrity(db.neworder, [](auto a, auto b) {
        return
            a.no_d_id == b.no_d_id &&
            a.no_o_id == b.no_o_id &&
            a.no_w_id == b.no_w_id;
    });
    testIndexIntegrity(db.order, [](auto a, auto b) {
        return
            a.o_id == b.o_id &&
            a.o_d_id == b.o_d_id &&
            a.o_w_id == b.o_w_id &&
            a.o_c_id == b.o_c_id &&
            a.o_entry_d == b.o_entry_d &&
            a.o_carrier_id == b.o_carrier_id &&
            a.o_ol_cnt == b.o_ol_cnt &&
            a.o_all_local == b.o_all_local;


    });
    testIndexIntegrity(db.orderline, [](auto a, auto b) {
        return
            a.ol_o_id == b.ol_o_id &&
            a.ol_d_id == b.ol_d_id &&
            a.ol_w_id == b.ol_w_id &&
            a.ol_number == b.ol_number &&
            a.ol_i_id == b.ol_i_id &&
            a.ol_supply_w_id == b.ol_supply_w_id &&
            a.ol_delivery_d == b.ol_delivery_d &&
            a.ol_quantity == b.ol_quantity &&
            a.ol_amount == b.ol_amount &&
            a.ol_dist_info == b.ol_dist_info;
    });
    testIndexIntegrity(db.item, [](auto a, auto b) {
        return
            a.i_id == b.i_id &&
            a.i_im_id == b.i_im_id &&
            a.i_name == b.i_name &&
            a.i_price == b.i_price &&
            a.i_data == b.i_data;

    });
    testIndexIntegrity(db.stock, [](auto a, auto b) {
        return
            a.s_i_id == b.s_i_id &&
            a.s_w_id == b.s_w_id &&
            a.s_quantity == b.s_quantity &&
            a.s_dist_01 == b.s_dist_01 &&
            a.s_dist_02 == b.s_dist_02 &&
            a.s_dist_03 == b.s_dist_03 &&
            a.s_dist_04 == b.s_dist_04 &&
            a.s_dist_05 == b.s_dist_05 &&
            a.s_dist_06 == b.s_dist_06 &&
            a.s_dist_07 == b.s_dist_07 &&
            a.s_dist_08 == b.s_dist_08 &&
            a.s_dist_09 == b.s_dist_09 &&
            a.s_dist_10 == b.s_dist_10 &&
            a.s_ytd == b.s_ytd &&
            a.s_order_cnt == b.s_order_cnt &&
            a.s_remote_cnt == b.s_remote_cnt &&
            a.s_data == b.s_data;
    });
}

void testUpdate(Database& db) {
    for (size_t i = 0; i < db.district.size; ++i) {
        auto districtLine = db.district.getRow(i);
        const auto o_id = districtLine.d_next_o_id;
        districtLine.d_next_o_id = o_id + 1;
        db.district.update(districtLine);

        const auto districtLine2 = db.district.getRow(i);
        assert(districtLine.d_id == districtLine2.d_id &&
               districtLine.d_next_o_id == districtLine2.d_next_o_id);
    }
}


