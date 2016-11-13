# PracticalCourseDatabaseImplementation

# Cmake compile instructions
```bash
mkdir buildRelease
cd buildRelease
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release ..
# First, generate code according for the database
cd parser
make
./parser ../../schema.sql > ../../Database.h
./query ../../schema.sql > ../../query.cpp
cd ..
make
```

# Execute with
```bash
./queryh4
```

or run a unit test with
```bash
./unitTest
```

# Sample output
```
wLzEBrpKbbfeb01 PRIABLEOUGHT    1       0.00
wLzEBrpKbbfeb01 PRIABLEOUGHT    1       0.00
wLzEBrpKbbfeb01 PRIABLEOUGHT    1       0.00
wLzEBrpKbbfeb01 PRIABLEOUGHT    1       0.00
wLzEBrpKbbfeb01 PRIABLEOUGHT    1       0.00
wLzEBrpKbbfeb01 PRIABLEOUGHT    1       0.00
wLzEBrpKbbfeb01 PRIABLEOUGHT    1       0.00
wLzEBrpKbbfeb01 PRIABLEOUGHT    1       0.00
```
