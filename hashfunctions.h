#pragma once
#include <tuple>
#include "Types.h"
// https://stackoverflow.com/questions/7110301/generic-hash-for-tuples-in-unordered-map-unordered-set
namespace std {
namespace {

// Code from boost
// Reciprocal of the golden ratio helps spread entropy
//     and handles duplicates.
// See Mike Seymour in magic-numbers-in-boosthash-combine:
//     http://stackoverflow.com/questions/4948780

template <class T>
inline void hash_combine(std::size_t& seed, T const& v) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// Recursive template code derived from Matthieu M.
template < class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1 >
struct HashValueImpl {
    static void apply(size_t& seed, Tuple const& tuple) {
        HashValueImpl < Tuple, Index - 1 >::apply(seed, tuple);
        hash_combine(seed, std::get<Index>(tuple));
    }
};

template <class Tuple>
struct HashValueImpl<Tuple, 0> {
    static void apply(size_t& seed, Tuple const& tuple) {
        hash_combine(seed, std::get<0>(tuple));
    }
};
}

template <typename ... TT>
struct hash<std::tuple<TT...>> {
    size_t
    operator()(std::tuple<TT...> const& tt) const {
        size_t seed = 0;
        HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
        return seed;
    }
};

template<>
struct hash<Integer> {
    size_t
    operator()(const Integer& tt) const {
        return tt.hash();
    }
};

template<unsigned L>
struct hash<Char<L>> {
    size_t
    operator()(const Char<L>& tt) const {
        return tt.hash();
    }
};
}
