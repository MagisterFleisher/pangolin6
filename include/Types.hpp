#ifndef PANGOLIN_TYPES_HPP
#define PANGOLIN_TYPES_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <string>
#include <execution>
#include <algorithm>

#include <vector>
#include <set>
#include <map>
#include <unordered_map>

/********************* MACROS AND SHORTCUT FUNCTIONS ******************/
#define RANGE(x)        x.begin(),      x.end()
#define cRANGE(x)       x.cbegin(),     x.cend()

template<typename item_type>
item_type UniqueVector(item_type it) {
    std::sort(std::execution::unseq, RANGE(it));
    it.erase(std::unique(RANGE(it)), it.end());
    it.shrink_to_fit();
    return it; }

/********************* TYPES *******************************************/
enum Direction          { in,       out,    both };
enum Magnitude          { giant,    large,    small,  tiny };
enum Directionality     { directed, undirected };
enum FileFormat         { csv,      node,   pajak,  ucinet };

using Integer       =   std::uint_fast64_t;
using ID            =   Integer;

using Node          =   ID;
using Nodelist      =   std::vector<Node>;

typedef struct {
    ID id;
    Node from;
    Node to;        }   Edge;
using Edgelist      =   std::vector<Edge>;

typedef struct Graph {
    Magnitude           magnitude;
    Directionality      directionality = directed;
    Edgelist            edges;
    Nodelist            nodes;
                    }   Graph;

typedef struct {
    Node                node;
    std::uint_fast64_t  DegreeBoth;
    std::uint_fast64_t  DegreeIn;
    std::uint_fast64_t  DegreeOut;
    std::uint_fast64_t  PrestigeBoth;
    std::uint_fast64_t  PrestigeIn;
    std::uint_fast64_t  PrestigeOut;
                    }   NodeAttributes;

#endif//PANGOLIN_TYPES_HPP