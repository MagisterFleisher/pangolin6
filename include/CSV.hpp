#ifndef PANGOLIN_CSV_HPP
#define PANGOLIN_CSV_HPP
#pragma  GCC optimize("Ofast")
#pragma once
#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include "Types.hpp"
#include "GenNodelist.hpp"
#include "CheckFile.hpp"

namespace {  //  Anonymous namespace to allow for local globals, such as the enum for Filetype

enum Filetype { edge, node };

int FileOpenError(const std::string& file_name) {
    const auto&         err             ("\tRead_CSV: Error opening file ");
    const auto&         err_name        (file_name);
    const auto&         err_message     (err + err_name);
    std::cerr << err_message << "\n";
    exit(1);
    return 0; }

Node ParseNode(const std::string& line, const std::uint8_t& base) {
    // TODO Make nodes file a node/attribute file
    Node node;
    switch(base) {
        case 8:     { std::sscanf(line.c_str(), "%lo", &node); break; };
        case 10:    { std::sscanf(line.c_str(), "%lu", &node); break; };
        case 16:    { std::sscanf(line.c_str(), "%lx", &node); break; };
        default:    { std::cerr << "Problem reading base of csv.\n"; break; } }
    return node;}

Edge ParseEdge(const std::string& line, const std::uint8_t& base) {
    Edge edge;
    switch(base) {
        case 8:     { std::sscanf(line.c_str(), "%lo,%lo,%lo", &edge.id, &edge.from, &edge.to); break; };
        case 10:    { std::sscanf(line.c_str(), "%lu,%lu,%lu", &edge.id, &edge.from, &edge.to); break; };
        case 16:    { std::sscanf(line.c_str(), "%lx,%lx,%lx", &edge.id, &edge.from, &edge.to); break; };
        default:    { std::cerr << "Problem reading base of csv.\n"; break; } }
    return edge; }

Edgelist ReadEdgelist_CSV(const std::string& file_name, const int& skip_lines, const std::uint8_t& base) {
    Edgelist            edges;
    const auto&         csv_ext         {"_edges.csv"};
    const auto&         full_name       {file_name + csv_ext};
    std::ifstream       csv             (full_name);
    std::string         line;
    if(csv.fail()) { const auto& error_return (FileOpenError(full_name)); /* Check if file can open.  Exit with error if not.  */ }
    switch(skip_lines) { case 0 : break; default: {
        for(auto i = 0; i != skip_lines; i++) { csv.ignore(10,'\n'); }                        /* When specified, Ignore the first n lines of the CSV file. */
        break;}; }                                                                          // std::cout << "\tRead_CSV: Begin reading file\n";
    while(std::getline(csv, line)) { edges.emplace_back(ParseEdge(line, base)); }     // std::cout << "\tRead_CSV: File successfully read\n\tCalling NodelistGen: Generating node list\n";
    return edges; }

Nodelist ReadNodelist_CSV(const std::string& file_name, const int& skip_lines, const std::uint8_t& base) {
    Nodelist            nodes;
    const auto&         csv_ext         ("_nodes.csv");
    const auto&         full_name       (file_name + csv_ext);
    std::ifstream       csv             (full_name);
    std::string         line;
    if(csv.fail()) { const auto& error_return (FileOpenError(full_name)); /* Check if file can open.  Exit with error if not.  */ }
    switch(skip_lines) { case 0 : break; default: {
        for(auto i = 0; i != skip_lines; i++) { csv.ignore(10,'\n'); }                        /* When specified, Ignore the first n lines of the CSV file. */
        break;}; }                                                                          // std::cout << "\tRead_CSV: Begin reading file\n";
    while(std::getline(csv, line)) { nodes.emplace_back(ParseNode(line, base)); }     // std::cout << "\tRead_CSV: File successfully read\n\tCalling NodelistGen: Generating node list\n";
    return nodes; }

Graphsize FindGraphsize(const Edgelist& edgelist) {
    Graphsize graph_size;
    switch(edgelist.size()) {
        case 0: {                       std::cerr << "ReadGraph_CSV: No edges.  That's a problem."; exit(1);};
        case 1 ... 500 : {              graph_size = tiny;    break; };
        case 501 ... 100'000 : {        graph_size = small;   break; };
        case 100'001 ... 30'000'000 : { graph_size = big;     break; };
        default : {                     graph_size = giant;   break; }; }
    return graph_size; }

Graph LoadGraph_CSV(const std::string& file_name, const int& skip_lines, const std::uint8_t& base) {
    Graph g;
    const auto&     path    ("./data/" + file_name + "/" + file_name);
    g.nodes = ReadNodelist_CSV(path, skip_lines, base);
    g.edges = ReadEdgelist_CSV(path, skip_lines, base);
    g.graph_size = FindGraphsize(g.edges);
    return g; }
/*
Graph ReadGraph_CSV(const std::string& file_name, const int& skip_lines, const std::uint8_t& base) { // std::cout << "Called Read_CSV" << "\n";
    const auto&         path            ("./data/");
    const auto&         csv_ext         ("_edges.csv");
    const auto&         full_name       ((path + file_name) + csv_ext);
    std::ifstream       csv             (full_name);
    Graph               graph;
    std::string         line;*
    if(csv.fail()) {                                                                        /* Check if file can open.  Exit with error if not.  */
/*        const auto&      err             ("\tRead_CSV: Error opening file ");
        const auto&      err_name        (full_name);
        const auto&      err_message     (err + err_name);
        std::cerr << err_message << "\n"; exit(1); }                                       //  std::cout << "\tRead_CSV: File successfully opened\n\tSkipping "  << skip_lines << " lines\n";
    switch(skip_lines) { case 0 : break; default: {
        for(int i = 0; i < skip_lines; i++) { csv.ignore(10,'\n'); }                        /* When specified, Ignore the first n lines of the CSV file. */
/*        break;}; }                                                                          // std::cout << "\tRead_CSV: Begin reading file\n";
    while(std::getline(csv, line)) { graph.edges.emplace_back(ParseLine(line, base)); }     // std::cout << "\tRead_CSV: File successfully read\n\tCalling NodelistGen: Generating node list\n";
    /* When nodelist exists */
    /* When no nodelist exists */
//    graph.nodes = std::move(GenNodelist(graph.edges));                                      //std::cout << "\tNodelist size: " << graph.nodes.size() << "\n";
    // graph.nodes = GenNodelist(graph.edges); Need to benchmark                                      std::cout << "\tNodelist size: " << graph.nodes.size() << "\n";
                                                                                            //std::cout << "\tRead_CSV: Node list successfully generated\nReturning graph\n";
 /*   switch(graph.edges.size()) {
        case 0: {                       std::cerr << "ReadGraph_CSV: No edges.  That's a problem."; exit(1);};
        case 1 ... 500 : {              graph.graph_size = tiny;    break; };
        case 501 ... 100'000 : {        graph.graph_size = small;   break; };
        case 100'001 ... 30'000'000 : { graph.graph_size = big;     break; };
        default : {                     graph.graph_size = giant;   break; }; }
    return graph; } */

std::optional<std::string> WriteGraph_EdgesCSV(const Graph& g, const std::string& file_name, const std::uint8_t& base) { std::cout << "\tCalled WriteGraph_EdgesCSV\n"; 
const std::string&          file_edges_save     (("data/" + file_name) + "_edges.csv");
    std::ofstream           out_edges_CSVFile   ( file_edges_save, std::ios::out | std::ios::binary);
    if( out_edges_CSVFile.is_open()) {
        switch(base) {
        case 8:{ for(const auto& edge : g.edges ) {
            out_edges_CSVFile << std::oct << edge.id << "," << std::oct << edge.from << "," << std::oct << edge.to << "\n"; } break; };
        case 10:{ for(const auto& edge : g.edges ) {
            out_edges_CSVFile << edge.id << "," << edge.from << "," << edge.to << "\n"; } break; };
        case 16:{ for( const auto& edge : g.edges ) {
            out_edges_CSVFile << std::hex << edge.id << "," << std::hex << edge.from << "," << std::hex << edge.to << "\n"; } break; };
        default:{ std::cerr << "Problem finding base to write csv.\n"; return "Problem finding base to write csv."; } }
    out_edges_CSVFile.close();
    } else { std::cerr << "\tWriteGraphStream_CSV: Error opening " << file_edges_save << "\n"; }
    return std::nullopt; }

std::optional<std::string> WriteGraph_NodesCSV(const Graph& g, const std::string& file_name, const std::uint8_t& base) { std::cout << "\tCalled WriteGraph_NodesCSV\n"; 
    const std::string&      file_nodes_save     (("data/" + file_name) + "_nodes.csv");
    std::ofstream           out_nodes_CSVFile   ( file_nodes_save, std::ios::out | std::ios::binary);
    if( out_nodes_CSVFile.is_open()) {
        switch(base) {
        case 8:  { for(auto& node : g.nodes ) { out_nodes_CSVFile << std::oct << node << "\n";} break; };
        case 10: { for(auto& node : g.nodes ) { out_nodes_CSVFile << node << "\n";} break; };
        case 16: { for(auto& node : g.nodes ) { out_nodes_CSVFile << std::hex << node << "\n";} break; };
        default:{ std::cerr << "Problem finding base to write csv.\n"; return "Problem finding base to write csv."; } }
    out_nodes_CSVFile.close();
    } else { std::cerr << "\tWriteGraphStream_CSV: Error opening" << file_nodes_save << "\n";  }
    return std::nullopt; }

std::optional<std::string> WriteGraph_CSV(const Graph& g, std::string& file_name, const std::uint8_t& base) { std::cout << "\tCalled WriteGraph_CSV\n";
    /* Use futures to multitask */
    const auto& err_edges (WriteGraph_EdgesCSV(g, file_name, base));
    const auto& err_nodes (WriteGraph_NodesCSV(g, file_name, base));
    return std::nullopt; }
}
#endif//PANGOLIN_CSV_HPP
