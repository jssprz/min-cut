//
// Created by Jesus on 8/2/2018.
//

#ifndef MIN_CUT_MIN_CUT_PROBABILISTIC_H
#define MIN_CUT_MIN_CUT_PROBABILISTIC_H

#include "../graph.h"
#include <boost/pending/disjoint_sets.hpp>

namespace min_cut{
    disjoint_sets<ulong*,ulong*> contract(unsigned long n, const std::vector<edge_t> &edges, unsigned long t,
                                          std::vector<ulong> &rank, std::vector<ulong> &parent);

    std::vector<edge_t> karger(unsigned long n, const std::vector<edge_t> &edges);

    std::vector<edge_t> karger_iters(unsigned long n, const std::vector<edge_t> &edges);
}

#endif //MIN_CUT_MIN_CUT_PROBABILISTIC_H
