#ifndef _TOOLBOX_GRAPH_DIJKSTRA_INL_
#define _TOOLBOX_GRAPH_DIJKSTRA_INL_

#include <vector>
#include <algorithm>

#include "graph/heap.h"

template <int K>
void dijkstra_on_kary_heap(const Graph& graph, int source,
                           std::vector<int>& distance) {
  std::vector<int> color(graph.size(), WHITE);
  color[source] = GRAY;

  distance.assign(graph.size(), INFINITY);
  distance[source] = 0;

  GraphKaryHeap<int, K> active_vertices(graph.size());
  active_vertices.push(source, distance[source]);

  while (!active_vertices.empty()) {
    // find min
    int closest_active_vertex = active_vertices.top();
    active_vertices.pop();
    assert(color[closest_active_vertex] == GRAY);

    // early terminate, not to do overflows
    if (distance[closest_active_vertex] == INFINITY) {
      break;
    }

    // relax
    color[closest_active_vertex] = BLACK;
    for (int arc_index = 0;
         arc_index < graph[closest_active_vertex].size();
         ++arc_index) {
      const Arc& arc = graph[closest_active_vertex][arc_index];
      if (color[arc.head] != BLACK &&
          distance[arc.head] > distance[closest_active_vertex] + arc.weight) {
        distance[arc.head] = distance[closest_active_vertex] + arc.weight;
        if (color[arc.head] == WHITE) {
          color[arc.head] = GRAY;
          active_vertices.push(arc.head, distance[arc.head]);
        } else {
          active_vertices.decrease_key(arc.head, distance[arc.head]);
        }
      }
    }
  }
}

template <int K>
int dijkstra_on_kary_heap(const Graph& graph, int source, int destination) {
  std::vector<int> distance;
  dijkstra_on_kary_heap<K>(graph, source, distance);
  return distance[destination];
}

#endif  // _TOOLBOX_GRAPH_DIJKSTRA_INL_
