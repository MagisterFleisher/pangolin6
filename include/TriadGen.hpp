#ifndef PANGOLIN_TRIADGEN_HPP
#define PANGOLIN_TRIADGEN_HPP
#pragma GCC optimize("Ofast")
#include <execution>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>
#include "GenCliques.hpp"
#include "Degree.hpp"
#include "Types.hpp"
/*
 * TRIAD GEN ALGORITHM
 * find Cliques
 *   Triads are minimal cliques and cliques divided into triples
 */
std::set<std::set<Node>> TriadGen(const Altermap& alter_map) {
    std::set<std::set<Node>> blank;
    return blank; }
#endif // PANGOLIN_TRIADGEN_HPP

    /*std::set<std::set<Node>>        triad_set;
    const auto&                     clique_set  (CliqueFind(alter_hash));
    const std::vector<std::set<Node>> cliques   (clique_set.begin(), clique_set.end());
    for(const auto& clique : cliques) {
        std::vector<Node>           vec_clique  (clique.begin(), clique.end());
        Node                        node        (vec_clique.at(0));
        Node                        alter       (vec_clique.at(1));
        std::vector<Node>           triad_nodes (vec_clique.at(2), vec_clique.end());
        for(const auto& node : triad_nodes) {
            std::set<Node>          triad       {node, alter, triad_node};
            triad_set.emplace(triad); }}
    return triad_set; */