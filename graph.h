//
// Created by Jesus on 8/2/2018.
//

#ifndef MIN_CUT_GRAPH_H
#define MIN_CUT_GRAPH_H

#include <vector>
#include <utility>

namespace min_cut{
    template <typename T>
    struct Graph{
        std::vector<T> V;
        std::vector<std::pair<int, int>> E;
    };
}

#endif //MIN_CUT_GRAPH_H
