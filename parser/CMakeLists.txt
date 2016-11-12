project(practicalcoursedatabaseimplementation)
add_executable(query query.cpp Parser.cpp Schema.cpp query/TableScan.cpp query/HashJoin.cpp query/Selection.cpp query/Printer.cpp query/InformationUnit.h)
add_executable(parser parser.cpp Parser.cpp Schema.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -fshow-column -pipe -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -O0 -DDEBUG -fsanitize=undefined -fsanitize=address -fsanitize=leak")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -DNDEBUG")
