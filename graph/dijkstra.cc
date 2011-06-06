#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <limits>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cassert>

#include "graph/ssspp.h"
#include "graph/common.h"
#include "graph/heap.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::deque;
using std::set;
using std::min;
using std::swap;

void dijkstra_on_array(const Graph& graph, int source,
                       vector<int>& distance) {
  vector<int> color(graph.size(), WHITE);
  color[source] = GRAY;

  distance.assign(graph.size(), INFINITY);
  distance[source] = 0;

  for (int step = 0; step < graph.size(); ++step) {
    // find min
    int min_gray_vertex = 0;
    int min_distance_to_gray = INFINITY;
    for (int vertex = 0; vertex < graph.size(); ++vertex) {
      if (color[vertex] == GRAY &&
          distance[vertex] < min_distance_to_gray) {
        min_gray_vertex = vertex;
        min_distance_to_gray = distance[vertex];
      }
    }

    // early terminate
    if (min_distance_to_gray == INFINITY) {
      break;
    }

    // relax
    color[min_gray_vertex] = BLACK;
    for (int arc_index = 0;
         arc_index < graph[min_gray_vertex].size();
         ++arc_index) {
      const Arc& arc = graph[min_gray_vertex][arc_index];
      if (color[arc.head] != BLACK &&
          distance[arc.head] > min_distance_to_gray + arc.weight) {
        color[arc.head] = GRAY;
        distance[arc.head] = min_distance_to_gray + arc.weight;
      }
    }
  }
}

struct EstimatedVertex {
  int index;
  int distance;

  EstimatedVertex() :
      index(-1),
      distance(INFINITY)
    { }

  EstimatedVertex(int v, int d) :
      index(v),
      distance(d)
    { }
};



bool operator<(const EstimatedVertex& first, const EstimatedVertex& second) {
  if (first.distance == second.distance) {
    return first.index < second.index;
  } else {
    return first.distance < second.distance;
  }
}

bool operator>(const EstimatedVertex& first, const EstimatedVertex& second) {
  if (first.distance == second.distance) {
    return first.index > second.index;
  } else {
    return first.distance > second.distance;
  }
}

bool operator==(const EstimatedVertex& first, const EstimatedVertex& second) {
  return (first.index == second.index) && (first.distance == second.distance);
}

std::ostream& operator<<(std::ostream& out, const EstimatedVertex& vertex) {
  out << '(' << vertex.index << ';' << vertex.distance << ')';
  return out;
}

void dijkstra_on_priority_queue(const Graph& graph, int source,
                                vector<int>& distance) {
  vector<int> color(graph.size(), WHITE);
  color[source] = GRAY;

  distance.assign(graph.size(), INFINITY);
  distance[source] = 0;

  std::priority_queue<EstimatedVertex,
                      vector<EstimatedVertex>,
                      std::greater<EstimatedVertex> > active_vertices;

  active_vertices.push(EstimatedVertex(source, distance[source]));

  while (!active_vertices.empty()) {
    // find min
    EstimatedVertex closest_active_vertex = active_vertices.top();
    active_vertices.pop();

    // already processed
    if (color[closest_active_vertex.index] == BLACK) {
      continue;
    }

    // early terminate, not to do overflows
    if (closest_active_vertex.distance == INFINITY) {
      break;
    }

    // relax
    color[closest_active_vertex.index] = BLACK;
    for (int arc_index = 0;
         arc_index < graph[closest_active_vertex.index].size();
         ++arc_index) {
      const Arc& arc = graph[closest_active_vertex.index][arc_index];
      if (color[arc.head] != BLACK &&
          distance[arc.head] > closest_active_vertex.distance + arc.weight) {
        color[arc.head] = GRAY;
        distance[arc.head] = closest_active_vertex.distance + arc.weight;
        active_vertices.push(EstimatedVertex(arc.head, distance[arc.head]));
      }
    }
  }
}

void dijkstra_on_set(const Graph& graph, int source,
                     vector<int>& distance) {
  vector<int> color(graph.size(), WHITE);
  color[source] = GRAY;

  distance.assign(graph.size(), INFINITY);
  distance[source] = 0;

  set<EstimatedVertex> active_vertices;

  active_vertices.insert(EstimatedVertex(source, distance[source]));

  while (!active_vertices.empty()) {
    // find min
    EstimatedVertex closest_active_vertex = *active_vertices.begin();
    active_vertices.erase(active_vertices.begin());

    // early terminate, not to do overflows
    if (closest_active_vertex.distance == INFINITY) {
      break;
    }

    // relax
    color[closest_active_vertex.index] = BLACK;
    for (int arc_index = 0;
         arc_index < graph[closest_active_vertex.index].size();
         ++arc_index) {
      const Arc& arc = graph[closest_active_vertex.index][arc_index];
      if (color[arc.head] != BLACK &&
          distance[arc.head] > closest_active_vertex.distance + arc.weight) {
        if (color[arc.head] == WHITE) {
          color[arc.head] = GRAY;
        } else {
          active_vertices.erase(active_vertices.find(
              EstimatedVertex(arc.head, distance[arc.head])));
        }
        distance[arc.head] = closest_active_vertex.distance + arc.weight;
        active_vertices.insert(EstimatedVertex(arc.head, distance[arc.head]));
      }
    }
  }
}

int bidirected_dijkstra(const Graph& graph, int source, int destination) {
  std::vector< Graph > graphs(2);
  graphs[0] = graph;
  graphs[1] = invert(graph);

  std::vector< std::vector<int> > color(2,
                                        std::vector<int>(graph.size(), WHITE));
  color[0][source] = GRAY;
  color[1][destination] = GRAY;

  std::vector< std::vector<int> > distance(2,
                                           std::vector<int>(graph.size(),
                                                            INFINITY));
  distance[0][source] = 0;
  distance[1][destination] = 0;

  std::vector< GraphKaryHeap<int, 2> > active_vertices(2,
                                                       GraphKaryHeap<int, 2>(
                                                           graph.size()));
  active_vertices[0].push(source, distance[0][source]);
  active_vertices[1].push(destination, distance[1][destination]);

  std::vector<int> max_distance_reached(2, 0);
  int shortest_path = INFINITY;

  while (!active_vertices[0].empty() && !active_vertices[1].empty()) {
    int dijkstra_index = 0;
    int next_dijkstra_index = 1;
    if (max_distance_reached[1] < max_distance_reached[0]) {
      std::swap(dijkstra_index, next_dijkstra_index);
    }

    // find min
    int closest_active_vertex = active_vertices[dijkstra_index].top();
    active_vertices[dijkstra_index].pop();
    assert(color[dijkstra_index][closest_active_vertex] == GRAY);

    if ((dijkstra_index == 0 && closest_active_vertex == destination) ||
        (dijkstra_index == 1 && closest_active_vertex == source)) {
      shortest_path = distance[dijkstra_index][closest_active_vertex];
      break;
    }

    // early terminate, not to do overflows
    if (distance[dijkstra_index][closest_active_vertex] == INFINITY) {
      break;
    }

    // relax
    color[dijkstra_index][closest_active_vertex] = BLACK;
    for (int arc_index = 0;
         arc_index < graphs[dijkstra_index][closest_active_vertex].size();
         ++arc_index) {
      const Arc& arc = graphs[dijkstra_index][closest_active_vertex][arc_index];
      int candidate_distance = distance[dijkstra_index][closest_active_vertex] +
          arc.weight;
      if (color[dijkstra_index][arc.head] != BLACK &&
          distance[dijkstra_index][arc.head] > candidate_distance) {
        distance[dijkstra_index][arc.head] = candidate_distance;
        if (color[dijkstra_index][arc.head] == WHITE) {
          color[dijkstra_index][arc.head] = GRAY;
          active_vertices[dijkstra_index].push(
              arc.head, distance[dijkstra_index][arc.head]);
        } else {
          active_vertices[dijkstra_index].decrease_key(
              arc.head, distance[dijkstra_index][arc.head]);
        }
      }
      if (color[next_dijkstra_index][arc.head] == BLACK) {
        shortest_path = std::min(shortest_path,
                                 distance[dijkstra_index][closest_active_vertex]
                                 + arc.weight +
                                 distance[next_dijkstra_index][arc.head]);
      }
    }

    max_distance_reached[dijkstra_index] =
        distance[dijkstra_index][closest_active_vertex];

    if (color[next_dijkstra_index][closest_active_vertex] == BLACK) {
      shortest_path = std::min(
          shortest_path,
          distance[dijkstra_index][closest_active_vertex] +
          distance[next_dijkstra_index][closest_active_vertex]);
      break;
    }
  }

  return shortest_path;
}

int dijkstra_on_array(const Graph& graph, int source, int destination) {
  vector<int> distance;
  dijkstra_on_array(graph, source, distance);
  return distance[destination];
}

int dijkstra_on_priority_queue(const Graph& graph,
                               int source,
                               int destination) {
  vector<int> distance;
  dijkstra_on_priority_queue(graph, source, distance);
  return distance[destination];
}

int dijkstra_on_set(const Graph& graph, int source, int destination) {
  vector<int> distance;
  dijkstra_on_set(graph, source, distance);
  return distance[destination];
}
