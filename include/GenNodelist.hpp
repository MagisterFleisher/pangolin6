#ifndef PANGOLIN_NODELISTGEN_HPP
#define PANGOLIN_NODELISTGEN_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <algorithm>
#include <execution>
#include <iostream>
#include "Types.hpp"
/*
 * std::sort(std::execution::unseq
 * std::sort(std::execution::par */
Nodelist SquishNodelist(Nodelist& nodes) { //       std::cout << "\tCalled SquishNodelist\n";
    std::sort(std::execution::unseq, RANGE(nodes));
    nodes.erase(std::unique(RANGE(nodes)), nodes.end());
    nodes.shrink_to_fit();
    return nodes; }
/*
 * for_each: std::execution::unseq 244132.132111ms
 * for_eaach: Error
 * for(const auto& edge : edges)   243936.129677ms
 * No move:                        245329.113241ms
 * 2 moves:                        243600.323133ms */
Nodelist GenNodelist(const Edgelist& edges) { //    std::cout << "Called NodelistGen: Edge count "  << edges.size() << "\n";
    Nodelist nodes;
    for(const auto& edge : edges) { nodes.emplace_back(edge.from); }
    nodes = SquishNodelist(nodes);
    for(const auto& edge : edges) { nodes.emplace_back(edge.to); }
    nodes = SquishNodelist(nodes);
    return nodes; }
#endif//PANGOLIN_NODELISTGEN_HPP