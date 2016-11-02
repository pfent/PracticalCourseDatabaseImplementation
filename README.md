# PracticalCourseDatabaseImplementation

# Cmake compile instructions
```bash
mkdir buildRelease
cd buildRelease
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release ..
cd parser
make parser && ./parser ../../schema.sql > ../../Database.h
cd ..
make
```

# Execute with
```bash
./practicalcoursedatabaseimplementation
```

or run a unit test with
```bash
./unitTest
```

# Sample output
```
Imported database:
NewOrders lines: 45000
Orders lines: 150000
OrderLines lines: 1425564
14178.2ms spent
70530.8 Transactions per second
Database after a million random Orders
NewOrders lines: 1045000
Orders lines: 1150000
OrderLines lines: 11426900
```
