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
    struct edge_t {
        unsigned long first;
        unsigned long second;
    };
}

#endif //MIN_CUT_GRAPH_H
