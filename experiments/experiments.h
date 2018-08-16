//
// Created by Jesus on 8/2/2018.
//
#include "../graph.h"
#include <vector>
#include <utility>
#include <random>
#include <numeric>

namespace experiments{

    std::vector<min_cut::edge_t> random_graph(unsigned long n, double p);

    void min_cut_test(std::vector<min_cut::edge_t>(*min_cut_function)(unsigned long, std::vector<min_cut::edge_t>));

    void min_cut_test(std::vector<min_cut::edge_t>(*min_cut_function1)(unsigned long, std::vector<min_cut::edge_t>),
                      std::vector<min_cut::edge_t>(*min_cut_function2)(unsigned long, std::vector<min_cut::edge_t>));
}