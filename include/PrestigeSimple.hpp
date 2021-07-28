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
#include "GraphProperties.hpp"

namespace {

using Prestige_map  =   std::unordered_map<Node, float>;
using Alter_map     =   std::unordered_map<Node, Nodelist>;


template<typename Number_type>
Prestige_map NormalizeEigen(Prestige_map prestige_map) {
    /*
    * Purpose:  Normalize the prestige values of the entire map.  Create a vector of prestige values.  Find the greatest value.  Divide each value by that greatest value.
    * Variables:
    * TODO:
    */
    std::vector<Number_type>                prestige_values         {};
    std::unordered_map<Node, Number_type>   prestige_normalized     {};
    Number_type                             prestige_max            (1);
    // Normalize the eigencentrality
    std::for_each(std::execution::unseq, RANGE(prestige_map), [&prestige_values](const auto key_value) {
        prestige_values.emplace_back(key_value.second); });
    prestige_max  = FindMaxValue<decltype(prestige_values), Number_type>(prestige_values);
    std::for_each(std::execution::unseq, RANGE(prestige_map), [&prestige_max, &prestige_normalized](const auto &key_value){
        prestige_normalized[key_value.first] = key_value.second / prestige_max; });
    return prestige_normalized; }


template<typename prestige_value>
prestige_value NodePrestigeCalculate(std::unordered_map<Node, prestige_value> add_map, const Nodelist& node_alters) {
    /*
    * Purpose: Create a vector of degree/prestige values.  Return the sum of those values.
    * Variables:
    * TODO:
    */
    std::vector<prestige_value> alter_degree {};
    // find the degree of each alter
    std::for_each(std::execution::unseq, cRANGE(node_alters), [&alter_degree, &add_map](auto& alter){
            alter_degree.emplace_back(add_map.at(alter)); });
    return std::reduce(std::execution::par_unseq, cRANGE(alter_degree)); }

template<typename prestige_value>
Prestige_map PrestigeCalculate(std::unordered_map<Node, prestige_value> add_map, Prestige_map prestige_map, const Alter_map& node_alters, const Nodelist& nodes)  {
    /*
    * Purpose: Go through all the nodes.  Calculate the prestige value for each node.
    * Variables:
    * TODO:
    */
    std::for_each(std::execution::par_unseq, cRANGE(nodes), [&add_map, &prestige_map, &node_alters](const auto node){
        prestige_map[node] = NodePrestigeCalculate<float>(add_map, node_alters.at(node));  });

    return prestige_map; }


template<typename Degree_map, typename Iter_num>
std::unordered_map<Node, float> Prestige(const Graph& g, const Alter_map& node_alters, const Degree_map& degree_map, Iter_num iter_num) {
    /*
    * Purpose:
    * Variables:
    * TODO:
    */
    const Nodelist                  nodes               (g.nodes);
    std::size_t                     iterations          (iter_num);
    Prestige_map                    prestige_map;

    // Change the copy integer degree values into float values
    std::for_each(std::execution::par_unseq, cRANGE(nodes), [&prestige_map, &degree_map](const auto& node){
        prestige_map[node] = static_cast<float>(degree_map.at(node)); });

    // Normalize the float values
    prestige_map = NormalizeEigen<float>(prestige_map);

    if(iterations > 1000) { iterations = 1000; }
    // go for n iterations
    for(std::size_t i = 0; i != (iterations - 1); ++i) {
            // For each node, get the alters, get the prestige of the alters, add the prestige together
            prestige_map = NormalizeEigen<float>(PrestigeCalculate<float>(prestige_map, prestige_map, node_alters, nodes)); }
    return prestige_map; }

}; // End Anonymous namespace
#endif //PANGOLIN_PRESTIGESIMPLE_HPP