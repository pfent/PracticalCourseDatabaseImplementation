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
Running transactions …
fork took 3.7621ms
sum = 1367872001.2500
query took avg. 56.5211ms
fork took 4.92023ms
sum = -36177990988.8000
query took avg. 84.7377ms
fork took 5.7144ms
sum = -49810665200.0900
query took avg. 121.548ms
fork took 7.2989ms
sum = -73581681145.7300
query took avg. 185.158ms
fork took 9.30513ms
sum = -102086088480.3900
query took avg. 250.39ms
fork took 12.5807ms
sum = -156769658685.8800
query took avg. 397.843ms
19.0891s spent
52386 Transactions per second
Database after a million random Orders
NewOrders lines: 499963
Orders lines: 1049992
OrderLines lines: 10431287
```
