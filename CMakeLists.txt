cmake_minimum_required(VERSION 2.6)
project(practicalcoursedatabaseimplementation)

add_executable(practicalcoursedatabaseimplementation main.cpp Types.cpp parser/Parser.cpp parser/QueryParser.cpp parser/query/TableScan.cpp parser/query/Selection.cpp parser/query/HashJoin.cpp parser/query/Printer.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -fshow-column -pipe -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -O0 -DDEBUG -fsanitize=undefined -fsanitize=address -fsanitize=leak")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -DNDEBUG")

set(CMAKE_CXX_STANDARD 14)

install(TARGETS practicalcoursedatabaseimplementation RUNTIME DESTINATION bin)
