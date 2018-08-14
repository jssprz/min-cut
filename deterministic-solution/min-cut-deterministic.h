//
// Created by Jesus on 8/2/2018.
//

#ifndef MIN_CUT_MIN_CUT_DETERMINISTIC_H
#define MIN_CUT_MIN_CUT_DETERMINISTIC_H

#include "../graph.h"
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/read_dimacs.hpp>

namespace min_cut {
//    std::vector<w_edge> weighted_min_cut(WGraph_adj_list G){
//        unsigned int s = 0;
//        auto n = num_vertices(G);
//        long min = LONG_MAX;
//        for (size_t t = 1; t < n; ++t) {
//            long flow = boost::push_relabel_max_flow(G, s, t);
//            if(flow < min)
//                min = flow;
//        }
//    }

    // weighted directed graph representation with an adjacency list
    typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
    typedef property<vertex_index_t, unsigned int> VertexNameProperty;
    typedef property<edge_capacity_t, long, property<edge_residual_capacity_t, long, property<edge_reverse_t, Traits::edge_descriptor>>> EdgeResidualCapacityProperty;
    typedef adjacency_list<vecS, vecS, directedS, VertexNameProperty, EdgeResidualCapacityProperty> ResidualGraph;

    std::vector<w_edge> weighted_min_cut(ResidualGraph G){
        using namespace boost;

        std::vector<w_edge> result;
        long flow;

        property_map<ResidualGraph, edge_capacity_t>::type
                capacity = get(edge_capacity, G);
        property_map<ResidualGraph, edge_reverse_t>::type
                rev = get(edge_reverse, G);
        property_map<ResidualGraph, edge_residual_capacity_t>::type
                residual_capacity = get(edge_residual_capacity, G);

        Traits::vertex_descriptor s, t;
        read_dimacs_max_flow(G, capacity, rev, s, t);

        flow = push_relabel_max_flow(G, s, t);

        //std::cout << "c  The total flow:" << std::endl;
        //std::cout << "s " << flow << std::endl << std::endl;

        //std::cout << "c flow values:" << std::endl;
        graph_traits<ResidualGraph>::vertex_iterator u_iter, u_end;
        graph_traits<ResidualGraph>::out_edge_iterator ei, e_end;
        for (tie(u_iter, u_end) = vertices(G); u_iter != u_end; ++u_iter)
            for (tie(ei, e_end) = out_edges(*u_iter, G); ei != e_end; ++ei)
                if (capacity[*ei] > 0)
                    result.push_back(w_edge(*u_iter, target(*ei, G), (capacity[*ei] - residual_capacity[*ei])));
//                    std::cout << "f " << *u_iter << " " << target(*ei, G) << " "
//                              << (capacity[*ei] - residual_capacity[*ei]) << std::endl;
        return result;
    }

    template <typename T>
    std::vector<edge> min_cut_deterministic(Graph<T> G){
//        std::vector<w_edge> WE;
//        for (edge e : G.E) {
//            WE.push_back(w_edge(e.first, e.second, 1));
//        }
//
//        WGraph WG;
//        WG.V = G.V;
//        WG.E = WE;
//
//        return weighted_min_cut(create_adj_list(WG));
        ResidualGraph RG;
        auto flow = weighted_min_cut(RG);
        return std::vector<edge>();
    }
}

#endif //MIN_CUT_MIN_CUT_DETERMINISTIC_H
