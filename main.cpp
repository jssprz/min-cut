//
// Created by jeperez on 31-07-18.
//

#include "config.h"
#include <numeric>
#include <iostream>
#include <tuple>

#ifdef USE_EXPERIMENTS
#include "experiments/experiments.h"
#endif

#ifdef USE_DETERMINISTIC
#include "deterministic-solution/min-cut-deterministic.h"
#endif

#ifdef USE_PROBABILISTIC
#endif

#ifdef USE_HYBRID
#endif

#ifdef USE_EXPERIMENTS
using namespace experiments;
#endif

int main(){

#ifdef USE_EXPERIMENTS

#ifdef USE_DETERMINISTIC
#endif

#ifdef USE_PROBABILISTIC
#endif

#ifdef USE_HYBRID
#endif

#endif

    vector<int> V(100);
    std::iota(std::begin(V), std::end(V), 0);

    for (int i = 0; i < TRIALS; ++i) {
        auto G = random_graph<int>(V, 0.5);
        cout << "edges count: " << G.E.size() << endl;
    }

    min_cut_test<int>(&(min_cut::min_cut_deterministic<int>));

    return 0;
}