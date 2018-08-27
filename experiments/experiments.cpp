//
// Created by Jesus on 8/15/2018.
//

#include "experiments.h"
#include "timer.h"
#include "config.h"
#include "../graph.h"
#include <iostream>
#include <fstream>
#include <random>

using namespace std;
using namespace min_cut;

DECLARE_TIMING(t1);
DECLARE_TIMING(t2);
DECLARE_TIMING(t3);

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

    void min_cut_test(std::vector<edge_t>(*min_cut_function)(unsigned long, const std::vector<edge_t>&)){
        for (int trial = 0; trial < TRIALS; ++trial) {
            unsigned long n = 10;
            for (int j = V_MIN_POW_2; j <= V_MAX_POW_2; ++j) {
                // get probabilities values
                double probs[5] = {0.1, 0.15, 0.2, 0.25, 0.3};
                for (auto p: probs) {
                    auto G = random_graph(n, p);
                    auto cut = min_cut_function(n, G);
//                    char c;
//                    cin >> c;
                };

                n = n << 1;
            }
        }
    }

    void min_cut_test(std::vector<min_cut::edge_t>(*alg0)(unsigned long, const std::vector<min_cut::edge_t>&),
                      std::vector<min_cut::edge_t>(*alg1)(unsigned long, const std::vector<min_cut::edge_t>&),
                      std::vector<min_cut::edge_t>(*alg2)(unsigned long, const std::vector<min_cut::edge_t>&),
                      void(*set_k_alg1)(int k), void(*set_k_alg2)(int k), void(*set_t_alg2)(int t),
                      std::ofstream &report_file, string name_alg0, string name_alg1, string name_alg2){
        
        double alg0_time_results[TRIALS][V_MAX_POW_2-V_MIN_POW_2+1][5];
        double alg1_time_results[TRIALS][V_MAX_POW_2-V_MIN_POW_2+1][5][5];
        double alg2_time_results[TRIALS][V_MAX_POW_2-V_MIN_POW_2+1][5][5][5];
        fill((double*)alg1_time_results,(double*)alg1_time_results+sizeof(alg1_time_results)/sizeof(double),-1);
        fill((double*)alg2_time_results,(double*)alg2_time_results+sizeof(alg2_time_results)/sizeof(double),-1);

        int alg1_ok_results[TRIALS][V_MAX_POW_2-V_MIN_POW_2+1][5][5];
        int alg2_ok_results[TRIALS][V_MAX_POW_2-V_MIN_POW_2+1][5][5][5];
        fill((int*)alg1_ok_results,(int*)alg1_ok_results+sizeof(alg1_ok_results)/sizeof(int),0);
        fill((int*)alg2_ok_results,(int*)alg2_ok_results+sizeof(alg2_ok_results)/sizeof(int),0);

        string names[3] = {name_alg0, name_alg1, name_alg2};

        srand(TRIALS);

        for (int trial = 0; trial < TRIALS; ++trial) {
            cout << "TRIAL " << trial << endl;
            auto n = ulong(pow(2, V_MIN_POW_2));
            for (int j = V_MIN_POW_2; j <= V_MAX_POW_2; ++j) {
                cout << "testing V=2^" << j << endl;
                // get probabilities values
                auto step = (1 - 1.0 / n) / 4.0;
                //double probs[5] = {1.0/n, 1.0/n + step, 1.0/n + 2*step, 1.0/n + 3*step, 1.0};
                //double probs[5] = {1.0/n, 1.0/(.8*n), 1.0/(.6*n), 1.0/(.4*n), 1.0/(.2*n)};
                double probs[5] = {1.0 / n, 3.0 / n, 5.0 / n, 7.0 / n, 9.0 / n};
                //double probs[5] = {1.0/n, 2.0/n, 4.0/n, 8.0/n, 16.0/n};
                for (int p = 0; p < 5; ++p) {
                    cout << "\tp=" << probs[p] << endl;
                    cout << "\t\tgenerating graph..." << endl;
                    auto G = random_graph(n, probs[p]);

                    START_TIMING(t1);
                    cout << "\t\trunning " << name_alg0 << "...";
                    auto cut0 = alg0(n, G);
                    STOP_TIMING(t1);
                    alg0_time_results[trial][j - V_MIN_POW_2][p] = GET_TIMING(t1);
                    cout << "\t\tmin-cut size: " << cut0.size() << endl;

                    ulong k_values[5] = {n / 1024, n / 512, n / 256, n / 128,
                                         n / 64 /*,n*(n-1)/2, ulong((n*(n-1)/2)*log(n))*/};
                    ulong t_values[5] = {3, 3 + (n - 4) / 1024, 3 + (n - 4) / 512, 3 + (n - 4) / 256, 3 + (n - 4) / 128};

                    bool alg1_ok = false, alg2_ok = false;
                    for (int k = 0; k < 5; ++k) {
                        if(!alg1_ok) {
                            START_TIMING(t2);
                            cout << "\t\trunning " << name_alg1 << "_k=" << k << "...";
                            set_k_alg1(k_values[k]);
                            auto cut1 = alg1(n, G);
                            STOP_TIMING(t2);
                            alg1_time_results[trial][j - V_MIN_POW_2][p][k] = GET_TIMING(t2);
                            alg1_ok_results[trial][j - V_MIN_POW_2][p][k] = int(cut1.size() - cut0.size());

                            if (cut1.size() == cut0.size()) {
                                cout << "\t\tOK" << endl;
                                alg1_ok=true;
                            } else
                                cout << "\t\tWRONG" << endl;
                        }

                        for (int t = 0; t < 5; ++t) {
                            if(!alg2_ok) {
                                START_TIMING(t3);
                                cout << "\t\trunning " << name_alg2 << " k_" << k << " t_" << t << "...";
                                set_k_alg2(k_values[k]);
                                set_t_alg2(t_values[t]);
                                auto cut2 = alg2(n, G);
                                STOP_TIMING(t3);
                                alg2_time_results[trial][j - V_MIN_POW_2][p][k][t] = GET_TIMING(t3);
                                alg2_ok_results[trial][j - V_MIN_POW_2][p][k][t] = int(cut2.size() - cut0.size());

                                if (cut2.size() == cut0.size()) {
                                    cout << "\t\tOK" << endl;
                                    alg2_ok=true;
                                } else
                                    cout << "\t\tWRONG" << endl;
                            }
                        }
                    }
                }

                n = n << 1;
            }

            report_file << "TRIAL " << trial << endl;
            report_file << "\tTIME RESULTS" << endl;

            report_file << "\t\tMETHOD " << names[0] << " results:" << endl;
            for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                report_file << "\t\t2^" << k << "\t";
                for (int j = 0; j < 5; ++j)
                    report_file << alg0_time_results[trial][k - V_MIN_POW_2][j] << "\t\t";
                report_file << endl;
            }
            report_file << endl;

            for (int m = 0; m < 5; ++m) {
                report_file << "\t\tMETHOD " << names[1] << " k_" << m << " results:" << endl;
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    report_file << "\t\t2^" << k << "\t";
                    for (int j = 0; j < 5; ++j)
                        report_file << alg1_time_results[trial][k - V_MIN_POW_2][j][m] << "\t\t";
                    report_file << endl;
                }
                report_file << endl;
            }

            for (int m = 0; m < 5; ++m) {
                for (int t = 0; t < 5; ++t) {
                    report_file << "\t\tMETHOD " << names[2] << " k_" << m << " t_" << t << " results:" << endl;
                    for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                        report_file << "\t\t2^" << k << "\t";
                        for (int j = 0; j < 5; ++j)
                            report_file << alg2_time_results[trial][k - V_MIN_POW_2][j][m][t] << "\t\t";
                        report_file << endl;
                    }
                    report_file << endl;
                }
            }

            report_file << "\tCORRECTENESS" << endl;

            for (int m = 0; m < 5; ++m) {
                report_file << "\t\tMETHOD " << names[1] << " k_" << m << " results:" << endl;
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    report_file << "\t\t2^" << k << "\t";
                    for (int j = 0; j < 5; ++j)
                        report_file << alg1_ok_results[trial][k - V_MIN_POW_2][j][m] << "\t\t";
                    report_file << endl;
                }
                report_file << endl;
            }

            for (int m = 0; m < 5; ++m) {
                for (int t = 0; t < 5; ++t) {
                    report_file << "\t\tMETHOD " << names[2] << " k_" << m << " t_" << t << " results:" << endl;
                    for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                        report_file << "\t\t2^" << k << "\t";
                        for (int j = 0; j < 5; ++j)
                            report_file << alg2_ok_results[trial][k - V_MIN_POW_2][j][m][t] << "\t\t";
                        report_file << endl;
                    }
                    report_file << endl;
                }
            }
        }

        report_file << "AVERAGE RESULTS" << endl;
        report_file << "\tTIME RESULTS" << endl;
        
        double alg0_time_totals[V_MAX_POW_2-V_MIN_POW_2+1][5], alg0_time_sd[V_MAX_POW_2-V_MIN_POW_2+1][5];
        fill((double*)alg0_time_totals,(double*)alg0_time_totals+sizeof(alg0_time_totals)/sizeof(double),0);
        fill((double*)alg0_time_sd,(double*)alg0_time_sd+sizeof(alg0_time_sd)/sizeof(double),0);
        
        for (int m = 0; m < TRIALS; ++m) {
            for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                for (int j = 0; j < 5; ++j)
                    alg0_time_totals[k - V_MIN_POW_2][j] += alg0_time_results[m][k - V_MIN_POW_2][j];
            }
        }
        
        for (int m = 0; m < TRIALS; ++m) {
            for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                for (int j = 0; j < 5; ++j) {
                    auto avg = alg0_time_totals[k - V_MIN_POW_2][j] / TRIALS;
                    alg0_time_sd[k - V_MIN_POW_2][j] = (alg0_time_results[m][k - V_MIN_POW_2][j] - avg) *
                                                      (alg0_time_results[m][k - V_MIN_POW_2][j] - avg);
                }
            }
        }
        
        report_file << "\t\tMETHOD " << names[0] << " results:" << endl;
        for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
            report_file << "\t\t2^" << k << "\t";
            for (int j = 0; j < 5; ++j)
                report_file << alg0_time_totals[k - V_MIN_POW_2][j] / TRIALS << "(+-"
                            << sqrt(alg0_time_sd[k - V_MIN_POW_2][j] / TRIALS) << ")\t\t";
            report_file << endl;
        }
        report_file << endl;

        double alg1_time_totals[V_MAX_POW_2-V_MIN_POW_2+1][5][5], alg1_time_sd[V_MAX_POW_2-V_MIN_POW_2+1][5][5];
        fill((double*)alg1_time_totals,(double*)alg1_time_totals+sizeof(alg1_time_totals)/sizeof(double),0);

        for (int m = 0; m < TRIALS; ++m) {
            for (int kk = 0; kk < 5; ++kk) {
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    for (int j = 0; j < 5; ++j) {
                        if (alg1_time_results[m][k - V_MIN_POW_2][j][kk] >= 0)
                            alg1_time_totals[k - V_MIN_POW_2][j][kk] += alg1_time_results[m][k - V_MIN_POW_2][j][kk];
                    }
                }
            }
        }

        for (int m = 0; m < TRIALS; ++m) {
            for (int kk = 0; kk < 5; ++kk) {
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    for (int j = 0; j < 5; ++j) {
                        auto avg = alg1_time_totals[k - V_MIN_POW_2][j][kk] / TRIALS;
                        if (alg1_time_results[m][k - V_MIN_POW_2][j][kk] >= 0)
                            alg1_time_sd[k - V_MIN_POW_2][j][kk] =
                                    (alg1_time_results[m][k - V_MIN_POW_2][j][kk] - avg) *
                                    (alg1_time_results[m][k - V_MIN_POW_2][j][kk] - avg);
                    }
                }
            }
        }

        for (int kk = 0; kk < 5; ++kk) {
            report_file << "\t\tMETHOD " << names[1] << " k_" << kk << " results:" << endl;
            for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                report_file << "\t\t2^" << k << "\t";
                for (int j = 0; j < 5; ++j)
                    report_file << alg1_time_totals[k - V_MIN_POW_2][j][kk] / TRIALS << "(+-"
                                << alg1_time_sd[k - V_MIN_POW_2][j][kk] << ")\t";
                report_file << endl;
            }
            report_file << endl;
        }

        double alg2_time_totals[V_MAX_POW_2-V_MIN_POW_2+1][5][5][5], alg2_time_sd[V_MAX_POW_2-V_MIN_POW_2+1][5][5][5];
        fill((double*)alg2_time_totals,(double*)alg2_time_totals+sizeof(alg2_time_totals)/sizeof(double),0);

        for (int m = 0; m < TRIALS; ++m) {
            for (int kk = 0; kk < 5; ++kk) {
                for (int t = 0; t < 5; ++t) {
                    for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                        for (int j = 0; j < 5; ++j) {
                            if (alg2_time_results[m][k - V_MIN_POW_2][j][kk][t] >= 0)
                                alg2_time_totals[k - V_MIN_POW_2][j][kk][t] += alg2_time_results[m][k -
                                                                                                 V_MIN_POW_2][j][kk][t];
                        }
                    }
                }
            }
        }

        for (int m = 0; m < TRIALS; ++m) {
            for (int kk = 0; kk < 5; ++kk) {
                for (int t = 0; t < 5; ++t) {
                    for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                        for (int j = 0; j < 5; ++j) {
                            auto avg = alg2_time_totals[k - V_MIN_POW_2][j][kk][t] / TRIALS;
                            if (alg2_time_results[m][k - V_MIN_POW_2][j][kk][t] >= 0)
                                alg2_time_sd[k - V_MIN_POW_2][j][kk][t] =
                                        (alg2_time_results[m][k - V_MIN_POW_2][j][kk][t] - avg) *
                                        (alg2_time_results[m][k - V_MIN_POW_2][j][kk][t] - avg);
                        }
                    }
                }
            }
        }

        for (int kk = 0; kk < 5; ++kk) {
            for (int t = 0; t < 5; ++t) {
                report_file << "\t\tMETHOD " << names[1] << " k_" << kk << " t_" << t << " results:" << endl;
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    report_file << "\t\t2^" << k << "\t";
                    for (int j = 0; j < 5; ++j)
                        report_file << alg2_time_totals[k - V_MIN_POW_2][j][kk][t] / TRIALS << "(+-"
                                    << alg2_time_sd[k - V_MIN_POW_2][j][kk][t] << ")\t";
                    report_file << endl;
                }
                report_file << endl;
            }
        }

        report_file << "\tCORRECTENESS RESULTS" << endl;

        double alg1_ok_totals[V_MAX_POW_2-V_MIN_POW_2+1][5][5], alg1_ok_sd[V_MAX_POW_2-V_MIN_POW_2+1][5][5];
        fill((double*)alg1_ok_totals,(double*)alg1_ok_totals+sizeof(alg1_ok_totals)/sizeof(double),0);

        for (int m = 0; m < TRIALS; ++m) {
            for (int kk = 0; kk < 5; ++kk) {
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    for (int j = 0; j < 5; ++j)
                        alg1_ok_totals[k - V_MIN_POW_2][j][kk] += alg1_ok_results[m][k - V_MIN_POW_2][j][kk];
                }
            }
        }

        for (int m = 0; m < TRIALS; ++m) {
            for (int kk = 0; kk < 5; ++kk) {
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    for (int j = 0; j < 5; ++j) {
                        auto avg = alg1_ok_totals[k - V_MIN_POW_2][j][kk] / TRIALS;
                        alg1_ok_sd[k - V_MIN_POW_2][j][kk] = (alg1_ok_results[m][k - V_MIN_POW_2][j][kk] - avg) *
                                                       (alg1_ok_results[m][k - V_MIN_POW_2][j][kk] - avg);
                    }
                }
            }
        }

        for (int kk = 0; kk < 5; ++kk) {
            report_file << "\t\tMETHOD " << names[1] << " k_" << kk << " results:" << endl;
            for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                report_file << "\t\t2^" << k << "\t";
                for (int j = 0; j < 5; ++j)
                    report_file << alg1_ok_totals[k - V_MIN_POW_2][j][kk] / TRIALS << "(+-"
                                << alg1_ok_sd[k - V_MIN_POW_2][j][kk] << ")\t";
                report_file << endl;
            }
            report_file << endl;
        }

        double alg2_ok_totals[V_MAX_POW_2-V_MIN_POW_2+1][5][5][5], alg2_ok_sd[V_MAX_POW_2-V_MIN_POW_2+1][5][5][5];
        fill((double*)alg2_ok_totals,(double*)alg2_ok_totals+sizeof(alg2_ok_totals)/sizeof(double),0);

        for (int m = 0; m < TRIALS; ++m) {
            for (int kk = 0; kk < 5; ++kk) {
                for (int t = 0; t < 5; ++t) {
                    for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                        for (int j = 0; j < 5; ++j)
                            alg2_ok_totals[k - V_MIN_POW_2][j][kk][t] += alg2_ok_results[m][k - V_MIN_POW_2][j][kk][t];
                    }
                }
            }
        }

        for (int m = 0; m < TRIALS; ++m) {
            for (int kk = 0; kk < 5; ++kk) {
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    for (int t = 0; t < 5; ++t) {
                        for (int j = 0; j < 5; ++j) {
                            auto avg = alg2_ok_totals[k - V_MIN_POW_2][j][kk][t] / TRIALS;
                            alg2_ok_sd[k - V_MIN_POW_2][j][kk][t] =
                                    (alg2_ok_results[m][k - V_MIN_POW_2][j][kk][t] - avg) *
                                    (alg2_ok_results[m][k - V_MIN_POW_2][j][kk][t] - avg);
                        }
                    }
                }
            }
        }

        for (int kk = 0; kk < 5; ++kk) {
            for (int t = 0; t < 5; ++t) {
                report_file << "\t\tMETHOD " << names[1] << " k_" << kk << " t_" << t << " results:" << endl;
                for (int k = V_MIN_POW_2; k <= V_MAX_POW_2; ++k) {
                    report_file << "\t\t2^" << k << "\t";
                    for (int j = 0; j < 5; ++j)
                        report_file << alg2_ok_totals[k - V_MIN_POW_2][j][kk][t] / TRIALS << "(+-"
                                    << alg2_ok_sd[k - V_MIN_POW_2][j][kk][t] << ")\t";
                    report_file << endl;
                }
                report_file << endl;
            }
        }
    }
}