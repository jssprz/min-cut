//
// Created by Jesus on 8/2/2018.
//
#include "../graph.h"
#include <fstream>

namespace experiments{

    std::vector<min_cut::edge_t> random_graph(unsigned long n, double p);

    void min_cut_test(std::vector<min_cut::edge_t>(*min_cut_function)(unsigned long, const std::vector<min_cut::edge_t>&));

    void min_cut_test(std::vector<min_cut::edge_t>(*alg0)(unsigned long, const std::vector<min_cut::edge_t>&),
                      std::vector<min_cut::edge_t>(*alg1)(unsigned long, const std::vector<min_cut::edge_t>&),
                      std::vector<min_cut::edge_t>(*alg2)(unsigned long, const std::vector<min_cut::edge_t>&),
                      void(*set_k_alg1)(int k), void(*set_k_alg2)(int k), void(*set_t_alg2)(int t),
                      std::ofstream &report_file, std::string name_alg0, std::string name_alg1, std::string name_alg2);
}