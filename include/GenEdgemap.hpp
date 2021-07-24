#ifndef PANGOLIN_GENEDGEMAP_HPP
#define PANGOLIN_GENEDGEMAP_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <vector>
#include <map>
// #include <iostream>
#include "Types.hpp"

Edgemap GenEdgemap(const Graph& g) {
    Edgemap node_edge_map;
    const auto& edges (g.edges);
    const auto& nodes (g.nodes);
    // Initialize map
    for(const auto& node : nodes) {
        node_edge_map[node] = {}; }
    // Create the edgemap
    for(const auto& edge : edges) {
        std::vector<ID> temp_edgelist_from (node_edge_map.at(edge.from));
        temp_edgelist_from.emplace_back(edge.id);
        node_edge_map[edge.from] = temp_edgelist_from;
        std::vector<ID> temp_edgelist_to (node_edge_map.at(edge.to));
        temp_edgelist_to.emplace_back(edge.id);
        node_edge_map[edge.to] = temp_edgelist_to; }
    return node_edge_map; }

#endif //PANGOLIN_GENEDGEMAP_HPP