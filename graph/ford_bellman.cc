#include <vector>
#include <queue>
#include <iostream>

#include "graph/common.h"

using std::vector;
using std::queue;
using std::cout;
using std::endl;

void ford_bellman(const Graph& graph,
                  int source,
                  std::vector<int>& distance) {
  distance.assign(graph.size(), INFINITY);
  distance[source] = 0;

  for (int pass = 0; pass < graph.size(); ++pass) {
    for (int tail = 0; tail < graph.size(); ++tail) {
      if (distance[tail] < INFINITY) {
        for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
          const Arc& arc = graph[tail][arc_index];
          int relaxed_distance = distance[tail] + arc.weight;
          if (distance[arc.head] > relaxed_distance) {
            distance[arc.head] = relaxed_distance;
          }
        }
      }
    }
  }
}

void ford_bellman_on_queue(const Graph& graph,
                           int source,
                           std::vector<int>& distance) {
  distance.assign(graph.size(), INFINITY);
  distance[source] = 0;

  vector<int> color(graph.size(), WHITE);
  color[source] = GRAY;

  queue<int> active_vertices;
  active_vertices.push(source);

  while (!active_vertices.empty()) {
    int tail = active_vertices.front();
    active_vertices.pop();

    color[tail] = BLACK;

    for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
      const Arc& arc = graph[tail][arc_index];
      if (distance[arc.head] > distance[tail] + arc.weight) {
        distance[arc.head] = distance[tail] + arc.weight;
        if (color[arc.head] != GRAY) {
          color[arc.head] = GRAY;
          active_vertices.push(arc.head);
        }
      }
    }
  }
}

int ford_bellman(const Graph& graph, int source, int destination) {
  vector<int> distance;
  ford_bellman(graph, source, distance);
  return distance[destination];
}

int ford_bellman_on_queue(const Graph& graph, int source, int destination) {
  vector<int> distance;
  ford_bellman_on_queue(graph, source, distance);
  return distance[destination];
}
