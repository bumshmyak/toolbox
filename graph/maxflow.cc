#include <graph/maxflow.h>

#include <limits>
#include <cassert>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <iostream>
#include <utility>

using std::vector;
using std::queue;
using std::min;
using std::list;
using std::pair;
using std::make_pair;

using std::cerr;
using std::endl;

struct LinkedArc {
  int head;
  int weight;
  int inv_arc_index;

  LinkedArc()
      : head(-1),
        weight(-1),
        inv_arc_index(-1)
  { }

  LinkedArc(int h, int w, int ind)
      : head(h),
        weight(w),
        inv_arc_index(ind)
  { }
};

typedef vector< vector<LinkedArc> > LinkedGraph;

void initialize_capacity_and_flow(const Graph& graph,
                                  Graph& capacity,
                                  LinkedGraph& flow) {
  capacity.resize(graph.size());
  flow.resize(graph.size());
  for (int tail = 0; tail < graph.size(); ++tail) {
    for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
      const Arc& arc = graph[tail][arc_index];
      capacity[tail].push_back(arc);
      capacity[arc.head].push_back(Arc(tail, 0));
      
      int forward_arc_index = flow[tail].size();
      int inv_arc_index = flow[arc.head].size();
      flow[tail].push_back(LinkedArc(arc.head, 0, inv_arc_index));
      flow[arc.head].push_back(LinkedArc(tail, 0, forward_arc_index));
    }
  }
}

int extract_real_flow(const LinkedGraph& flow,
                      int source,
                      Graph& output_flow) {
  output_flow.clear();
  output_flow.resize(flow.size());

  for (int tail = 0; tail < flow.size(); ++tail) {
    for (int arc_index = 0; arc_index < flow[tail].size(); ++arc_index) {
      const LinkedArc& arc = flow[tail][arc_index];
      if (arc.weight > 0) {
        output_flow[tail].push_back(Arc(arc.head, arc.weight));
      }
    }
  }

  int flow_value = 0;
  for (int arc_index = 0; arc_index < output_flow[source].size(); ++arc_index) {
    flow_value += output_flow[source][arc_index].weight;
  }

  return flow_value;
}

void add_flow(LinkedGraph& flow, int source, int arc_index, int value) {
  LinkedArc& arc = flow[source][arc_index];
  arc.weight += value;
  flow[arc.head][arc.inv_arc_index].weight -= value;
}

bool augment_flow_by_shortest_path(
    const Graph& capacity,
    int source,
    int destination,
    LinkedGraph& flow) {
  int vertices_count = capacity.size();
  assert(flow.size() == vertices_count);
  
  queue<int> active_vertices;
  active_vertices.push(source);
  
  vector<int> visited(vertices_count);
  visited[source] = true;
  
  vector< pair<int, int> > pred(vertices_count, make_pair(-1, -1));
 
  while (!active_vertices.empty()) {
    int tail = active_vertices.front();
    active_vertices.pop();

    if (tail == destination) {
      int min_remaining_capacity = std::numeric_limits<int>::max();
      int current = destination;
      while (current != source) {
        int previous = pred[current].first;
        int arc_index = pred[current].second;
        min_remaining_capacity = min(min_remaining_capacity,
                                     capacity[previous][arc_index].weight -
                                     flow[previous][arc_index].weight);
        current = previous;
      }
      
      current = destination;
      while (current != source) {
        int previous = pred[current].first;
        int arc_index = pred[current].second;
        add_flow(flow, previous, arc_index, min_remaining_capacity);
        current = previous;
      }
      return true;
    } else {
      for (int head_index = 0;
           head_index < capacity[tail].size();
           ++head_index) {
        int head = capacity[tail][head_index].head;
        if (!visited[head] &&
            capacity[tail][head_index].weight
            - flow[tail][head_index].weight > 0) {
          visited[head] = true;
          active_vertices.push(head);
          pred[head] = make_pair(tail, head_index);
        }
      }
    }
  }

  return false;
}
                               

// Augmenting paths: augment by shortest path using bfs
int edmondson_karp(const Graph& graph,
                  int source,
                  int destination,
                  Graph& output_flow) {
  assert(source != destination);

  Graph capacity;
  LinkedGraph flow;
  initialize_capacity_and_flow(graph, capacity, flow);
  while (augment_flow_by_shortest_path(capacity, source, destination, flow)) { }
  return extract_real_flow(flow, source, output_flow);
}


void relabel(const Graph& capacity,
             const LinkedGraph& flow,
             int tail,
             vector<int>& h) {
  int min_height = 2 * capacity.size();
  for (int arc_index = 0; arc_index < capacity[tail].size(); ++arc_index) {
    if (capacity[tail][arc_index].weight - flow[tail][arc_index].weight > 0) {
      min_height = min(min_height, h[capacity[tail][arc_index].head]);
    }
  }
  h[tail] = min_height + 1;
}

void push(const Graph& capacity,
          int tail,
          int arc_index,
          vector<int>& e,
          LinkedGraph& flow) {
  int addition = min(e[tail],
                     capacity[tail][arc_index].weight -
                     flow[tail][arc_index].weight);

  add_flow(flow, tail, arc_index, addition);
  e[tail] -= addition;
  e[capacity[tail][arc_index].head] += addition;
}

void discharge(const Graph& capacity,
               int tail,
               LinkedGraph& flow,
               vector<int>& e,
               vector<int>& h) {
  int arc_index = 0;
  int size = capacity[tail].size();
  while (e[tail] > 0) {
    if (arc_index == size) {
      relabel(capacity, flow, tail, h);
      arc_index = 0;
    } else {
      if (capacity[tail][arc_index].weight - flow[tail][arc_index].weight > 0 &&
               h[tail] == h[capacity[tail][arc_index].head] + 1) {
        push(capacity, tail, arc_index, e, flow);
      }
      ++arc_index;
    }
  }
}
               

int relabel_to_front(const Graph& graph,
                     int source,
                     int destination,
                     Graph& output_flow) {
  assert(source != destination);
  output_flow.clear();

  Graph capacity;
  LinkedGraph flow;
  initialize_capacity_and_flow(graph, capacity, flow);

  vector<int> h(graph.size());
  h[source] = graph.size();

  vector<int> e(graph.size());
  for (int arc_index = 0; arc_index < capacity[source].size(); ++arc_index) {
    const Arc& arc = capacity[source][arc_index];
    add_flow(flow, source, arc_index, arc.weight);
    e[arc.head] += arc.weight;
  }

  list<int> processing_list;
  for (int v = 0; v < graph.size(); ++v) {
    if (v != source && v != destination) {
      processing_list.push_back(v);
    }
  }

  list<int>::iterator current = processing_list.begin();
  while (current != processing_list.end()) {
    int v = *current;
    int old_height = h[v];
    discharge(capacity, v, flow, e, h);
    if (h[v] > old_height) {
      processing_list.erase(current);
      processing_list.push_front(v);
      current = processing_list.begin();
    }
    ++current;
  }

  return extract_real_flow(flow, source, output_flow);
}

bool build_shortest_paths_graph(const Graph& capacity,
                                const LinkedGraph& flow,
                                int source,
                                int destination,
                                vector< list<int> >& arc_indexes) {
  arc_indexes.clear();
  arc_indexes.resize(capacity.size());
  
  vector<int> distance(capacity.size(), INFINITY);
  distance[source] = 0;
  
  queue<int> active_vertices;
  active_vertices.push(source);

  while (!active_vertices.empty()) {
    int tail = active_vertices.front();
    active_vertices.pop();
    for (int arc_index = 0; arc_index < capacity[tail].size(); ++arc_index) {
      int head = capacity[tail][arc_index].head;
      if (capacity[tail][arc_index].weight - flow[tail][arc_index].weight > 0) {
        if (distance[head] == INFINITY) {
          distance[head] = distance[tail] + 1;
          active_vertices.push(head);
        }
        if (distance[head] == distance[tail] + 1) {
          arc_indexes[tail].push_back(arc_index);
        }
      }
    }
  }

  return distance[destination] < INFINITY;
}

int augment_blocking_flow(const Graph& capacity,
                          LinkedGraph& flow,
                          vector< list<int> >& arc_indexes,
                          int source,
                          int destination,
                          int value) {
  if (source == destination) {
    return value;
  }
  
  int sended_flow = 0;
  while (!arc_indexes[source].empty() && sended_flow == 0) {
    int arc_index = *(arc_indexes[source].begin());
    int arc_weight = capacity[source][arc_index].weight -
        flow[source][arc_index].weight;
    assert(arc_weight > 0);
    sended_flow = augment_blocking_flow(capacity, flow,
                                        arc_indexes,
                                        capacity[source][arc_index].head,
                                        destination,
                                        min(value, arc_weight));
                                        
    if (sended_flow == 0) {
      arc_indexes[source].erase(arc_indexes[source].begin());
    } else {
      add_flow(flow, source, arc_index, sended_flow);
      if (sended_flow == arc_weight) {
        arc_indexes[source].erase(arc_indexes[source].begin());
      }
    }
  }

  return sended_flow;
}
  
int blocking_flows(const Graph& graph,
                   int source,
                   int destination,
                   Graph& output_flow) {
  assert(source != destination);
  output_flow.clear();

  Graph capacity;
  LinkedGraph flow;
  initialize_capacity_and_flow(graph, capacity, flow);

  vector< list<int> > arc_indexes;
  while (build_shortest_paths_graph(capacity, flow,
                                    source, destination,
                                    arc_indexes)) {
    while (augment_blocking_flow(capacity, flow, arc_indexes,
                                 source, destination, INFINITY) != 0) { }
  }

  return extract_real_flow(flow, source, output_flow);
}

int get_max_bipartite_matching(const Graph& graph) {
  Graph super_graph = graph;
  int left_part_size = graph.size();
  for (int tail = 0; tail < graph.size(); ++tail) {
    for (int arc_index = 0; arc_index < graph[tail].size(); ++arc_index) {
      super_graph[tail][arc_index].head += left_part_size;
      super_graph[tail][arc_index].weight = 1;
    }
  }
  
  for (int i = 0; i < left_part_size + 2; ++i) {
    super_graph.push_back(vector<Arc>());
  }

  int source = super_graph.size() - 2;
  int destination = super_graph.size() - 1;

  for (int i = 0; i < left_part_size; ++i) {
    super_graph[source].push_back(Arc(i, 1));
    super_graph[left_part_size + i].push_back(Arc(destination, 1));
  }

  Graph flow;
  return edmondson_karp(super_graph, source, destination, flow);
}

int find_matches(int source,
                const Graph& neighbors,
                vector<int>& left_part_matches,
                vector<int>& right_part_matches) {
  vector<int> p(left_part_matches.size(), -1);
  p[source] = source;
  
  queue<int> active_vertices;
  active_vertices.push(source);

  bool path_found = false;
  int tail, next, matched;

  while (!active_vertices.empty() && !path_found) {
    tail = active_vertices.front();
    active_vertices.pop();
    for (int arc_index = 0; arc_index < neighbors[tail].size(); ++arc_index) {
      const Arc& arc = neighbors[tail][arc_index];
      matched = arc.head;
      next = right_part_matches[matched];
      if (next != tail) {
        if (next == -1) {
          path_found = true;
          break;
        } else if (p[next] == -1) {
          p[next] = tail;
          active_vertices.push(next);
        }
      }
    }
  }

  if (path_found) {
    while (tail != source) {
      int tmp = left_part_matches[tail];
      left_part_matches[tail] = matched;
      right_part_matches[matched] = tail;
      matched = tmp;
      tail = p[tail];
    }
    left_part_matches[tail] = matched;
    right_part_matches[matched] = tail;
    return 1;
  } else {
    return 0;
  }
}

int get_max_bipartite_matching_simply(int left_part_size,
                                      int right_part_size,
                                      const vector< pair<int, int> >& arcs) {
  Graph neighbors(left_part_size);
  for (int arc_index = 0; arc_index < arcs.size(); ++arc_index) {
    const pair<int, int>& arc = arcs[arc_index];
    neighbors[arc.first].push_back(Arc(arc.second, 1));
  }

  vector<int> left_part_matches(left_part_size, -1);
  vector<int> right_part_matches(right_part_size, -1);

  int result = 0;
  for (int source = 0; source < left_part_size; ++source) {
    result += find_matches(source, neighbors,
                           left_part_matches,
                           right_part_matches);
  }

  return result;
}
