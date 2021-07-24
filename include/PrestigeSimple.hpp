#ifndef PANGOLIN_PRESTIGESIMPLE_HPP
#define PANGOLIN_PRESTIGESIMPLE_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <algorithm>
#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include "Types.hpp"
/* 
* Simple Prestige: the sum of the degree of an actor's alters
* In Prestige: the sum of the degree of in direction alters (actor is .to)
* Out Prestige: the sum of the degree of out direction alters (actor is .from)
* Eigenvalue centrality: also prestige but more complicated
*/
/* 
 * Performance Note:
 * at 1'000'000 edges x 1'000 iterations
 * vector for loop, no execution policies : 1'482'101.986657 ms = 24 minutes 42.101988 seconds
 * for_each, par          unseq           : 1'415'465.532934 ms = 23 minutes 35.465532 seconds 
 * for_each, par_unseq    unseq           : 1'402'769.541336 ms = 23 minutes 22.769544 seconds
 * for_each, par_unseq, unseq, and reduce : 1'370'127.705629 ms = 22 minutes 50.1277056 seconds
*/
/*
 * Simple Prestige
 * for each node,
 *    the node's prestige = sum of it's alters' degree centrality
 */


std::unordered_map<Node, float> PrestigeSimple(std::unordered_map<Node, Nodelist> node_alters, std::unordered_map<Node, float> degree_map) {
    std::unordered_map<Node, float>   prestige_map;
    float                             raw_prestige    (0);
    std::vector<float>                previous_values;
    float max_value (0);
    // Init Map
    for(const auto& [node, degree] : degree_map) { prestige_map[node] = {}; }
    // Calculate Simple Prestige

    std::for_each(std::execution::unseq, RANGE(node_alters), [&degree_map, &prestige_map, &raw_prestige, &max_value](const auto& node_alter) {
        const Node&                     node            (node_alter.first);
        const Nodelist&                 alters          (node_alter.second);
        std::vector<float>              alter_degree    {};
        std::for_each(std::execution::unseq, RANGE(alters), [&degree_map, &alter_degree](const auto& alter){
            alter_degree.emplace_back(degree_map[alter]); });
        prestige_map[node] = (std::reduce(std::execution::unseq, RANGE(alter_degree))); });

    for(const auto [key, value] : prestige_map) { max_value = (max_value > value) ? max_value : value; }

    std::for_each(std::execution::par_unseq, RANGE(prestige_map), [&max_value, &prestige_map](auto prestige_pair){
        prestige_map[prestige_pair.first] = prestige_pair.second / max_value; });

    return prestige_map; }

template<typename number>
std::unordered_map<Node, float> Prestige(std::unordered_map<Node, Nodelist> node_alters, std::unordered_map<Node, number> degree_map) {
    std::uint64_t iterations;
    iterations = node_alters.size() - 1;
    std::unordered_map<Node, float> prestige_map;
    for(const auto& [node, degree] : degree_map) {
        prestige_map[node] = static_cast<float>(degree); }

    if(iterations > 10) { iterations = 10; }

    prestige_map = PrestigeSimple(node_alters, prestige_map);
    if(iterations > 0) {
        for(int i = 0; i < iterations; ++i) {
            prestige_map = PrestigeSimple(node_alters, prestige_map); }}
    return prestige_map; }

#endif //PANGOLIN_PRESTIGESIMPLE_HPP