//  TODO: use PMR to speed up matrix creation
//  TODO: use binary vs valued depending upon matrix size
//  The adjencency list gives the position of each value in the incidence matrix.
//  The incidence matrix is an n X e array.
#ifndef PANGOLIN_INCIDENCEMATRIX_HPP
#define PANGOLIN_INCIDENCEMATRIX_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <vector>
#include <map>
// #include <iostream>
#include "GenEdgemap.hpp"
#include "Types.hpp"

std::vector<std::vector<uint>> IncidenceMatrix(const Graph& g, const Edgemap& edgemap) {
    const Nodelist& nodes   (g.nodes);
    const Edgelist& edges   (g.edges);
    const auto&     n_size  (nodes.size());
    const auto&     e_size  (edges.size());
    std::vector<std::vector<uint>> incidence_matrix;
    const Edgemap& node_edges (GenEdgemap(g));
    // INIT MATRIX WITH 0 VALUES
    for (uint i = 0; i < n_size; ++i) {
        std::vector<uint> row_vector;
        for (uint j = 0; j < e_size; ++j) { row_vector.emplace_back(0); }
        incidence_matrix.emplace_back(std::move(row_vector)); }
    // INSERT 1 IN ADJACENCY MATRIX
    for (const auto& [i, js] : node_edges) {
//        std::cout << "i: " << i;
        for(const auto& j : js) {
//            std::cout << "\tj: " << j;
            incidence_matrix[i - 1][j - 1] = 1; }
        //std::cout << "\n"; 
        }
    return incidence_matrix; }

#endif //PANGOLIN_INCIDENCEMATRIX_HPP