#include "graph/apspp.h"

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

#include "gtest/gtest.h"

#include "graph/ssspp.h"

using std::cout;
using std::endl;
using std::vector;

TEST(APSPPTest, Floyd) {
  for (int vertices_count = 1; vertices_count < 10; ++vertices_count) {
    for (int arcs_count = 0;
         arcs_count < vertices_count * (vertices_count - 1);
         ++arcs_count) {
      Graph graph = generate_random_graph(vertices_count, arcs_count);
      vector< vector<int> > shortest_paths_by_floyd;
      floyd(graph, shortest_paths_by_floyd);

      // verify by n * ford_bellman
      for (int source = 0; source < vertices_count; ++source) {
        vector<int> distance;
        ford_bellman(graph, source, distance);
        ASSERT_EQ(distance, shortest_paths_by_floyd[source]);
      }
    }
  }
}

TEST(APSPPTest, Johnson) {
  for (int vertices_count = 1; vertices_count < 10; ++vertices_count) {
    for (int arcs_count = 0;
         arcs_count < vertices_count * (vertices_count - 1);
         ++arcs_count) {
      Graph graph = generate_random_graph(vertices_count, arcs_count);
      vector< vector<int> > shortest_paths_by_floyd;
      floyd(graph, shortest_paths_by_floyd);

      vector< vector<int> > shortest_paths_by_johnson;
      johnson(graph, shortest_paths_by_johnson);

      ASSERT_EQ(shortest_paths_by_floyd, shortest_paths_by_johnson);
    }
  }
}
