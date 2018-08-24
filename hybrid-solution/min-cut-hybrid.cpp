//
// Created by Jesus on 8/17/2018.
//

#include "min-cut-hybrid.h"
#include "../probabilistic-solution/min-cut-probabilistic.h"
#include "../deterministic-solution/min-cut-deterministic.h"
#include <iostream>
#include <algorithm>

using namespace std;

namespace min_cut{
    int k = 2;

    std::vector<edge_t> contracts(unsigned long n, std::vector<edge_t> edges, unsigned long t,
                                          vector<ulong> &rank, vector<ulong> &parent){
        disjoint_sets<ulong*,ulong*> ds(&rank[0], &parent[0]);

        // Initially there are V vertices in
        // contracted graph
        int vertices = n;

        // Keep contracting vertices until there are
        // 2 vertices.
        while (vertices > t) {
            // Pick a random edge
            int i = rand() % edges.size();

            // If two corners belong to same subset,
            // then no point considering this edge
            if (ds.find_set(edges[i].first) != ds.find_set(edges[i].second)){
                // Contract the edge (or combine the corners of edge into one vertex)
                //cout << "Contracting edge " << edges[i].first << " - " << edges[i].second << endl;
                vertices--;
                ds.union_set(edges[i].first, edges[i].second);
            }

            edges.erase(edges.begin()+i);
        }

        return edges;
    }

    std::vector<edge_t> karger_stein(unsigned long n, vector<edge_t> edges, vector<ulong> rank, vector<ulong> parent){
        if(n <= 6){
            disjoint_sets<ulong*,ulong*> ds(&rank[0], &parent[0]);
            vector<edge_t> edges1;
            vector<ulong> used_vertices;
            for(auto e : edges){
                auto v1 = ds.find_set(e.first), v2 = ds.find_set(e.second);
                if (v1 != v2) {
                    auto begin_it = used_vertices.begin(), end_it = used_vertices.end();
                    auto match_v1 = find(begin_it, end_it, v1), match_v2 = find(begin_it, end_it, v2);
                    edge_t new_edge = {match_v1 - begin_it, match_v2 - begin_it};
                    if(match_v1 == end_it) {
                        used_vertices.push_back(v1);
                        new_edge.first = used_vertices.size() - 1;
                    }
                    if(match_v2 == end_it) {
                        used_vertices.push_back(v2);
                        new_edge.second = used_vertices.size() - 1;
                    }
                    edges1.push_back(new_edge);
                }
            }

//            vector<edge_t> min_cut, fast_min_cut = min_cut_max_flow(n, edges1);
//            for(auto e: fast_min_cut){
//                min_cut.push_back({used_vertices[e.first], used_vertices[e.second]});
//            }
//            return min_cut;
            return min_cut_max_flow(n, edges1);
        }

        ulong t = 1 + n / sqrt(2);

        vector<edge_t> min_cut(edges.size());
        for (int j = 0; j < k; ++j) {
            auto new_rank = rank, new_parent = parent;
            auto new_edges = contracts(n, edges, t + 1, new_rank, new_parent);
            auto cut = karger_stein(t + 1, new_edges, new_rank, new_parent);
            if(cut.size() < min_cut.size())
                min_cut = cut;
        }

        return min_cut;
    }

    std::vector<edge_t> karger_stein(unsigned long n, const std::vector<edge_t> &edges){
        auto rank = vector<ulong>(n), parent = vector<ulong>(n);
        for (ulong j = 0; j < n; ++j)
            parent[j]=j;
        auto min_cut = karger_stein(n, edges, rank, parent);
        cout << "karger-stain-min-cut size: " << min_cut.size() << endl;
        return min_cut;
    }
}