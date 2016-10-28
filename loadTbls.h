#ifndef LOADTBLS_H
#define LOADTBLS_H
#include "Types.h"
#include "hashfunctions.h"
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <unordered_map>
#include <tuple>
#include "Tables.h"

template<typename T>
T readLineFromFile(std::ifstream& fileName);

template<typename T>
std::vector<T> readTableFromFile(std::string fileName);

template<typename T>
std::vector<T> readTableFromFile(std::string name) {
    auto table = std::vector<T> {};
    auto fileStream = std::ifstream {};
    fileStream.open(name);
    if (!fileStream.is_open()) {
        throw "no file found";
    }
    for (std::string line; std::getline(fileStream, line, '\n');) {
        table.push_back(T::read(line));
    }
    fileStream.close();
    return table;
}

#endif // LOADTBLS_H
