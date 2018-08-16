//
// Created by Jesus on 8/15/2018.
//

#include "experiments.h"
#include "config.h"
#include "../graph.h"
#include <iostream>

using namespace std;
using namespace min_cut;

namespace experiments{
    std::vector<min_cut::edge_t> random_graph(unsigned long n, double p){
        default_random_engine random_engine;
        bernoulli_distribution random_bool_generator(p);
        vector<min_cut::edge_t> result;
        for (size_t i = 0; i < n-1; ++i) {
            result.push_back({i, i+1});
            for (size_t j = i+2; j < n; ++j) {
                if(random_bool_generator(random_engine)) {
                    result.push_back({i, j});
                }
            }
        }

        return result;
    }

    void min_cut_test(std::vector<edge_t>(*min_cut_function)(unsigned long, std::vector<edge_t>)){
        for (int trial = 0; trial < TRIALS; ++trial) {
            unsigned long n = 10;
            for (int j = V_MIN_POW_2; j < V_MAX_POW_2; ++j) {
                // get probabilities values
                double probs[5] = {0.2, 0.4, 0.6, 0.8, 1.0};
                for (auto p: probs) {
                    auto G = random_graph(n, p);
                    auto cut = min_cut_function(n, G);
                    char c;
                    cin >> c;
                };

                n = n << 1;
            }
        }
    }

    void min_cut_test(std::vector<edge_t>(*min_cut_function1)(unsigned long, std::vector<edge_t>),
            std::vector<edge_t>(*min_cut_function2)(unsigned long, std::vector<edge_t>)){
        for (int trial = 0; trial < TRIALS; ++trial) {
            unsigned long n = 10;
            for (int j = V_MIN_POW_2; j < V_MAX_POW_2; ++j) {
                // get probabilities values
                double probs[5] = {0.2, 0.4, 0.6, 0.8, 1.0};
                for (auto p: probs) {
                    auto G = random_graph(n, p);
                    if(min_cut_function1(n, G).size() == min_cut_function2(n, G).size())
                        cout<<"OK"<<endl;
                    else
                        cout<<"WRONG"<<endl;
                };

                n = n << 1;
            }
        }
    }
}