/*****************************  Eigencentrality v 2 *****************************/
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

namespace {

using Prestige_map  =   std::unordered_map<Node, float>;
using Alter_map     =   std::unordered_map<Node, Nodelist>;

template<typename Add_map>
Prestige_map PrestigeCalculate(Add_map add_map, Prestige_map prestige_map, const Alter_map& node_alters, const Nodelist& nodes)  {
    std::for_each(std::execution::par_unseq, cRANGE(nodes), [&add_map, &prestige_map, &node_alters](const auto node){
        // find the alters of the node
        const auto&                 all_alters          (node_alters.at(node));
        std::vector<Integer>        alter_degree;
        // find the degree of each alter
        for(auto alter : all_alters) {
            alter_degree.emplace_back(add_map.at(alter)); }
        prestige_map[node] = std::reduce(std::execution::par_unseq, RANGE(alter_degree)); } );
    return prestige_map; }


template<typename Prestige_varied>
Prestige_map NormalizeEigen(Prestige_varied prestige_map) {
    std::unordered_map<Node, float> prestige_normalized;
    // Normalize the eigencentrality
    for(const auto [key, value] : prestige_map) {
        prestige_values.emplace_back(value); }

    prestige_max  = prestige_values.at(std::distance(prestige_values.begin(), std::max_element(std::execution::par_unseq, RANGE(prestige_values))));

    for(const auto [key, value] : prestige_map) {
        prestige_normalized[key] = value / prestige_max; }

    std::cout << prestige_max << "\n";

    return prestige_normalized; }

template<typename Degree_map, typename Iter_num>
std::unordered_map<Node, float> Prestige(const Graph& g, const Alter_map& node_alters, const Degree_map& degree_map, Iter_num iter_num) {
    // initialize a degree map with floats
    const Nodelist                  nodes               (g.nodes);
    std::vector<Integer>            prestige_values;
    float                           prestige_max;
    std::size_t                     iterations          (iter_num);
    Prestige_map                    degree_map_float;
    Prestige_map                    prestige_map;
    // sif(iter_num > 1000) { iterations = 1000; }

    // Initialize
    std::for_each(std::execution::unseq, cRANGE(nodes), [&prestige_map](const auto node) {
        prestige_map[node] = 0; });

    // go for n iterations
    for(std::size_t i = 0; i != (iterations - 1); ++i) {
        if(i == 0) {
            // For each node, get the alters, get the degree of the alters, add the degree together
            prestige_map = NormalizeEigen(PrestigeCalculate<Degree_map>(degree_map, prestige_map, node_alters, nodes));
        } else {
            // For each node, get the alters, get the prestige of the alters, add the prestige together
            prestige_map = NormalizeEigen(PrestigeCalculate<Prestige_map>(prestige_map, prestige_map, node_alters, nodes));
        }
    }
    return prestige_map; } }
#endif //PANGOLIN_PRESTIGESIMPLE_HPP