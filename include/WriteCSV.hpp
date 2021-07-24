#ifndef PANGOLIN_CSV_HPP
#define PANGOLIN_CSV_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include "Types.hpp"
#include "GenNodelist.hpp"
#include "CheckFile.hpp"

int FileOpenError(const std::string& file_name) {
    const auto&         err             ("\tRead_CSV: Error opening file ");
    const auto&         err_name        (file_name);
    const auto&         err_message     (err + err_name);
    std::cerr << err_message << "\n";
    exit(1);
    return 0; }

Nodelist ParseNodelist(std::ifstream csv, const std::string& full_name, const int& skip_lines, const std::uint8_t& base) {
    // TODO Make nodes file a node/attribute file
    Nodelist nodes;
    std::string         line;
    if(csv.fail()) { const auto& error_return (FileOpenError(full_name));} /* Check if file can open.  Exit with error if not.  */
    switch(skip_lines) {
        case 0 : break;
        default: {
            for(int i = 0; i < skip_lines; i++) { csv.ignore(10,'\n'); }                        /* When specified, Ignore the first n lines of the CSV file. */
            break;}; }
    while(std::getline(csv, line)) {
        Node node;
        switch(base) {
            case 8:     { std::sscanf(line.c_str(), "%lo", &node); break; };
            case 10:    { std::sscanf(line.c_str(), "%lu", &node); break; };
            case 16:    { std::sscanf(line.c_str(), "%lx", &node); break; };
            default:    { std::cerr << "Problem reading base of csv.\n"; break; }
        nodes.emplace_back(node); }
        }
    return nodes; }

Edgelist ParseEdgelist(std::ifstream csv, const std::string& full_name, const int& skip_lines, const std::uint8_t& base) {
    Edgelist edges;
    std::string         line;
    if(csv.fail()) { const auto& error_return (FileOpenError(full_name)); } /* Check if file can open.  Exit with error if not.  */
    switch(skip_lines) {
        case 0 : break;
        default: {
            for(int i = 0; i < skip_lines; i++) { csv.ignore(10,'\n'); }                        /* When specified, Ignore the first n lines of the CSV file. */
            break;}; }
    while(std::getline(csv, line)) {
        Edge edge;
        switch(base) {
            case 8:     { std::sscanf(line.c_str(), "%lo,%lo,%lo", &edge.id, &edge.from, &edge.to); break; };
            case 10:    { std::sscanf(line.c_str(), "%lu,%lu,%lu", &edge.id, &edge.from, &edge.to); break; };
            case 16:    { std::sscanf(line.c_str(), "%lx,%lx,%lx", &edge.id, &edge.from, &edge.to); break; };
            default:    { std::cerr << "Problem reading base of csv.\n"; break; } }
        edges.emplace_back(edge); }
    return edges; }

Template<typename GraphRepresentation>
GraphRepresentation Read_CSV(const std::string& file_name, DataStructure data_structure, const int& skip_lines, const std::uint8_t& base) {
    GraphRepresentation graph_representation;
    const auto&         csv_ext         {"_edges.csv"};
    const auto&         full_name       {file_name + csv_ext};
    std::ifstream       csv             (full_name);
    switch(data_structure) {
        case nodelist:          {
            graph_representation = ParseNodelist(std::ifstream csv, base);
            break; }
        case edgelist:          {
            graph_representation = ParseEdgelist(std::ifstream csv, base);
            break; }
        case edgemap:           {
            graph_representation = ParseEdgemap(std::ifstream csv, base);
            break; }
        case edge_altermap:     {
            graph_representation = ParseEdgeAltermap(std::ifstream csv, base);
            break; }
        case altermap:          {
            graph_representation = ParseAltermap(std::ifstream csv, base);
            break; }
        case node_edgemap:      {
            graph_representation = ParseNodeEdgemap(std::ifstream csv, base);
            break; }
        case incidence_matrix:  {
            graph_representation = ParseIncidenceMatrix(std::ifstream csv, base);
            break; }
        case adjacency_matrix:  {
            graph_representation = ParseAdjacencyMatrix(std::ifstream csv, base);
            break; }
        default: break; }
    return edges; }

Nodelist ReadNodelist_CSV(const std::string& file_name, const int& skip_lines, const std::uint8_t& base) {
    Nodelist                    nodes;
    const auto&                 csv_ext         ("_nodes.csv");
    const auto&                 full_name       (file_name + csv_ext);
    std::ifstream               csv             (full_name);
    std::string                 line;
    if(csv.fail()) { const auto& error_return (FileOpenError(full_name)); /* Check if file can open.  Exit with error if not.  */ }
    switch(skip_lines) { case 0 : break; default: {
        for(int i = 0; i < skip_lines; i++) { csv.ignore(10,'\n'); }                        /* When specified, Ignore the first n lines of the CSV file. */
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
    const auto&                 path                ("./data/" + file_name + "/" + file_name);
    g.nodes = ReadNodelist_CSV<DataStructure::nodelist>(path, skip_lines, base);
    g.edges = ReadEdgelist_CSV<DataStructure::edgelist>(path, skip_lines, base);
    g.graph_size = FindGraphsize(g.edges);
    return g; }

std::optional<std::string> WriteGraph_EdgesCSV(const Graph& g, const std::string& file_name, const std::uint8_t& base) { 
    std::cout << "\tCalled WriteGraph_EdgesCSV\n";
    const std::string&          file_edges_save     (("data/" + file_name) + "_edges.csv");
    std::ofstream               out_edges_CSVFile   ( file_edges_save, std::ios::out | std::ios::binary);
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
    const std::string&          file_nodes_save     (("data/" + file_name) + "_nodes.csv");
    std::ofstream               out_nodes_CSVFile   ( file_nodes_save, std::ios::out | std::ios::binary);
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
    const auto&                 err_edges           (WriteGraph_EdgesCSV(g, file_name, base));
    const auto&                 err_nodes           (WriteGraph_NodesCSV(g, file_name, base));
    return std::nullopt; }
#endif//PANGOLIN_CSV_HPP
