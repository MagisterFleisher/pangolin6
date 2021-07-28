// TODO:
// FIXME:
// XXX:
// HACK:
// BUG:
// CONSIDER:
// DOING:
// CLARIFY:
// MARK:
// !!!:
// ???:
// WARNING:
// UNDONE:
//
#pragma GCC optimize("Ofast")
#include <future>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <chrono>
#include <random>
#include <ratio>
#include <vector>
#include "DegreeCentrality.hpp"
#include "GenAlters.hpp"
#include "GraphProperties.hpp"
#include "PrestigeSimple.hpp"
#include "ReadFile.hpp"
#include "SimplifyGraph.hpp"
#include "Types.hpp"
#include "EigenLibraryTest.hpp"
// #include "GenCliques.hpp"
// #include "GenEdgemap.hpp"
// #include "GenNodeEdgemap.hpp"
// #include "DisconnectedSubgraphs.hpp"
/* I got the following timing code from someone else.  Hence the ugly used of using to simply get rid of std and whatnot */
using std::milli;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

void print_results(const char *const tag, high_resolution_clock::time_point startTime, high_resolution_clock::time_point endTime)
{
    printf("\t\t%s: Time: %fms\n\n", tag, duration_cast<duration<double, milli>>(endTime - startTime).count());
}
/*
NodeAttributes GenAllNodeAttributes(const Graph& graph, const Altermap& altermap) {
    NodeAttributes      node_attributes_table   (graph.nodes.size());
    std::future<Attribute>  DegreeBoth              (std::async([&]() {    return Degree(graph, both);}));
    std::future<Attribute>  DegreeOut               (std::async([&]() {    return Degree(graph, out);}));
    std::future<Attribute>  DegreeIn                (std::async([&]() {    return Degree(graph, in);}));
    const Attribute&        degree_both             (DegreeBoth.get());
    std::future<Attribute>  PrestigeBoth            (std::async([&]() {    return PrestigeSimple(altermap, degree_both); }));
    const Attribute&        degree_out              (DegreeOut.get());
    std::future<Attribute>  PrestigeOut             (std::async([&]() {    return PrestigeSimple(altermap, degree_out); }));
    const Attribute&        degree_in               (DegreeIn.get());
    std::future<Attribute>  PrestigeIn              (std::async([&]() {    return PrestigeSimple(altermap, degree_in); }));
    const auto&             prestige_both           (PrestigeBoth.get());
    const auto&             prestige_out            (PrestigeOut.get());
    const auto&             prestige_in             (PrestigeIn.get());
    std::cout<<"Creating NodeAttributesTable"<<"\n";
    for(const auto& node : graph.nodes) {
        NodeAttributes node_att ({
            .node           =   node,
            .DegreeBoth     =   degree_both.at(     node),
            .DegreeIn       =   degree_in.at(       node),
            .DegreeOut      =   degree_out.at(      node),
            .PrestigeBoth   =   prestige_both.at(   node),
            .PrestigeIn     =   prestige_in.at(     node),
            .PrestigeOut    =   prestige_out.at(    node) });
        node_attributes_table.emplace_back(         node_att); }
    return node_attributes_table; } */

int main(int argc, char *argv[])
{
    // const auto                         startTimeAll         (high_resolution_clock::now());
    const std::string &file_name(argv[1]);
    const std::size_t &skip_lines(static_cast<std::size_t>(std::stoi(argv[2], nullptr, 10))); /* how many lines to skip at beginning of csv */
    const std::uint8_t &base_read(static_cast<std::uint8_t>(std::stoi(argv[3], nullptr, 10)));
    // const std::uint8_t&                  base_write             (static_cast<std::uint8_t> (std::stoi(argv[4], nullptr, 10)) );

    std::cout << "Main: File: " << file_name << "\n";
    std::cout << "Main: argc: " << argc << "\n";
    const auto startTimeReadCSV(high_resolution_clock::now());
    Graph raw_graph(Graph_Read(csv, file_name, skip_lines, base_read));
    const auto endTimeReadCSV(high_resolution_clock::now());

    Summarize_Graph(raw_graph);
    print_results("ReadGraph_CSV(graph)", startTimeReadCSV, endTimeReadCSV);

    const auto startTimeSimplify(high_resolution_clock::now());
    Graph graph(SimplifyGraph(raw_graph));
    const auto endTimeSimplify(high_resolution_clock::now());

    Summarize_Graph(graph);
    print_results("SimplifyGraph(raw_graph, undirected)", startTimeSimplify, endTimeSimplify);

    if (raw_graph.edges.size() == graph.edges.size())
    {
        graph.edges.clear();
        graph.edges.shrink_to_fit();
        graph.nodes.clear();
        graph.nodes.shrink_to_fit();
        // Graph& graph (raw_graph);
    }

    std::cout << "\nCalling Gen_NodeAlters of graph\n";
    const auto startTimeNodeAlters(high_resolution_clock::now());
    const auto &node_alters(Gen_NodeAlters(raw_graph));
    const auto endTimeNodeAlters(high_resolution_clock::now());
    // Summarize_NodeAlter(node_alters);
    print_results("Gen_NodeAlters(graph)", startTimeNodeAlters, endTimeNodeAlters);

    std::cout << "\nCalling Degree (both) of raw graph\n";
    const auto startTimeDegree(high_resolution_clock::now());
    const std::unordered_map<Node, Integer> &node_degree(Degree(raw_graph, both));
    const auto endTimeDegree(high_resolution_clock::now());
    //    Summarize_Nodes(node_attributes);
    print_results("Degree(raw_graph, both)", startTimeDegree, endTimeDegree);

    std::cout << "\nCalling in-Degree of raw graph\n";
    const auto startTimeDegree_in(high_resolution_clock::now());
    const std::unordered_map<Node, Integer> &node_attributes_in(Degree(raw_graph, in));
    const auto endTimeDegree_in(high_resolution_clock::now());
    //    Summarize_Nodes(node_attributes_in);
    print_results("Degree(raw_graph, in)", startTimeDegree_in, endTimeDegree_in);

    std::cout << "\nCalling out-Degree of raw graph\n";
    const auto startTimeDegree_out(high_resolution_clock::now());
    const auto node_attributes_out(Degree(raw_graph, out));
    const auto endTimeDegree_out(high_resolution_clock::now());
    //    Summarize_Nodes(node_attributes_out);
    print_results("Degree(raw_graph, in)", startTimeDegree_out, endTimeDegree_out);

    std::cout << "\nCalling Prestige of raw graph\n";
    const auto node_size(raw_graph.nodes.size());
    const auto startTimePrestige(high_resolution_clock::now());
    const auto node_prestige_both(Prestige<decltype(node_degree), decltype(node_size)>(raw_graph, node_alters, node_degree, node_size));
    const auto endTimePrestige(high_resolution_clock::now());
    print_results("PrestigeSimple(raw_graph, in)", startTimePrestige, endTimePrestige);
    Summarize_Prestige(node_prestige_both);

    std::cout << "\nCalling EigenLibraryTest\n";
    const auto startEigenLibraryTest(high_resolution_clock::now());
    EigenTest();
    const auto endEigenLibraryTest(high_resolution_clock::now());
    //    Summarize_Nodes(node_attributes_out);
    print_results("EigenLibraryTest()", startTimeDegree_out, endTimeDegree_out);


    // for(auto node : raw_graph.nodes) { std::cout << "Node: " << node << "\n"; }

    // std::string                          write_file_name        (file_name);
    // const auto&                          write_err              (WriteGraph_CSV(graph, write_file_name, base_write));
    /*
    std::cout << "\nCalling GenEdgemap\n";
    const auto                         startTimeEdgemap                 (high_resolution_clock::now());
    const Edgemap&                     edgemap                          (GenEdgemap(graph));
    const auto                         endTimeEdgemap                   (high_resolution_clock::now());
    print_results("GenEdgemap(graph)", startTimeEdgemap, endTimeEdgemap); */
    /*
    std::cout << "\nCalling GenNodeEdgemap\n";
    const auto                         startTimeNodeEdgemap             (high_resolution_clock::now());
    const NodeEdgemap&                 node_edgemap                     (GenNodeEdgemap(graph));
    const auto                         endTimeNodeEdgemap               (high_resolution_clock::now());
    print_results("GenNodeEdgemap(graph)", startTimeEdgemap, endTimeEdgemap);

    std::cout << "Beginning Node Attributes Table" << "\n";
    const auto                         startTimeNodeAtts              (high_resolution_clock::now());
    std::future<NodeAttributeTable>    NodeAttsTable           (std::async([&]() {    return GenAllNodeAttributes(raw_graph, altermap);}));
    const auto&                        node_atts_table         (NodeAttsTable.get());
    const auto                         endTimeNodeAtts              (high_resolution_clock::now());
    print_results("GenAllNodeAttributes(graph, altermap)", startTimeNodeAtts, endTimeNodeAtts);

    const auto                         endTimeAll              (high_resolution_clock::now());
    print_results("Total Program run: ", startTimeAll, endTimeAll); */
    return 0;
}