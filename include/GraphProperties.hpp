#ifndef PANGOLIN_GRAPH_PROPERTIES_HPP
#define PANGOLIN_GRAPH_PROPERTIES_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include "Types.hpp"
#include <unordered_map>
#include <iomanip>
#include <iostream>
/*---------------------------------- MAGNITUDE ------------------------------- */
Magnitude FindMagnitude(const std::uint_fast64_t& edge_count) {
    Magnitude magnitude;
    switch(edge_count) {
        case 0: {                       std::cerr << "ReadGraph: No edges.  That's a problem."; exit(1);};
        case 1 ... 500 : {              magnitude = tiny;    break; };
        case 501 ... 100'000 : {        magnitude = small;   break; };
        case 100'001 ... 30'000'000 : { magnitude = large;   break; };
        default : {                     magnitude = giant;   break; }; }
    return magnitude; }

std::string MagnitudeString(const Magnitude& magnitude) {
    std::string magnitude_string;
    switch(magnitude) {
        case giant: { magnitude_string = "giant"; break; }
        case large: { magnitude_string = "large"; break; }
        case small: { magnitude_string = "small"; break; }
        case tiny:  { magnitude_string = "tiny"; break; }
        default:    { magnitude_string = ""; break; } }
    return magnitude_string; }

std::string DirectionalityString(const Directionality& directionality) {
    std::string directionality_string;
    switch(directionality) {
        case directed:      { directionality_string = "directed"; break; }
        case undirected:    { directionality_string = "undirected"; break; }
        default:            { directionality_string = ""; break; } }
    return directionality_string; }

void Summarize_Graph(const Graph& g) {
    std::cout << "Graph edges #: " << g.edges.size() << "\n";
    std::cout << "Graph nodes #: " << g.nodes.size() << "\n";
//    for(const auto& edge: raw_graph.edges) { std::cout << "Edge ID: " << edge.id << " From: " << edge.from << " To: " << edge.to << "\n"; }
//    for(const auto& node: raw_graph.nodes) { std::cout << "Node ID: " << node << "\n"; }
    std::cout << "Graph Magnitude: " << MagnitudeString(g.magnitude) << "\n";
    std::cout << "Graph Directionality: " << DirectionalityString(g.directionality) << "\n"; }

void Summarize_Nodes(const std::unordered_map<Node, Integer>& Degree) {
    for(const auto& [node, degree] : Degree) { std::cout << "Node: " << node << " Degree: " << degree << "\n"; } }

template<typename Prestige_type>
void Summarize_Prestige(const Prestige_type& Prestige) {
    std::vector<ID> key_vector;
    for(const auto& [node, prestige] : Prestige) { key_vector.emplace_back(node); }
    std::sort(RANGE(key_vector));
    std::reverse(RANGE(key_vector));
    for(const auto& key : key_vector) { std::cout << "Node: " << key << " Prestige: " << std::setprecision(25) << Prestige.at(key) << "\n"; } }

void Summarize_NodeAlter(const std::unordered_map<Node, Nodelist> node_alters) {
    for(const auto& [node, alters] : node_alters) {
        std::cout << "Node: " << node; 
        for(const auto& alter : alters ) {
            std::cout << " alter: " << alter;}
        std::cout << "\n";} }

#endif//PANGOLIN_GRAPH_PROPERTIES_HPP