#ifndef _TOOLBOX_GRAPH_SSSPP_H_
#define _TOOLBOX_GRAPH_SSSPP_H_

#include <vector>
#include "graph/common.h"

// shortest paths from source to all other vertices
void ford_bellman(const Graph& graph,
                  int source,
                  std::vector<int>& shortest_paths);
void ford_bellman_on_queue(const Graph& graph,
                           int source,
                           std::vector<int>& shortest_paths);
void dijkstra_on_array(const Graph& graph,
                       int source,
                       std::vector<int>& shortest_paths);
void dijkstra_on_priority_queue(const Graph& graph,
                                int source,
                                std::vector<int>& shortest_paths);
void dijkstra_on_set(const Graph& graph,
                     int source,
                     std::vector<int>& shortest_paths);
template <int K>
void dijkstra_on_kary_heap(const Graph& graph,
                           int source,
                           std::vector<int>& shortest_paths);

// Generic case: negative cycles alowed, returns false if
// negative cycle exists
bool tarjan_ssspp(const Graph& graph,
                  int source,
                  std::vector<int>& shortest_paths);

// shortest path between specified pair of vertices
int bidirected_dijkstra(const Graph& graph, int source, int destination);
int ford_bellman(const Graph& graph, int source, int destination);
int ford_bellman_on_queue(const Graph& graph, int source, int destination);
int dijkstra_on_array(const Graph& graph, int source, int destination);
int dijkstra_on_priority_queue(const Graph& graph, int source, int destination);
int dijkstra_on_set(const Graph& graph, int source, int destination);
template <int K>
int dijkstra_on_kary_heap(const Graph& graph, int source, int destination);


#include "dijkstra-inl.h" // NOLINT

#endif  // _TOOLBOX_GRAPH_SSSPP_H_
