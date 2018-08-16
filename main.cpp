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
using namespace min_cut;
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

//    for (int i = 0; i < TRIALS; ++i)
//        cout << "edges count: " << random_graph(100, 0.5).size() << endl;

//    min_cut_test(&(stoer_wagner));
//    min_cut_test(&(min_cut_max_flow));
    min_cut_test(&(min_cut_max_flow), &(stoer_wagner));

    return 0;
}