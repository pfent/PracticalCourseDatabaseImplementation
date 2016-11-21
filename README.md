# PracticalCourseDatabaseImplementation

# Cmake compile instructions
```bash
mkdir buildRelease
cd buildRelease
cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release ..
# Parse the schema to generate code for the Database
cd parser
make
./parser ../../schema.sql > ../../Database.h
cd ..
make
```

Please also note, that the database expects a database dump to read in `tbls`. So please copy your `tpcc_*.tbl` to the `tbl` directory.

# Execute with
```bash
rlwrap ./practicalcoursedatabaseimplementation
```

# Sample output
```
Initializing database… took 4352.37ms
working directory: /tmp/tmp.FqdEcL9AC1
input a SQL query or finish execution by EOF (^D / Ctrl-D)
> select w_id from warehouse;
compiling… took 1394.32ms
4
1
5
2
3
took 0.083286ms
> ^D
```
