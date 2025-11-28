#pragma once
#include <tuple>
#include <utility>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

// ---------------------------------------------------------
//  Generic hash combine (similar to boost::hash_combine)
// ---------------------------------------------------------
inline void hash_combine(std::size_t& seed, std::size_t value) {
    seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

// ---------------------------------------------------------
//  Hash for std::pair
// ---------------------------------------------------------
template<typename T1, typename T2>
struct PairHash {
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        std::size_t seed = 0;
        hash_combine(seed, std::hash<T1>()(p.first));
        hash_combine(seed, std::hash<T2>()(p.second));
        return seed;
    }
};

// ---------------------------------------------------------
//  Hash for std::tuple (generic for any size)
// ---------------------------------------------------------
template<typename Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
struct TupleHashImpl {
    static void apply(std::size_t& seed, const Tuple& tuple) {
        TupleHashImpl<Tuple, Index - 1>::apply(seed, tuple);
        hash_combine(seed, std::hash<std::tuple_element_t<Index, Tuple>>()(
            std::get<Index>(tuple)));
    }
};

template<typename Tuple>
struct TupleHashImpl<Tuple, 0> {
    static void apply(std::size_t& seed, const Tuple& tuple) {
        hash_combine(seed,
            std::hash<std::tuple_element_t<0, Tuple>>()(
                std::get<0>(tuple)
                )
        );
    }
};

template<typename... Args>
struct TupleHash {
    std::size_t operator()(const std::tuple<Args...>& tpl) const {
        std::size_t seed = 0;
        TupleHashImpl<std::tuple<Args...>>::apply(seed, tpl);
        return seed;
    }
};

template <typename Map, typename Key>
void incrementCounter(
    Map* map,
    const Key& key,
    const typename Map::mapped_type& amount = typename Map::mapped_type(1))
{
    static_assert(std::is_arithmetic_v<typename Map::mapped_type>,
        "incrementCounter requires numeric mapped_type");

    if (!map) return;

    auto it = map->find(key);
    if (it != map->end()) {
        it->second += amount;
    }
    else {
        map->emplace(key, amount);
    }
}

template<typename OuterMap, typename Key, typename InnerKey, typename Value>
void addToNestedMap(
    OuterMap& outerMap,
    const Key& compositeKey,
    const InnerKey& innerKey,
    const Value& amount = Value(1))
{
    using InnerMapPtr = typename OuterMap::mapped_type;
    using InnerMap = std::remove_pointer_t<InnerMapPtr>;

    static_assert(std::is_arithmetic_v<Value>,
        "Value must be numeric for nested inner maps");

    InnerMap* innerMap = nullptr;

    auto it = outerMap.find(compositeKey);
    if (it == outerMap.end()) {
        // Create new inner map
        auto newInnerMap = std::make_unique<InnerMap>();
        innerMap = newInnerMap.get();

        // Insert pointer and release ownership
        outerMap.emplace(compositeKey, newInnerMap.release());
    }
    else {
        innerMap = it->second;
    }

    // Increment inside the inner numeric map
    incrementCounter(innerMap, innerKey, amount);
}


template<typename Map>
auto sortMapByValue(const Map& m, const bool desc = true) {
    using K = typename Map::key_type;
    using V = typename Map::mapped_type;

    std::vector<std::pair<K, V>> vec(m.begin(), m.end());
    std::sort(vec.begin(), vec.end(),
        [desc](auto& a, auto& b) {
			return desc ? a.second > b.second :
                a.second < b.second;
        });
    return vec;
}

template<typename PairVec>
void printStringDoublePairVector(const PairVec& vec, const bool rev = false)
{
    for (const auto& [str, val] : vec) {
		auto displayVal = rev ? ": $"  : ": ";
        std::cout<<"\t" << str << displayVal << val << "\n";
    }
    std::cout << std::endl;
}