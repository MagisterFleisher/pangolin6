#ifndef PANGOLIN_SIMPLIFYGRAPH_HPP
#define PANGOLIN_SIMPLIFYGRAPH_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <execution>
#include <set>
#include <iostream>
#include <algorithm>
#include "Types.hpp"
#include "GraphProperties.hpp"

Nodelist To_Undirected(Nodelist order_edge) {
    std::sort(std::execution::par_unseq, RANGE(order_edge));
    return order_edge; }

std::vector<Nodelist> Edge_Reduce(std::vector<Nodelist> node_node_list, const Edgelist& edges, const Directionality& direction) {
    std::for_each(std::execution::unseq, cRANGE(edges), [&node_node_list, &direction](const auto& edge) {
        Nodelist order_edge {edge.from, edge.to};
        if(direction == undirected) {
            order_edge = To_Undirected(order_edge);
        } else {
        node_node_list.emplace_back(order_edge); } });
    return node_node_list; }

Edgelist EdgeReduce_Vector(const Edgelist& edges, const Directionality& direction) {
    Edgelist reduce_edges;
    std::vector<Nodelist>   node_node_list;
    node_node_list          =               (UniqueVector<std::vector<Nodelist>>(Edge_Reduce(node_node_list, edges, direction)));
    std::size_t       node_node_list_size     (node_node_list.size());
    for(std::size_t i = 0; i != node_node_list_size; ++i) {
        Edge temp_edge {.id = static_cast<ID>(i), .from = node_node_list[i][0], .to = node_node_list[i][1]};
        reduce_edges.emplace_back(temp_edge);}
    return reduce_edges; }

Graph SimplifyGraph(const Graph& graph) {
    Graph simple_graph;
    simple_graph.directionality = graph.directionality;
    simple_graph.edges          = EdgeReduce_Vector(graph.edges, simple_graph.directionality);
    simple_graph.magnitude      = FindMagnitude(simple_graph.edges.size());
    simple_graph.nodes          = std::move(graph.nodes);
    return simple_graph; }
#endif//PANGOLIN_SIMPLIFYGRAPH_HPP