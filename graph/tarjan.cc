#include <vector>
#include <queue>
#include <iostream>
#include <utility>
#include <algorithm>
#include <list>

#include "graph/common.h"

using std::vector;
using std::queue;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;
using std::list;

bool check_and_destroy(int root, int checked,
                       vector< list<int> >& children,
                       vector< pair<int, list<int>::iterator> >& parent,
                       vector<bool>& actual) {
  if (root == checked) {
    return false;
  }

  for (list<int>::iterator child_iterator = children[root].begin();
       child_iterator != children[root].end();
       ++child_iterator) {
    int child = *child_iterator;
    actual[child] = false;
    parent[child] = make_pair(-1, list<int>::iterator());
    if (!check_and_destroy(child, checked, children, parent, actual)) {
      return false;
    }
  }
  children[root].clear();
  return true;
}

bool tarjan_ssspp(const Graph& graph,
                  int source,
                  vector<int>& distance) {
  distance.assign(graph.size(), INFINITY);
  distance[source] = 0;

  vector<int> color(graph.size(), WHITE);
  color[source] = GRAY;

  vector< list<int> > children(graph.size());
  vector< pair<int, list<int>::iterator> >
      parent(graph.size(), make_pair(-1, list<int>::iterator()));

  vector<bool> actual(graph.size());
  actual[source] = true;
  
  queue<int> active_vertices;
  active_vertices.push(source);

  while (!active_vertices.empty()) {
    int tail = active_vertices.front();
    active_vertices.pop();

    if (actual[tail]) {
      for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
        const Arc& arc = graph[tail][arc_index];
        if (distance[arc.head] > distance[tail] + arc.weight) {
          if (!check_and_destroy(arc.head, tail, children, parent, actual)) {
            return false;
          }
          
          distance[arc.head] = distance[tail] + arc.weight;
          if (parent[arc.head].first != -1) {
            children[parent[arc.head].first].erase(parent[arc.head].second);
          }
          children[tail].push_back(arc.head);
          parent[arc.head] = make_pair(tail, --children[tail].end());
          actual[arc.head] = true;
          if (color[arc.head] != GRAY) {
            color[arc.head] = GRAY;
            active_vertices.push(arc.head);
          }
        }
      }
    }
    color[tail] = BLACK;
  }

  return true;
}
