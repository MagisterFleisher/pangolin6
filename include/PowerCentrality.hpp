#ifndef PANGOLIN_POWER_CENTRALITY_HPP
#define PANGOLIN_POWER_CENTRALITY_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <algorithm>
#include <execution>
#include <iostream>
#include <unordered_map>
#include "Types.hpp"
#include "DegreeCentrality.hpp"

std::unordered_map<Node, Integer> Eigencentrality(const Graph& g, const std::unordered_map<Node, Integer> node_degree, const int iterations) {
    std::unordered_map<Node, Integer> centrality_map;
    
    std::for_each(std::execution::unseq, cRANGE(g.nodes), [&freq_map](const auto& node){ freq_map[node] = 0; });
    switch(direction) {
        case in:    {
            std::for_each(std::execution::par, cRANGE(g.edges), [&freq_map](const auto& edge){ freq_map[edge.to] += 1; });
            break; }
        case out:   {
            std::for_each(std::execution::par, cRANGE(g.edges), [&freq_map](const auto& edge){ freq_map[edge.from] += 1; });
            break; }
        case both:  {
            std::for_each(std::execution::par, cRANGE(g.edges), [&freq_map](const auto& edge){ freq_map[edge.to] += 1;   });
            std::for_each(std::execution::par, cRANGE(g.edges), [&freq_map](const auto& edge){ freq_map[edge.from] += 1; });
            break; }
        default:    {
            std::cerr << "Degree: Error, no valid direction for switch"; 
            break; } }
    return freq_map; }
#endif //PANGOLIN_DEGREECENTRALITY_HPP