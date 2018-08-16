//
// Created by jeperez on 16-08-18.
//

#include "min-cut-probabilistic.h"
#include <iostream>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;
using namespace boost;

namespace min_cut{
    std::vector<edge_t> karger(unsigned long n, std::vector<edge_t> edges){
        vector<ulong>  rank(n), parent(n);
        for (ulong j = 0; j < n; ++j) {
            parent[j]=j;
        }
        disjoint_sets<ulong*,ulong*> ds(&rank[0], &parent[0]);

        // Initially there are V vertices in
        // contracted graph
        int vertices = n;

        // Keep contracting vertices until there are
        // 2 vertices.
        while (vertices > 2) {
            // Pick a random edge
            int i = rand() % edges.size();

            // If two corners belong to same subset,
            // then no point considering this edge
            if (ds.find_set(edges[i].first) != ds.find_set(edges[i].second)){
                // Contract the edge (or combine the corners of edge into one vertex)
                cout << "Contracting edge " << edges[i].first << " - " << edges[i].second << endl;
                vertices--;
                ds.union_set(edges[i].first, edges[i].second);
            }
        }

        // Now we have two vertices (or subsets) left in
        // the contracted graph, so count the edges between
        // two components and return the count.
        vector<edge_t> cut;
        for (auto e: edges) {
            if (ds.find_set(e.first) != ds.find_set(e.second))
                cut.push_back(e);
        }

        cout << "karger-min-cut size: " << cut.size() << endl;
        return cut;

    }
}