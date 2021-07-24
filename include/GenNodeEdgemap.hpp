#ifndef PANGOLIN_GENEDGEMAP_HPP
#define PANGOLIN_GENEDGEMAP_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include "Types.hpp"
/*
NodeEdgemap GenNodeEdgemap(const Graph& g) {
    NodeEdgemap node_edge_map;
    const auto& edges (g.edges);
    const auto& nodes (g.nodes);
    // Initialize map
    for(const auto& node : nodes) {
        node_edge_map[node] = {}; }
    // Create the edgemap
    for(const auto& edge : edges) {
        std::vector<ID> temp_edgelist_from;
        temp_edgelist_from.emplace_back(edge.id);
        node_edge_map[edge.from] = temp_edgelist_from;

        std::vector<ID> temp_edgelist_to;
        temp_edgelist_to.emplace_back(edge.id);
        node_edge_map[edge.to] = temp_edgelist_to; }
    return node_edge_map; }
*/
#endif //PANGOLIN_GENEDGEMAP_HPP