//
// Created by Jesus on 8/2/2018.
//

#ifndef MIN_CUT_GRAPH_H
#define MIN_CUT_GRAPH_H

#include <vector>
#include <utility>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

namespace min_cut{

    // undirected graph representation with an adjacency list
    typedef adjacency_list<vecS, vecS, undirectedS> Graph_adj_list;

    // weighted undirected graph representation with an adjacency list
    typedef property<edge_weight_t, double> EdgeWeightProperty;
    typedef adjacency_list<vecS, vecS, undirectedS, no_property, EdgeWeightProperty> WGraph_adj_list;

    typedef std::pair<unsigned int, unsigned int> edge;
    typedef std::tuple<unsigned int, unsigned int, double> w_edge;

    struct edge_t
    {
        unsigned long first;
        unsigned long second;
    };

    template <typename T>
    struct Graph{
        std::vector<T> V;
        std::vector<edge> E;
    };

    template <typename T>
    struct WGraph{
        std::vector<T> V;
        std::vector<w_edge> E;
    };

    template <typename T>
    Graph_adj_list create_adj_list(Graph<T> g){
        // create a typedef for the Graph type
        Graph_adj_list result(g.V.size());

        // add the edges to the graph object
        for (int i = 0; i < g.E.size(); ++i)
            add_edge(g.E[i].first, g.E[i].second, result);

        return result;
    }

    template <typename T>
    WGraph_adj_list create_adj_list(WGraph<T> g){
        // create a typedef for the Graph type
        WGraph_adj_list result(g.V.size());

        // add the edges to the graph object
        for (int i = 0; i < g.E.size(); ++i)
            add_edge(std::get<0>(g.E[i]), std::get<1>(g.E[i]), std::get<2>(g.E[i]), result);

        return result;
    }
}

#endif //MIN_CUT_GRAPH_H
