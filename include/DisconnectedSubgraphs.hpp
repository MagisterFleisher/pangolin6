#ifndef PANGOLIN_DISCONNECTEDSUBGRAPHS_HPP
#define PANGOLIN_DISCONNECTEDSUBGRAPHS_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <vector>
#include <map>
#include <execution>
#include <iostream>
#include "Types.hpp"

// Find singleton nodes
///  Get the BothDegree from the Node Table
///  Look for nodes with degree 0
///  Write each node as subgraph in the subgraph list
///  Remove nodes from Alterlist or Edgelist (whichever0 used)

namespace { // Anonymous namespace to keep precompile global definitions local to these functions

using Subgraphs = std::unordered_map<Node, std::size_t>;


// 2.

}
#endif // PANGOLIN_DISCONNECTEDSUBGRAPHS_HPP