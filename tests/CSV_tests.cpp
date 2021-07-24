#include "../include/ReadCSV.hpp"
#include <iostream>
// Graphsize FindGraphsize(const Edgelist& edgelist) { }
// int FileOpenError(const std::string& file_name) { }
/*---------------------------------- ALTER MAP ------------------------------- */
// Altermap ParseAltermap() {}
/*---------------------------------- NODE EDGE MAP ------------------------------- */
// EdgeAltermap NodeEdgemap() { }
/*---------------------------------- EDGE MAP ------------------------------- */
// Edgemap ParseEdgemap() { }
/*---------------------------------- EDGE LIST ------------------------------- */
// Edge ParseEdge(std::ifstream csv, const std::string& full_name, const int& skip_lines, const std::uint8_t& base) { }
// Edgelist LoadEdgelist() { }
/*---------------------------------- NODE LIST ------------------------------- */
// Node ParseNode(std::ifstream csv, const std::string& full_name, const int& skip_lines, const std::uint8_t& base) { }
// Nodelist LoadNodelist() {}
/*---------------------------------- GRAPH ------------------------------- */
// Graph GenGraphNodelistEdgelist(const std::string& file_name, const int& skip_lines, const std::uint8_t& base) { }
// Graph GenGraphEdgelist(const std::string& file_name, const int& skip_lines, const std::uint8_t& base) {}
/*---------------------------------- GENERAL ------------------------------- */
// Template<Representation R>
// R Read(DataStructure data_structure, const std::string& file_name, const int& skip_lines, const std::uint8_t& base) {}
int main() {
    const auto& nodelist (csv::Read(DataStructure::nodelist, "soc", 0, 10));
    return 0; }