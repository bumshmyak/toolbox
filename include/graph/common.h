#ifndef _TOOLBOX_GRAPH_COMMON_H_
#define _TOOLBOX_GRAPH_COMMON_H_

#include <vector>

struct Arc {
  int head;
  int weight;

  Arc() :
      head(-1),
      weight(0)
    { }

  Arc(int to, int label) :
      head(to),
      weight(label)
  { }
};

enum VertexColor {
  WHITE = 0,
  GRAY,
  BLACK
};

typedef std::vector< std::vector<Arc> > Graph;

const int INFINITY = 1000000000;
const int MAX_WEIGHT = 1000000;

void input_from_matrix(Graph& graph, int& source, int& destination);
void input_from_lists(Graph& graph, int& source, int& destination);

// generates graph with multi arcs and 1-cycles
Graph generate_random_graph(long long vertices_count, long long arcs_count);

// sums up multi arcs and removes 1-cycles
Graph normalize(const Graph& graph);

// additionaly makes no arcs to source and from destination
Graph normalize(const Graph& graph, int source, int destination);

void output(const Graph& graph);

Graph invert(const Graph& graph);
Graph add_source_vertex(const Graph& graph);

#endif  // _TOOLBOX_GRAPH_COMMON_H_
