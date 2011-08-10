#include <vector>
#include <iostream>

#include "graph/maxflow.h"

#include "gtest/gtest.h"

using std::cerr;
using std::endl;
using std::vector;

TEST(MaxFlowTest, Simplest) {
  Graph graph(2);
  graph[0].push_back(Arc(1, 1));
  
  Graph flow;
  EXPECT_EQ(1, edmondson_karp(graph, 0, 1, flow));
  EXPECT_EQ(2, flow.size());
  EXPECT_EQ(1, flow[0][0].head);
  EXPECT_EQ(1, flow[0][0].weight);

  EXPECT_EQ(1, relabel_to_front(graph, 0, 1, flow));
  EXPECT_EQ(2, flow.size());
  EXPECT_EQ(1, flow[0][0].head);
  EXPECT_EQ(1, flow[0][0].weight);

  EXPECT_EQ(1, blocking_flows(graph, 0, 1, flow));
  EXPECT_EQ(2, flow.size());
  EXPECT_EQ(1, flow[0][0].head);
  EXPECT_EQ(1, flow[0][0].weight);
}

TEST(MaxFlowTest, UnreachableDestination) {
  Graph graph(2);
  
  Graph flow;
  EXPECT_EQ(0, edmondson_karp(graph, 0, 1, flow));
  EXPECT_EQ(0, relabel_to_front(graph, 0, 1, flow));
}

TEST(MaxFlowTest, CormenSample) {
  // Lucky Puck example from Cormen et al

  Graph graph;
  graph.resize(6);

  graph[0].push_back(Arc(1, 16));
  graph[0].push_back(Arc(2, 13));
  graph[1].push_back(Arc(2, 10));
  graph[1].push_back(Arc(3, 12));
  graph[2].push_back(Arc(1, 4));
  graph[2].push_back(Arc(4, 14));
  graph[3].push_back(Arc(2, 9));
  graph[3].push_back(Arc(5, 20));
  graph[4].push_back(Arc(3, 7));
  graph[4].push_back(Arc(5, 4));

  Graph flow;
  EXPECT_EQ(23, edmondson_karp(graph, 0, 5, flow));
  EXPECT_EQ(23, relabel_to_front(graph, 0, 5, flow));
  EXPECT_EQ(23, blocking_flows(graph, 0, 5, flow));
}

TEST(MaxFlowTest, EdmondsonKarpMaxTest) {
  const int TEST_COUNT = 10;
  const int VERTICES_COUNT = 100;
  const int ARCS_COUNT = 10 * VERTICES_COUNT;
  srand(42);

  for (int test = 0; test < TEST_COUNT; ++test) {
    Graph graph = normalize(generate_random_graph(VERTICES_COUNT, ARCS_COUNT),
                            0, VERTICES_COUNT - 1);
    Graph flow;
    edmondson_karp(graph, 0, VERTICES_COUNT - 1, flow);
  }
}

TEST(MaxFlowTest, RelabelToFrontMaxTest) {
  const int TEST_COUNT = 10;
  const int VERTICES_COUNT = 100;
  const int ARCS_COUNT = 10 * VERTICES_COUNT;
  srand(42);

  for (int test = 0; test < TEST_COUNT; ++test) {
    Graph graph = normalize(generate_random_graph(VERTICES_COUNT, ARCS_COUNT),
                            0, VERTICES_COUNT - 1);
    Graph flow;
    relabel_to_front(graph, 0, VERTICES_COUNT - 1, flow);
  }
}

TEST(MaxFlowTest, BlockingFlowsMaxTest) {
  const int TEST_COUNT = 10;
  const int VERTICES_COUNT = 100;
  const int ARCS_COUNT = 10 * VERTICES_COUNT;
  srand(42);

  for (int test = 0; test < TEST_COUNT; ++test) {
    Graph graph = normalize(generate_random_graph(VERTICES_COUNT, ARCS_COUNT),
                            0, VERTICES_COUNT - 1);
    Graph flow;
    blocking_flows(graph, 0, VERTICES_COUNT - 1, flow);
  }
}


TEST(MaxFlowTest, Stress) {
  const int TEST_COUNT = 100;
  const int MAX_VERTICES_COUNT = 100;

  for (int test = 0; test < TEST_COUNT; ++test) {
    int vertices_count = rand() % MAX_VERTICES_COUNT + 1;
    int arcs_count = rand() % (vertices_count * vertices_count);
    Graph graph = normalize(
        generate_random_graph(vertices_count, arcs_count));

    graph.push_back(vector<Arc>());
    graph.push_back(vector<Arc>());
    graph[vertices_count].push_back(Arc(0, rand() % MAX_WEIGHT));
    graph[rand() % vertices_count].push_back(Arc(vertices_count + 1,
                                                 rand() % MAX_WEIGHT));

    Graph flow;
    int edmondson_karp_result = edmondson_karp(graph, vertices_count,
                                               vertices_count + 1, flow);
    
    int relabel_to_front_result = relabel_to_front(graph, vertices_count,
                                                   vertices_count + 1, flow);

    int blocking_flows_result = blocking_flows(graph, vertices_count,
                                               vertices_count + 1, flow);
    
    EXPECT_EQ(edmondson_karp_result, relabel_to_front_result);
    EXPECT_EQ(edmondson_karp_result, blocking_flows_result);
    if (edmondson_karp_result != relabel_to_front_result) {
      cerr << "graph: " << endl;
      output(graph);
      cerr << "edmondson_karp " << edmondson_karp_result << endl;
      cerr << "relabel_to_front " << relabel_to_front_result << endl;
      break;
    }
  }
}

TEST(MaxFlowTest, SelfIncomingEdges) {
  Graph graph(3);
  graph[0].push_back(Arc(1, 1));
  graph[1].push_back(Arc(1, 1));
  graph[1].push_back(Arc(2, 1));

  Graph flow;
  EXPECT_EQ(1, edmondson_karp(graph, 0, 2, flow));
  EXPECT_EQ(1, relabel_to_front(graph, 0, 2, flow));
  EXPECT_EQ(1, blocking_flows(graph, 0, 2, flow));
}
