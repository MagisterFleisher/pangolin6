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

/*****************************  Eigencentrality 2 *****************************/

template<typename degree_number, typename iter_num>
std::unordered_map<Node, float> Prestige(const Graph& g, const std::unordered_map<Node, Nodelist>& node_alters, const std::unordered_map<Node, degree_number>& degree_map, iter_num iterations) {
    // initialize a degree map with floats
    const Nodelist nodes (g.nodes);
    std::vector<float> prestige_values;
    float prestige_max;
    std::unordered_map<Node, float> degree_map_float;
    std::unordered_map<Node, float> prestige_map;
    std::unordered_map<Node, float> prestige_normalized;

    if(iterations > 1000) { iterations = 1000; }

    // Initialize
    std::for_each(std::execution::par, cRANGE(nodes), [&degree_map, &degree_map_float](const auto node) {
        degree_map_float[node] = static_cast<float>(degree_map.at(node)); });
    // Initialize
    std::for_each(std::execution::unseq, cRANGE(nodes), [&prestige_map](const auto node) {
        prestige_map[node] = 0; });
    // Initialize
    std::for_each(std::execution::unseq, cRANGE(nodes), [&prestige_normalized](const auto node){
        prestige_normalized[node] = 0; });

    // go for n iterations
    for(std::size_t i = 0; i != iterations; ++i) {
        // TODO: FIX ME, this does not work recursively, which it should
        // For each node, get the alters, get the degree of the alters, add the degree together
        std::for_each(std::execution::unseq, cRANGE(nodes), [&degree_map, &prestige_map, &node_alters](const auto node){
            // find the alters of the node
            const auto& all_alters (node_alters.at(node));

            std::vector<degree_number> alter_degree;
            // find the degree of each alter
            for(auto alter : all_alters) {
                alter_degree.emplace_back(degree_map.at(alter)); }

            prestige_map[node] = std::reduce(std::execution::par_unseq, RANGE(alter_degree)); }); }

    // Normalize the eigencentrality

    for(const auto [key, value] : prestige_map) {
        prestige_values.emplace_back(value);  }

    prestige_max  = prestige_values.at(std::distance(prestige_values.begin(), std::max_element(RANGE(prestige_values))));
    
    for(const auto [key, value] : prestige_map) {
        prestige_normalized[key] = value / prestige_max; }
    
    return prestige_normalized; }

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


    //Which is faster, loop through map or loop through nodes
    //std::for_each(std::execution::par_unseq, RANGE(nodes), [&prestige_values, &prestige_normalized, &prestige_max](const auto node){
    //    prestige_normalized[node] = (prestige_values.at(node) / prestige_max); 
    //});
/*
    std::for_each(std::execution::par_unseq, cRANGE(nodes), [&prestige_map, &prestige_normalized, &prestige_max](const auto node){
        float node_prestige (prestige_map.at(node));
        prestige_normalized[node] = (node_prestige / prestige_max); }); */
    // Return the eigencentrality





/*
std::unordered_map<Node, float> PrestigeSimple(std::unordered_map<Node, Nodelist> node_alters, std::unordered_map<Node, float> degree_map) {
    std::unordered_map<Node, float> prestige_map;
    float max_value (0);
    // Init Map
    for(const auto& [node, degree] : degree_map) { prestige_map[node] = {}; }
    // Calculate Simple Prestige

    std::for_each(std::execution::par_unseq, RANGE(node_alters), [&degree_map, &prestige_map](const auto& node_alter) {    
        const Node&                     node            (node_alter.first);
        const Nodelist&                 alters          (node_alter.second);
        std::vector<float>              alter_degree    {};

        std::for_each(std::execution::unseq, RANGE(alters), [&degree_map, &alter_degree](const auto& alter){
            alter_degree.emplace_back(degree_map[alter]); });
        
        prestige_map[node] = (std::reduce(std::execution::par_unseq, RANGE(alter_degree)));  });

    // Find Max Prestige for Normalization
    for(const auto [key, value] : prestige_map) { max_value = (max_value > value) ? max_value : value; }

    // Normalize
    std::for_each(std::execution::unseq, RANGE(prestige_map), [&max_value, &prestige_map](auto prestige_pair){
        prestige_map[prestige_pair.first] = prestige_pair.second / max_value; });

    return prestige_map; }

template<typename number>
std::unordered_map<Node, float> Prestige(std::unordered_map<Node, Nodelist> node_alters, std::unordered_map<Node, number> degree_map) {
    std::size_t iterations (node_alters.size() - 1);

    std::unordered_map<Node, float> prestige_map;

    std::for_each(std::execution::par_unseq, RANGE(degree_map), [&prestige_map](const auto& node_degree){
        auto    node            (node_degree.first);
        float   degree          (static_cast<float>(node_degree.second));
        prestige_map[node] = degree; });

    if(iterations > 10) { iterations = 10; }

    prestige_map = PrestigeSimple(node_alters, prestige_map);

    if(iterations > 0) {
        for(std::size_t i = 0; i != iterations; ++i) {
            prestige_map = PrestigeSimple(node_alters, prestige_map); 
    } }

    return prestige_map; } */

/*
std::unordered_map<Node, float> PrestigeSimple_old(std::unordered_map<Node, Nodelist> node_alters, std::unordered_map<Node, float> degree_map) {
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
std::unordered_map<Node, float> Prestige_old(std::unordered_map<Node, Nodelist> node_alters, std::unordered_map<Node, number> degree_map) {
    std::size_t iterations (node_alters.size() - 1);
    std::unordered_map<Node, float> prestige_map;
    for(const auto& [node, degree] : degree_map) {
        prestige_map[node] = static_cast<float>(degree); }

    if(iterations > 10) { iterations = 10; }

    prestige_map = PrestigeSimple(node_alters, prestige_map);
    if(iterations > 0) {
        for(std::size_t i = 0; i != iterations; ++i) {
            prestige_map = PrestigeSimple(node_alters, prestige_map); }}
    return prestige_map; }
*/



#endif //PANGOLIN_PRESTIGESIMPLE_HPP