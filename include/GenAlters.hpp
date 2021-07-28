#ifndef PANGOLIN_ALTERMAP_HPP
#define PANGOLIN_ALTERMAP_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <execution>
#include <iostream>
#include <map>
#include "Types.hpp"

std::unordered_map<Node, Nodelist> Gen_NodeAlters(const Graph& g) {
    std::unordered_map<Node, Nodelist> altermap;
    // Init data structure
    for(const auto& node : g.nodes) { altermap[node] = {}; }
    std::for_each(std::execution::unseq, cRANGE(g.edges),  // Is this parallelized?
        [&altermap](const auto& edge) {
            altermap[edge.from].emplace_back(edge.to);
            altermap[edge.to].emplace_back(edge.from); });
    // Get rid of repeated alters
    std::for_each(std::execution::par, cRANGE(altermap),  // Is this parallelized?
        [&altermap](auto node_alter) {
            altermap[node_alter.first] = std::move(UniqueVector<Nodelist>(node_alter.second)); });
    return altermap; }

#endif // PANGOLIN_ALTERMAP_HPP