//
// Created by jeperez on 16-08-18.
//

#include "min-cut-probabilistic.h"
#include <iostream>
#include <random>

using namespace std;
using namespace boost;

namespace min_cut{
    //vector<ulong>  rank, parent;
    disjoint_sets<ulong*,ulong*> contract(unsigned long n, std::vector<edge_t> &edges, unsigned long t,
                                          vector<ulong> &rank, vector<ulong> &parent){
        for (ulong j = 0; j < n; ++j)
            parent[j]=j;
        disjoint_sets<ulong*,ulong*> ds(&rank[0], &parent[0]);

        // Initially there are V vertices in contracted graph
        auto vertices = n;

        // Keep contracting vertices until there are 2 vertices.
        while (vertices > t) {
            // Pick a random edge
            auto i = rand() % edges.size();

            // If two corners belong to same subset, then no point considering this edge
            if (ds.find_set(edges[i].first) != ds.find_set(edges[i].second)){
                // Contract the edge (or combine the corners of edge into one vertex)
                //cout << "Contracting edge " << edges[i].first << " - " << edges[i].second << endl;
                vertices--;
                ds.union_set(edges[i].first, edges[i].second);
            }

            edges.erase(edges.begin() + i);
        }

        return ds;
    }

    std::vector<edge_t> karger(unsigned long n, std::vector<edge_t> &edges){
        auto rank = vector<ulong>(n), parent = vector<ulong>(n);
        auto ds = contract(n, edges, 2, rank, parent);

        vector<edge_t> cut;
        for (auto e: edges) {
            if (ds.find_set(e.first) != ds.find_set(e.second))
                cut.push_back(e);
        }

        //cout << "karger-min-cut size: " << cut.size() << endl;
        return cut;
    }

    int karger_k = 5;
    void set_karger_k(int k){
        karger_k = k;
    }

    std::vector<edge_t> karger_iters(unsigned long n, const std::vector<edge_t> &edges){
        vector<edge_t> min_cut(edges.size());
        for (int j = 0; j < karger_k; ++j) {
            auto edges_aux = edges;
            auto cut = karger(n, edges_aux);
            if(cut.size() < min_cut.size())
                min_cut = cut;
        }
        //cout << "karger-iters-min-cut size: " << min_cut.size() << endl;
        return min_cut;
    }
}