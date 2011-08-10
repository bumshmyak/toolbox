#ifndef _TOOLBOX_GRAPH_MAXFLOW_H_
#define _TOOLBOX_GRAPH_MAXFLOW_H_

#include <graph/common.h>
#include <vector>
#include <utility>

// O(n * m^2)
int edmondson_karp(const Graph& graph,
                   int source,
                   int destination,
                   Graph& flow);

// O(n^3)
int relabel_to_front(const Graph& graph,
                     int source,
                     int destination,
                     Graph& flow);

// O(n^2 * m)
int blocking_flows(const Graph& graph,
                   int source,
                   int destination,
                   Graph& flow);

// Each part has graph.size() vertices
int get_max_bipartite_matching(const Graph& graph);

int get_max_bipartite_matching_simply(
    int left_part_size,
    int right_part_size,
    const std::vector< std::pair<int, int> >& arcs);

#endif  // _TOOLBOX_GRAPH_MAXFLOW_H_
