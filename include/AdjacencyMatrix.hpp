//  TODO: use PMR to speed up matrix creation
//  TODO: use binary vs valued depending upon matrix size
//  The adjencency list gives the position of each value in the incidence matrix.
//  The incidence matrix is an n X n array.
#ifndef PANGOLIN_ADJACENCYMATRIX_HPP
#define PANGOLIN_ADJACENCYMATRIX_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <vector>
#include "Types.hpp"

std::vector<std::vector<uint>> AdjacencyMatrix(const Graph &g, const Altermap& altermap) {
    const Nodelist& nodes   (g.nodes);
    const auto&     dim     (nodes.size());
    std::vector<std::vector<uint>> adjacency_matrix;
    // INIT MATRIX WITH 0 VALUES
    for (uint i = 0; i < dim; ++i) {
        std::vector<uint> row_vector;
        for (uint j = 0; j < dim; ++j) { row_vector.emplace_back(0); }
        adjacency_matrix.emplace_back(std::move(row_vector)); }
    // INSERT 1 IN INCIDENCE MATRIX
    for(const auto& node_id : altermap) {
        const auto i (node_id.first);
        for(const auto& j : node_id.second) { adjacency_matrix[i - 1][j - 1] = 1; } }
    return adjacency_matrix; }

#endif //PANGOLIN_INCIDENCEMATRIX_HPP