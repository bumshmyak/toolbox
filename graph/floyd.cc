#include <vector>
#include <algorithm>

#include "graph/common.h"

using std::vector;

void floyd(const Graph& graph,
           vector< vector<int> >& shortest_paths) {
  shortest_paths.assign(graph.size(), vector<int>(graph.size(), INFINITY));

  for (int tail = 0; tail < graph.size(); ++tail) {
    for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
      const Arc& arc = graph[tail][arc_index];
      shortest_paths[tail][arc.head] = std::min(shortest_paths[tail][arc.head],
                                                arc.weight);
    }
  }

  for (int vertex = 0; vertex < graph.size(); ++vertex) {
    shortest_paths[vertex][vertex] = 0;
  }

  for (int k = 0; k < graph.size(); ++k) {
    for (int i = 0; i < graph.size(); ++i) {
      for (int j = 0; j < graph.size(); ++j) {
        if (shortest_paths[i][k] != INFINITY &&
            shortest_paths[k][j] != INFINITY) {
          shortest_paths[i][j] =
              std::min(shortest_paths[i][j],
                       shortest_paths[i][k] + shortest_paths[k][j]);
        }
      }
    }
  }
}
