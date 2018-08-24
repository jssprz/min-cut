//
// Created by Jesus on 8/2/2018.
//

#ifndef MIN_CUT_MIN_CUT_DETERMINISTIC_H
#define MIN_CUT_MIN_CUT_DETERMINISTIC_H

#include "../graph.h"


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

    std::vector<edge_t> min_cut_max_flow(unsigned long n, const std::vector<edge_t> &edges);

    std::vector<edge_t> stoer_wagner(unsigned long n, const std::vector<edge_t> &edges);

}

#endif //MIN_CUT_MIN_CUT_DETERMINISTIC_H
