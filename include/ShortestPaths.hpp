#ifndef PANGOLIN_SHORTESTPATHS_HPP
#define PANGOLIN_SHORTESTPATHS_HPP
#pragma once
// #include <folly/FBVector.h>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <tuple>
#include <algorithm>
#include "Types.hpp"
/*
from each node to every other node
n! paths
path length 1 is trivial  (alter hash)
path length 2 also trivial (alter hash of alterhash - nodes in first alterhash)
path length 3, path length 2 plus 1 (may have multiple shortest paths)
path length 4+, nontrivial
Dykstra's algorithm - should be parallelizable
Data: { set of 2 nodes : shortest path(s) between nodes }
shortest paths: unordered_map { set<Node> {N1, N2}, set<vector<Node>> {path} }
does the pair have a shortest path: { set<Node> {N1, N2}, path_length}
    if path_length < current path checking, skip
---
betweenness: for each node, sum of occurances in a shortest path set
- should be parallelizable */

std::map<ID, int64_t> InitNodes(Nodelist nodes) {
    std::map<ID, int64_t> nodemap;
    for(const auto& node : nodes) { nodemap.at(node) = 0; }
    return nodemap; }


std::uint8_t ShortestPaths(const Graph& g, const Altermap& node_alters) {
    const Nodelist  nodes      (g.nodes);
    std::set<ID> used_nodes;
    std::map<ID, uint> subgraphID;
    std::map<std::pair<ID, ID>, uint> distance_map{};
    uint            distance   (0);

    // start values
    for(const auto& node : nodes) {

        ++distance;
        used_nodes.emplace(node);
        const auto& alters (node_alters.at(node));
        for(const auto& alter : alters) {
            if(used_nodes.find(alter) == used_nodes.end()) {
                used_nodes.emplace(alter);
                std::pair<ID, ID> path_tuple (std::make_pair(node, alter));
                std::cout << "Node: " << node << " alter: " << alter << " at distance " << distance << "\n";
                std::cout << "Number of used nodes: " << used_nodes.size() << "\n";
                std::cout << "Tuple: " << path_tuple.first << " " << path_tuple.second << "\n"; } } }
    return 0; }


/*
 * Unweighted Shortest Path Between Two Nodes
 *
 * Algorithm
 * start with two nodes
 * find alters for each node
 *      save alter pairs
 * find alters for alters
 *      save alter pairs
 * if alters in previous list, not shortest path, remove
 * Repeat
 * When shortest paths from each node meet, there is the shortest path
 */
//folly::fbvector<Path> ShortestPaths(const Graph& graph, const Node& start, const Node& end) {
//folly::fbvector<ID> id_path;
/*
std::vector<Path> ShortestPaths(const Graph& graph, const Node& start, const Node& end) {
    Path                toward;
    std::fbvector<ID>   id_path;

    for(const auto graph : g.edges) {  }

    return adjacencylist; }

//////////////////////////////////////////////////////////

using Element = std::variant<A, B, C>;
struct Eq {
    template <typename T>
    bool operator()(T const& lhs, T const& rhs) const {
        return equalNode(lhs, rhs); }
    template <typename T, typename U>
    bool operator()(T const&, U const&) const {
        return false; } };
bool equalTree(Element const& lhs, Element const& rhs) {
    return std::visit(Eq{}, lhs, rhs); } */

//////////////////////////////////////////////////////////
/*
struct Branch {
    std::vector<ID>     ids;
    Node                node;
    Node                alter;
    Path                root; } */

/*
Path ShortestPath(Graph graph, Node start, Node finish, Altermap altermap) { }

std::vector<Branch> AddBranches(Graph graph, Node node) {
    std::vector<Branch> branches
    for(const auto& edge : graph.edges) {
    if(edge.from == start || edge.to == start) {
        if(edge.from == node) {
            Branch branch {.ids = branch.ids.emplace_back(edge.id), .node = node, .alter = edge.to, .root = root.emplace_back(edge) };
            branches.emplace_back(branch);
        } else {
            Branch branch {.ids = branch.ids.emplace_back(edge.id), .node = edge.from, .alter = node, .root = root.emplace_back(edge) };
            branches.emplace_back(branch); } }  }
    return branches; } */

/* 
Path ShortestPath(Graph graph, Node start, Node finish) {
    std::vector<std::vector<Branch>> start_paths;
    std::vector<std::vector<Branch>> end_paths;
    // INIT
    start_paths.emplace_back(AddBranches(graph, start));
    end_paths.emplace_back(AddBranches(graph, finish));
    for(const auto& branch : start_paths) {
        if(branch) }
    do {

    } while ( shortest_path == false ); }


// Unweighted Shortest Path For All Nodes
// std::vector<Edgelist> ShortestPaths(const Graph& graph) {}

// Weighted Shortest Path Between Two Nodes
// std::vector<Edgelist> ShortestPaths(const Graph& graph, Attributes weights, const Node& start, const Node& end) {}

// Weighted Shortest Path For All Nodes
// std::vector<Edgelist> ShortestPaths(const Graph& graph, Attributes weigsts) {}
 */
#endif //PANGOLIN_SHORTESTPATHS_HPP