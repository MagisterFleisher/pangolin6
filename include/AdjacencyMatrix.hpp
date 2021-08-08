//  TODO: use PMR to speed up matrix creation
//  TODO: use binary vs valued depending upon matrix size
//  The adjencency list gives the position of each value in the incidence matrix.
//  The incidence matrix is an n X n array.
#ifndef PANGOLIN_ADJACENCYMATRIX_HPP
#define PANGOLIN_ADJACENCYMATRIX_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <iostream>
#include <vector>
#include "Types.hpp"


std::vector<std::vector<Node>> AdjacencyMatrix(const Graph &g, const std::unordered_map<Node, Nodelist>& altermap) {
    const Nodelist&                         nodes               (g.nodes);
    const std::size_t                       dim                 (nodes.size());
    std::vector<Node>                       init_row            (dim);
    std::vector<std::vector<Node>>          adjacency_matrix;
    std::cout << "Test AdjacencyMatrix function\n";

    std::generate(RANGE(init_row), [](){ return 0; });
    for(std::size_t i = 0; i != dim; ++i) {
        adjacency_matrix.emplace_back(init_row); }

/*
    for(auto [node, alters] : altermap) {
        for(auto alter : alters) {
            adjacency_matrix[node - 1][alter - 1] = 1; } }

    for(auto row : adjacency_matrix) {
        for(auto i : row) {
            // for(auto i : col) {
                std::cout << i << " "; }
                std::cout << "\n"; }// }
*/
    return adjacency_matrix; }

/*
template<typename adj_mat>
adj_mat Advanced_AdjacencyMatrix(const Graph &g, const Altermap& altermap) {

    std::array<std::size_t, dim>    range_counter;
    std::array<std::array<std::size_t, dim>, dim> adjacency_matrix;

    std::iota(RANGE(range_counter), 0);

    std::for_each(std::execution::par_unseq, RANGE(range_counter),
        [&adjacency_matrix](auto i){});


    for(std::size_t i = 0; i < dim; ++i) {
        std::vector<uint> row_vector;
        for (uint j = 0; j < dim; ++j) { row_vector.emplace_back(0); }
        adjacency_matrix.emplace_back(std::move(row_vector)); }
    // INSERT 1 IN INCIDENCE MATRIX
    for(const auto& node_id : altermap) {
        const auto i (node_id.first);
        for(const auto& j : node_id.second) { adjacency_matrix[i - 1][j - 1] = 1; } }
    return adjacency_matrix; } */

#endif //PANGOLIN_INCIDENCEMATRIX_HPP