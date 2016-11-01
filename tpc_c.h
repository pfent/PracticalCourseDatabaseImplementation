#ifndef NEWORDER_H
#define NEWORDER_H
#include "Types.h"
#include <array>
#include <cstdint>

void newOrder(int32_t w_id, int32_t d_id, int32_t c_id, int32_t items, int32_t supware[], int32_t itemid[], int32_t qty[], Timestamp datetime);

void delivery(int32_t w_id, int32_t o_carrier_id, Timestamp datetime);

#endif // NEWORDER_H
