#ifndef PANGOLIN_DISCONNECTEDSUBGRAPHS_HPP
#define PANGOLIN_DISCONNECTEDSUBGRAPHS_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <vector>
#include <tuple>
#include <map>
#include <execution>
#include <iostream>
#include <string_view>
#include "GenNodelist.hpp"
#include "Types.hpp"

// Find singleton nodes
///  Get the BothDegree from the Node Table
///  Look for nodes with degree 0
///  Write each node as subgraph in the subgraph list
///  Remove nodes from Alterlist or Edgelist (whichever used)


namespace { // Anonymous namespace to keep precompile global definitions local to these functions
#define VISITED true;
#define NOT_VISITED false;

// std::pair<std::map<Nodelist, uint>, Nodelist>

std::map<bool, std::string> VisitStrings() {
    std::map<bool, std::string>    visit_map;
    std::string                    visited_string {"visited"};
    std::string                    unvisited_string {"not visited"};
    visit_map[true]     =   visited_string;
    visit_map[false]    =   unvisited_string;
    return visit_map; }

// Find the next level of nodes

std::map<Node, bool> InitVisitMap(const Altermap& altermap) {
    std::map<Node, bool> visit_map;
    std::for_each(RANGE(altermap), [&visit_map](const auto& node_alters) {
        visit_map[node_alters.first] = NOT_VISITED; });
    return visit_map; }

Nodelist FindBranches(const Nodelist& roots, const std::map<Node, bool> visit_map, const Altermap& altermap) {
    Nodelist branches {};
    std::map<bool, std::string> visit_strings (VisitStrings());
    for(const auto& [node, visit_status] : visit_map) {
        std::cout << "Node: " << node << " Visited: " << visit_strings[visit_status] << "\n"; }

    std::for_each(RANGE(roots), [&branches, &altermap, &visit_map](const auto& root){
        const auto& alters (altermap.at(root));
        for(const auto& branch : alters) { branches.emplace_back(branch); } });

    return branches; }

Nodelist ExploreGraph(const Altermap& altermap, Nodelist roots) {
    std::map<Node, bool> visit_map (InitVisitMap(altermap));
    bool finished (false);
    uint x = 0;
    Nodelist graph_nodes {};
    do{
        ++x;
        std::cout << "Iteration #: " << x << "\n";
        const auto status_quo (visit_map);
        std::for_each(RANGE(roots), [&visit_map](const auto& node) { visit_map[node] = VISITED; });
        const Nodelist& branchlist (FindBranches(roots, visit_map, altermap));
        if(visit_map == status_quo) {
            finished = true;
            graph_nodes = roots; } else {
                for(const auto& branch : branchlist) {
                    roots.emplace_back(branch); } }
    } while(!finished);
    return graph_nodes; }


std::vector<Graph> DisconnectedSubgraphs(const Graph& g, const Altermap& altermap, const Edgemap& edgemap) {
    std::vector<Graph>  disconnected_subgraphs;
    const Nodelist&     start_node                  {(g.nodes.front())};

    const Nodelist&     subgraph_nodes              (ExploreGraph(altermap, start_node));
    // const Graph&        subgraph                    (GraphFromNodes(subgraph_nodes, edgemap, g));
    //disconnected_subgraphs.emplace_back(subgraph);

    return disconnected_subgraphs; }
}
#endif // PANGOLIN_DISCONNECTEDSUBGRAPHS_HPP