#ifndef NEWORDERRANDOM_H
#define NEWORDERRANDOM_H
#include "Types.h"

void newOrderRandom(Timestamp now);

void deliveryRandom(Timestamp now);

void oltp(Timestamp now);

Numeric<12, 4> joinQuery();

Numeric<12, 4> customHashTblJoinQuery();

#endif //NEWORDERRANDOM_H
