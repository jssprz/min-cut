//
// Created by Jesus on 8/17/2018.
//

#include "min-cut-hybrid.h"
#include "../deterministic-solution/min-cut-deterministic.h"
#include "../probabilistic-solution/min-cut-probabilistic.h"
#include <iostream>
#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

namespace min_cut{
    void contracts(unsigned long n, const std::vector<edge_t> &edges, unsigned long t,
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
        }
    }

    int hybrid_t = 6;
    void set_hybrid_t(int t){
        hybrid_t = t;
    }

    std::vector<edge_t> karger_stein(unsigned long n, const vector<edge_t> &edges, vector<ulong> &rank, vector<ulong> &parent){
        if(n <= hybrid_t){
            disjoint_sets<ulong*,ulong*> ds(&rank[0], &parent[0]);
            vector<edge_t> edges1;
            vector<ulong> vertices_map(parent.size(), ULONG_MAX);
            ulong index = 0;
            for(auto e : edges){
                auto v1 = ds.find_set(e.first), v2 = ds.find_set(e.second);
                if (v1 != v2) {
                    //auto begin_it = used_vertices.begin(), end_it = used_vertices.end();
                    //auto match_v1 = find(begin_it, end_it, v1), match_v2 = find(begin_it, end_it, v2);
                    //edge_t new_edge = {ulong(match_v1 - begin_it), ulong(match_v2 - begin_it)};
                    edge_t new_edge = {vertices_map[v1], vertices_map[v2]};
                    if(vertices_map[v1] == ULONG_MAX)
                        new_edge.first = vertices_map[v1] = index++;
                    if(vertices_map[v2] == ULONG_MAX)
                        new_edge.second = vertices_map[v2] = index++;
                    edges1.push_back(new_edge);
                }
            }

            //is easy to obtain the edges of the original graph using the used_vertices vector
            return min_cut_max_flow(n, edges1);
        }

        auto t = ulong(1 + n / 1.414);

        auto new_rank = rank, new_parent = parent;
        contracts(n, edges, t, new_rank, new_parent);
        auto cut1 = karger_stein(t, edges, new_rank, new_parent);

        new_rank = rank, new_parent = parent;
        contracts(n, edges, t, new_rank, new_parent);
        auto cut2 = karger_stein(t, edges, new_rank, new_parent);

        return cut1.size() <= cut2.size() ? cut1 : cut2;
    }

    std::vector<edge_t> karger_stein(unsigned long n, const std::vector<edge_t> &edges){
        auto rank = vector<ulong>(n), parent = vector<ulong>(n);
        for (ulong j = 0; j < n; ++j)
            parent[j]=j;
        auto min_cut = karger_stein(n, edges, rank, parent);
        //cout << "karger-stain-min-cut size: " << min_cut.size() << endl;
        return min_cut;
    }

    std::vector<edge_t> hybrid(unsigned long n, std::vector<edge_t> &edges){
        auto rank = vector<ulong>(n), parent = vector<ulong>(n);
        auto ds = contract(n, edges, hybrid_t, rank, parent);

        vector<edge_t> edges1;
        vector<ulong> vertices_map(parent.size(), ULONG_MAX);
        ulong index = 0;
        for(auto e : edges){
            auto v1 = ds.find_set(e.first), v2 = ds.find_set(e.second);
            if (v1 != v2) {
                edge_t new_edge = {vertices_map[v1], vertices_map[v2]};
                if(vertices_map[v1] == ULONG_MAX)
                    new_edge.first = vertices_map[v1] = index++;
                if(vertices_map[v2] == ULONG_MAX)
                    new_edge.second = vertices_map[v2] = index++;
                edges1.push_back(new_edge);
            }
        }

        return min_cut_max_flow(index, edges1);
    }



    int hybrid_k = 5;
    void set_hybrid_k(int k){
        hybrid_k = k;
    }

    std::vector<edge_t> hybrid_iters(unsigned long n, const std::vector<edge_t> &edges){
        vector<edge_t> min_cut(edges.size());
        for (int k = 0; k < hybrid_k; ++k) {
            auto edges_aux = edges;
            auto cut = hybrid(n, edges_aux);
            if (cut.size() < min_cut.size())
                min_cut = cut;
        }
        return min_cut;
    }

    std::vector<edge_t> hybrid_iters2(unsigned long n, const std::vector<edge_t> &edges){
        auto rank = vector<ulong>(n), parent = vector<ulong>(n);

        auto min_contraction = vector<edge_t>(edges.size());

        for (int j = 0; j < hybrid_k; ++j) {
            auto edges_aux = edges;
            auto ds = contract(n, edges_aux, hybrid_t, rank, parent);
            vector<edge_t> edges1;
            vector<ulong> vertices_map(parent.size(), ULONG_MAX);
            ulong index = 0;
            for (auto e : edges) {
                auto v1 = ds.find_set(e.first), v2 = ds.find_set(e.second);
                if (v1 != v2) {
                    edge_t new_edge = {vertices_map[v1], vertices_map[v2]};
                    if (vertices_map[v1] == ULONG_MAX)
                        new_edge.first = vertices_map[v1] = index++;
                    if (vertices_map[v2] == ULONG_MAX)
                        new_edge.second = vertices_map[v2] = index++;
                    edges1.push_back(new_edge);
                }
            }
            if (edges1.size() < min_contraction.size())
                min_contraction = edges1;
        }

        return min_cut_max_flow(hybrid_t, min_contraction);
    }
}