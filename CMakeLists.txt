cmake_minimum_required(VERSION 2.6)
project(practicalcoursedatabaseimplementation)

add_executable(practicalcoursedatabaseimplementation main.cpp Types.cpp neworderrandom.cpp newOrder.cpp loadTbls.cpp Database.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -fshow-column -pipe -march=native -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -Og -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -DNDEBUG")

set(CMAKE_CXX_STANDARD 14)

install(TARGETS practicalcoursedatabaseimplementation RUNTIME DESTINATION bin)
