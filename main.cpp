//
// Created by jeperez on 31-07-18.
//

#include "config.h"
#include <numeric>
#include <iostream>
#include <tuple>
#include <string>
#include <iomanip>      // std::setprecision

#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "experiments/experiments.h"
#include "deterministic-solution/min-cut-deterministic.h"
#include "probabilistic-solution/min-cut-probabilistic.h"
#include "hybrid-solution/min-cut-hybrid.h"

using namespace min_cut;
using namespace experiments;
using namespace std;

int main(){

    try {
        string strPath = "tests_results";

#ifdef _WIN32
        const int dir_err = _mkdir(strPath.c_str());
#else
        const int dir_err = mkdir(strPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

        if (dir_err != 0 && dir_err != -1 && dir_err != EEXIST)
            throw string("error creating 'tests_results' folder");
    }
    catch (string e){
        cout << e << endl;
    }

    auto report_file = std::ofstream("tests_results/results.report", std::ios::trunc);
    report_file << fixed << setprecision(5);

    min_cut_test(&(min_cut_max_flow), &(karger_iters), &(hybrid_iters2), &(set_karger_k), &(set_hybrid_k),
                 &(set_hybrid_t), report_file, "min-cut-max-flow", "karger-iters", "hybrid");
    report_file << "*************" << endl;

    return 0;
}