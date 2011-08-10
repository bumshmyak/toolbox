#include "graph/ssspp.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "gtest/gtest.h"

using std::vector;
using std::cerr;
using std::endl;

TEST(SSSPPTest, Simplest) {
  Graph graph(3);
  graph[0].push_back(Arc(1, 0));
  graph[0].push_back(Arc(2, 9));
  graph[1].push_back(Arc(2, 8));
  
  vector<int> dijkstra_on_array_result;
  dijkstra_on_array(graph, 0, dijkstra_on_array_result);

  vector<int> tarjan_ssspp_result;
  ASSERT_TRUE(tarjan_ssspp(graph, 0, tarjan_ssspp_result));

  ASSERT_EQ(dijkstra_on_array_result, tarjan_ssspp_result);
}

TEST(SSSPPTest, SingleSource) {
  for (int vertices_count = 1; vertices_count < 10; ++vertices_count) {
    for (int arcs_count = 0;
         arcs_count < vertices_count * (vertices_count - 1);
         ++arcs_count) {
      Graph graph = generate_random_graph(vertices_count, arcs_count);
      int source = rand() % vertices_count;

      vector<int> dijkstra_on_array_result;
      dijkstra_on_array(graph, source, dijkstra_on_array_result);

      vector<int> dijkstra_on_priority_queue_result;
      dijkstra_on_priority_queue(graph, source,
                                 dijkstra_on_priority_queue_result);

      vector<int> dijkstra_on_set_result;
      dijkstra_on_set(graph, source, dijkstra_on_set_result);

      vector<int> dijkstra_on_kary_heap_result;
      dijkstra_on_kary_heap<2>(graph, source, dijkstra_on_kary_heap_result);

      vector<int> ford_bellman_result;
      ford_bellman(graph, source, ford_bellman_result);

      vector<int> ford_bellman_on_queue_result;
      ford_bellman_on_queue(graph, source, ford_bellman_on_queue_result);

      vector<int> tarjan_ssspp_result;
      EXPECT_TRUE(tarjan_ssspp(graph, source, tarjan_ssspp_result));

      ASSERT_EQ(dijkstra_on_array_result, dijkstra_on_priority_queue_result);
      ASSERT_EQ(dijkstra_on_array_result, dijkstra_on_set_result);
      ASSERT_EQ(dijkstra_on_array_result, dijkstra_on_kary_heap_result);
      ASSERT_EQ(dijkstra_on_array_result, ford_bellman_result);
      ASSERT_EQ(dijkstra_on_array_result, ford_bellman_on_queue_result);
      ASSERT_EQ(dijkstra_on_array_result, tarjan_ssspp_result);
    }
  }
}

TEST(SSSPPTest, SourceDestination) {
  for (int vertices_count = 1; vertices_count < 10; ++vertices_count) {
    for (int arcs_count = 0;
         arcs_count < vertices_count * (vertices_count - 1);
         ++arcs_count) {
      Graph graph = generate_random_graph(vertices_count, arcs_count);
      int source = rand() % vertices_count;
      int destination = rand() % vertices_count;

      int dijkstra_on_array_result =
        dijkstra_on_array(graph, source, destination);

      // move to separate test
      int bidirected_dijkstra_result =
        bidirected_dijkstra(graph, source, destination);

      EXPECT_EQ(dijkstra_on_array_result, bidirected_dijkstra_result);
    }
  }
}

// test shows graph on which dijkstra breaks,
// but ford-bellman survives
TEST(SSSPPTest, NegativeArcs) {
  Graph graph(4);
  graph[0].push_back(Arc(1, 1));
  graph[0].push_back(Arc(2, 3));
  graph[1].push_back(Arc(3, 1));
  graph[2].push_back(Arc(3, -10));

  vector<int> dijkstra_result;
  dijkstra_on_array(graph, 0, dijkstra_result);

  vector<int> ford_bellman_result;
  ford_bellman(graph, 0, ford_bellman_result);

  EXPECT_EQ(0, dijkstra_result[0]);
  EXPECT_EQ(0, ford_bellman_result[0]);

  EXPECT_EQ(1, dijkstra_result[1]);
  EXPECT_EQ(1, ford_bellman_result[1]);

  EXPECT_EQ(3, dijkstra_result[2]);
  EXPECT_EQ(3, ford_bellman_result[2]);

  // !!!
  EXPECT_EQ(2, dijkstra_result[3]);
  EXPECT_EQ(-7, ford_bellman_result[3]);
}

TEST(SSSPPTest, NegativeCycle) {
  Graph graph(3);
  graph[0].push_back(Arc(1, 0));
  graph[1].push_back(Arc(2, 0));
  graph[2].push_back(Arc(0, -1));

  vector<int> d;
  EXPECT_FALSE(tarjan_ssspp(graph, 0, d));
}
