#ifndef PANGOLIN_GENCLIQUES_HPP
#define PANGOLIN_GENCLIQUES_HPP
#pragma GCC optimize("Ofast")
#pragma once
#include <execution>
#include <iostream>
#include <set>
#include "GenAlters.hpp"
#include "Types.hpp"

Nodelist GenAlterList(Altermap altermap, Nodelist alters, Node node) {
    Nodelist alter_alters_raw;
    std::for_each(std::execution::unseq, cRANGE(alters),                            /* loop through the alters */
        [&alters, &alter_alters_raw, &node, &altermap](const auto& alter) {   /* loop through the alters of those alters */
            std::for_each(std::execution::unseq, cRANGE((altermap.at(alter))),
                [&alters, &node, &alter_alters_raw](const auto& alt_alt) {
                    if((alt_alt == node) || ((std::find(std::execution::unseq, cRANGE(alters), alt_alt)) != alters.end())) {
                        alter_alters_raw.emplace_back(alt_alt);}  }); });           /* Filter for the intersection between the node, its alters, its alters' alters - the clique */
    return (SquishNodelist(alter_alters_raw)); }


std::set<Nodelist> GenCliques(const Altermap& altermap) {
    std::set<Nodelist>  cliques;
    std::for_each(std::execution::par_unseq, cRANGE(altermap), [&cliques, &altermap](const auto& node_alter) { /* loop through altermap */
        switch(node_alter.second.size()) { case 1: break; default:  {                       /* find nodes with mulitple alters - binary relations aren't cliques */
            const Nodelist&     alters          (node_alter.second);                            /* get the set of alters for those nodes */
            const Node&         node            (node_alter.first);
            Nodelist alter_alters (GenAlterList(altermap, alters, node));
            switch(alter_alters.size()) { case 1: break; default: {
                cliques.emplace(alter_alters);  }  }
            }
        }
    });
    return cliques;}
#endif//PANGOLIN_GENCLIQUES_HPP