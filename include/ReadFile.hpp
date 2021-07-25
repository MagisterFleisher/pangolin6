#ifndef PANGOLIN_READ_FILE_HPP
#define PANGOLIN_READ_FILE_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include "Types.hpp"
#include "GenNodelist.hpp"
#include "CheckFile.hpp"
#include "GraphProperties.hpp"

/*---------------------------------- FILE ERROR ------------------------------- */
void FileOpenError(const std::string& file_name) {  // Change this to work with some sort of return value.  I hate void functions.
    const auto&         err             ("\tRead_CSV: Error opening file ");
    const auto&         err_name        (file_name);
    const auto&         err_message     (err + err_name);
    std::cerr << err_message << "\n";
    exit(1); }

/*---------------------------------- EDGE LIST ------------------------------- */
Edge Parse_Edge(const std::string& line, const std::uint8_t& base) {
    /* EDGE1, EDGE2 */
    Edge edge;
    switch(base) {
        case 8:     { std::sscanf(line.c_str(), "%lo,%lo,%lo", &edge.id, &edge.from, &edge.to); break; }
        case 10:    { std::sscanf(line.c_str(), "%lu,%lu,%lu", &edge.id, &edge.from, &edge.to); break; }
        case 16:    { std::sscanf(line.c_str(), "%lx,%lx,%lx", &edge.id, &edge.from, &edge.to); break; }
        default:    { std::cerr << "Problem reading base of csv.\n"; break; } }
    return edge; }

Edgelist Read_Edgelist_CSV(const std::string& file_name, const std::size_t& skip_lines, const std::uint8_t& base) {
    Edgelist            edges;
    const auto&         csv_ext             {"_edges.csv"};
    const auto&         full_name           {file_name + csv_ext};

    std::ifstream       file_input_stream   (full_name);
    std::string         line;

    if(file_input_stream.fail()) { FileOpenError(full_name); /* Check if file can open.  Exit with error if not.  */ }
    switch(skip_lines) { case 0 : break; default: {
        for(std::size_t i = 0; i != skip_lines; i++) { file_input_stream.ignore(10,'\n'); }  // When specified, Ignore the first n lines of the CSV file.
        break;}; }
    while(std::getline(file_input_stream, line)) {
        edges.emplace_back(Parse_Edge(line, base)); }
    return edges; }

/*---------------------------------- NODE LIST ------------------------------- */

Node Parse_Node(const std::string& line, const std::uint8_t& base) {
    /* NODE1 */  // TODO: NODE1, ATT1, ATT2, ATT3, ATT4
    Node node;
    switch(base) {
        case 8:     { std::sscanf(line.c_str(), "%lo", &node); break; }
        case 10:    { std::sscanf(line.c_str(), "%lu", &node); break; }
        case 16:    { std::sscanf(line.c_str(), "%lx", &node); break; }
        default:    { std::cerr << "Problem reading base of csv.\n"; break; } }
    return node; }

Nodelist Read_Nodelist_CSV(const std::string& file_name, const std::size_t& skip_lines, const std::uint8_t& base) {
    Nodelist            nodes;
    const auto&         csv_ext             ("_nodes.csv");
    const auto&         full_name           (file_name + csv_ext);
    std::ifstream       file_input_stream   (full_name);
    std::string         line;
    if(file_input_stream.fail()) { FileOpenError(full_name); /* Check if file can open.  Exit with error if not.  */ }
    switch(skip_lines) { case 0 : break; default: {
        for(std::size_t i = 0; i != skip_lines; i++) { file_input_stream.ignore(10,'\n'); }                        /* When specified, Ignore the first n lines of the CSV file. */
        break;}; }                                                                          // std::cout << "\tRead_CSV: Begin reading file\n";
    while(std::getline(file_input_stream, line)) { nodes.emplace_back(Parse_Node(line, base)); }     // std::cout << "\tRead_CSV: File successfully read\n\tCalling NodelistGen: Generating node list\n";
    // std::sort(std::execution::par_unseq, nodes.begin(), nodes.end());
    return nodes; }

/*---------------------------------- GRAPH ------------------------------- */

Graph Gen_Graph_Lists_CSV(const std::string& file_name, const std::size_t& skip_lines, const std::uint8_t& base) {
    Graph g;
    g.nodes = Read_Nodelist_CSV(file_name, skip_lines, base);
    g.edges = Read_Edgelist_CSV(file_name, skip_lines, base);
    g.magnitude = FindMagnitude(g.edges.size());
    return g; }

Graph Gen_Graph_Edgelist(const std::string& file_name, const std::size_t& skip_lines, const std::uint8_t& base) {
    Graph g;
    g.edges = Read_Edgelist_CSV(file_name, skip_lines, base);
    g.magnitude = FindMagnitude(g.edges.size());
    return g; }

/*---------------------------------- GENERAL ------------------------------- */

Graph Graph_Read(FileFormat format, const std::string& file_name, const std::size_t& skip_lines, const std::uint8_t& base) {
    Graph graph;
    const auto&    path  ("./data/" + file_name + "/" + file_name);
    switch(format) {
        case csv:   {
            graph = Gen_Graph_Lists_CSV(path, skip_lines, base);
            // Find whether both nodelist file and edgelist file exist, or only edgelist file exists
            break; }
        case node:  {
            break; }
        case pajak: {
            break; }
        case ucinet:{
            break; }
        default: { break;} }
    return graph; }

#endif//PANGOLIN_READ_FILE_HPP