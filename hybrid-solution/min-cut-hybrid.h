//
// Created by Jesus on 8/2/2018.
//

#ifndef MIN_CUT_MIN_CUT_HYBRID_H
#define MIN_CUT_MIN_CUT_HYBRID_H

#include "../graph.h"

namespace min_cut{
    std::vector<edge_t> karger_stein(unsigned long n, const std::vector<edge_t> &edges);
}

#endif //MIN_CUT_MIN_CUT_HYBRID_H
