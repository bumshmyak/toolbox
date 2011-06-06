#include <vector>
#include <algorithm>

#include "graph/common.h"
#include "graph/ssspp.h"

using std::vector;

void johnson(const Graph& graph,
             vector< vector<int> >& shortest_paths) {
  Graph sourced_graph = add_source_vertex(graph);

  vector<int> potential;
  ford_bellman(sourced_graph, graph.size(), potential);

  // update distance
  Graph updated_distance_graph(graph.size());
  for (int tail = 0; tail < graph.size(); ++tail) {
    for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
      const Arc& arc = graph[tail][arc_index];
      updated_distance_graph[tail].push_back(
        Arc(arc.head, arc.weight + potential[tail] - potential[arc.head]));
    }
  }

  shortest_paths.assign(graph.size(), vector<int>());
  for (int source = 0; source < graph.size(); ++source) {
    dijkstra_on_priority_queue(updated_distance_graph,
                               source,
                               shortest_paths[source]);
    for (int destination = 0; destination < graph.size(); ++destination) {
      shortest_paths[source][destination] -=
          (potential[source] - potential[destination]);
    }
  }
}
