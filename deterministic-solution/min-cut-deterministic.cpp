//
// Created by Jesus on 8/15/2018.
//

#include "min-cut-deterministic.h"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/one_bit_color_map.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/typeof/typeof.hpp>

using namespace std;
using namespace boost;

namespace min_cut {
    typedef int EdgeWeightType;

    typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
    typedef adjacency_list<vecS, vecS, directedS,
            property<vertex_name_t, std::string,
                    property<vertex_index_t, long,
                            property<vertex_color_t, boost::default_color_type,
                                    property<vertex_distance_t, long,
                                            property<vertex_predecessor_t, Traits::edge_descriptor> > > > >,
            property<edge_capacity_t, EdgeWeightType,
                    property<edge_residual_capacity_t, EdgeWeightType,
                            property<edge_reverse_t, Traits::edge_descriptor> > > > ResidualGraph;

    Traits::edge_descriptor add_reverse_edge(Traits::vertex_descriptor &v1, Traits::vertex_descriptor &v2,
                                             property_map<ResidualGraph, edge_reverse_t>::type &rev,
                                             const double capacity,
                                             ResidualGraph &g) {
        Traits::edge_descriptor e1 = add_edge(v1, v2, g).first;
        Traits::edge_descriptor e2 = add_edge(v2, v1, g).first;
        put(edge_capacity, g, e1, capacity);
        put(edge_capacity, g, e2, capacity);

        rev[e1] = e2;
        rev[e2] = e1;
    }

    std::vector<edge_t> min_cut_max_flow(unsigned long n, const std::vector<edge_t> &edges) {
        ResidualGraph g;

        property_map<ResidualGraph, edge_reverse_t>::type rev = get(edge_reverse, g);

        auto V = vector<Traits::vertex_descriptor>(n);
        for (ulong j = 0; j < n; ++j) {
            V[j] = add_vertex(g);
        }
        for (auto e:edges) {
            add_reverse_edge(V[e.first], V[e.second], rev, 1, g);
            //add_reverse_edge(V[e.second], V[e.first], rev, 1, g);
        }
        std::vector<default_color_type> color(n);
        std::vector<Traits::edge_descriptor> pred(n);

        vector<edge_t> cut;
        EdgeWeightType max_flow = 1410065408, flow;
        auto s = 0, t = 1;

        for(ulong t = 1; t < n; ++t){
            //find min cut, a list of sources will be returned in s, and a list of sinks will be returned in t
//            flow = push_relabel_max_flow(g, V[s], V[t], color_map(&color[0]).predecessor_map(&pred[0]));
//            flow = edmonds_karp_max_flow(g, V[s], V[t], color_map(&color[0]).predecessor_map(&pred[0]));
            flow = boykov_kolmogorov_max_flow(g, V[s], V[t], color_map(&color[0]).predecessor_map(&pred[0]));

            if(flow < max_flow) {
                cut = vector<edge_t>();
                for (auto e : edges) {
                    if (color[e.first] == white_color && color[e.second] != white_color ||
                        color[e.first] != white_color && color[e.second] == white_color) {
                        //cout << "{" << e.first << "," << e.second << "}\n";
                        cut.push_back(e);
                    }
                }
//                cout << "Max flow is: " << flow << endl;
                max_flow = flow;
            }
        }

//        cout<<"max-flow-cut size: " <<cut.size()<<endl;
        return cut;
    }

    std::vector<edge_t> stoer_wagner(unsigned long n, const std::vector<edge_t> &edges){
        typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > UndirectedGraph;
        typedef graph_traits<UndirectedGraph>::vertex_descriptor vertex_descriptor;
        typedef property_map<UndirectedGraph, edge_weight_t>::type weight_map_type;
        typedef property_traits<weight_map_type>::value_type weight_type;

        // for each edge, define the associated edge weight=1
        auto ws = vector<weight_type>(edges.size(), 1);

        // construct the graph object. n is the number of vertices, which are numbered from 0 through n-1
        UndirectedGraph g(edges.begin(), edges.end(), ws.begin(), n, edges.size());

        // define a property map, `parities`, that will store a boolean value for each vertex.
        // Vertices that have the same parity after `stoer_wagner_min_cut` runs are on the same side of the min-cut.
        BOOST_AUTO(parities, make_one_bit_color_map(num_vertices(g), get(vertex_index, g)));

        // run the Stoer-Wagner algorithm to obtain the min-cut weight. `parities` is also filled in.
        int w = stoer_wagner_min_cut(g, get(edge_weight, g), parity_map(parities));

        cout << "min-cut weight: " << w << endl;

        vector<edge_t> cut;
        cut.reserve(w);
        for (auto e : edges) {
            if (get(parities, e.first)!=get(parities, e.second)) {
                //cout << "{" << e.first << "," << e.second << "}\n";
                cut.push_back(e);
            }
        }

        cout<<"stoer_wagner-cut size:" <<cut.size()<<endl;
        return cut;
    }
}