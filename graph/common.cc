#include "graph/common.h"

#include <iostream>
#include <algorithm>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

void input_from_matrix(Graph& graph, int& source, int& destination) {
  int vertices_count;
  cin >> vertices_count >> source >> destination;
  graph.resize(vertices_count);
  --source;
  --destination;

  for (int tail = 0; tail < vertices_count; ++tail) {
    for (int head = 0; head < vertices_count; ++head) {
      int weight;
      cin >> weight;
      if (tail != head && weight != -1) {
        graph[tail].push_back(Arc(head, weight));
      }
    }
  }
}

void input_from_lists(Graph& graph, int& source, int& destination) {
  int vertices_count, arcs_count;
  cin >> vertices_count >> arcs_count >> source >> destination;
  graph.resize(vertices_count);
  --source;
  --destination;

  for (int arc_index = 0; arc_index < arcs_count; ++arc_index) {
    int tail, head, weight;
    cin >> tail >> head >> weight;
    --tail;
    --head;
    graph[tail].push_back(Arc(head, weight));
  }
}

Graph generate_random_graph(long long vertices_count, long long arcs_count) {
  Graph graph(vertices_count);

  for (int arc_index = 0; arc_index < arcs_count; ++arc_index) {
    long long arc_code, tail, head;
    do {
      arc_code = ((long long)rand()) % (vertices_count * vertices_count);
      tail = arc_code / vertices_count;
      head = arc_code % vertices_count;
    } while (tail == head);

    int weight = rand() % MAX_WEIGHT;
    graph[tail].push_back(Arc(head, weight));
  }

  return graph;
}

Graph invert(const Graph& graph) {
  Graph inverted_graph(graph.size());
  for (int tail = 0; tail < graph.size(); ++tail) {
    for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
      const Arc& arc = graph[tail][arc_index];
      inverted_graph[arc.head].push_back(Arc(tail, arc.weight));
    }
  }
  return inverted_graph;
}

void output(const Graph& graph) {
  cout << graph.size() << endl;
  for (int tail = 0; tail < graph.size(); ++tail) {
    for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
      const Arc& arc = graph[tail][arc_index];
      cout << tail << ' ' << arc.head << ' ' << arc.weight << endl;
    }
  }
}

Graph add_source_vertex(const Graph& graph) {
  Graph sourced_graph = graph;
  sourced_graph.push_back(vector<Arc>());

  for (int destination = 0; destination < graph.size(); ++destination) {
    sourced_graph[graph.size()].push_back(Arc(destination, 0));
  }

  return sourced_graph;
}
