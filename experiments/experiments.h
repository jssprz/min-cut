//
// Created by Jesus on 8/2/2018.
//
#include "../graph.h"
#include <vector>
#include <utility>
#include <random>
#include <numeric>

using namespace std;
using namespace min_cut;

namespace experiments{

    template <typename T>
    Graph<T> random_graph(vector<T> V, double p){
        default_random_engine random_engine;
        bernoulli_distribution random_bool_generator(p);
        vector<min_cut::edge> E;
        for (size_t i = 0; i < V.size(); ++i) {
            for (size_t j = i+1; j < V.size(); ++j) {
                if(random_bool_generator(random_engine)) {
                    E.push_back(make_pair(i, j));
                }
            }
        }

        Graph<T> result;
        result.V = V;
        result.E = E;

        return result;
    }

    template <typename T>
    void min_cut_test(std::vector<min_cut::edge> (*min_cut_function)(Graph<T>)){
        vector<int> data(pow(2, V_MAX_POW_2));
        std::iota(std::begin(data), std::end(data), 0);

        for (int trial = 0; trial < TRIALS; ++trial) {
            int n = pow(2, V_MIN_POW_2);
            for (int j = V_MIN_POW_2; j < V_MAX_POW_2; ++j) {
                auto V = vector<int>(data.begin(), data.begin() + n);

                // get probabilities values
                double probs[5] = {0.2, 0.4, 0.6, 0.8, 1.0};
                for (auto p: probs) {
                    auto G = random_graph<int>(data, p);

                    auto cut = min_cut_function(G);
                };

                n = n << 1;
            }
        }
    }
}