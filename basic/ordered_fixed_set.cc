#include <vector>
#include <cstring>
#include <queue>
#include <iostream>
#include <cassert>
#include <algorithm>

#include "basic/hash.h"

using std::cout;
using std::endl;
using std::vector;

namespace bicycle {

void OrderedFixedSet::generate_random_tables() {
  for (int table_index = 0; table_index < 2; ++table_index) {
    for (int offset = 0; offset < 256; ++offset) {
      random_char_table_[table_index][offset] = rand() % vertices_count_;
    }
  }

  int max_key_length = 0;
  for (int i = 0; i < keys_.size(); ++i) {
    max_key_length = std::max(max_key_length, keys_[i].second);
  }

  random_position_.resize(max_key_length);
  for (int i = 0; i < max_key_length; ++i) {
    random_position_[i] = rand() % vertices_count_;
  }
}

int OrderedFixedSet::get_hash_by_key(
    const void* key_data,
    int key_length,
    int table_index) const {
  int hash = 0;
  for (int offset = 0; offset < key_length; ++offset) {
    int table_element =
        reinterpret_cast<const unsigned char*>(key_data)[offset];
    hash = (hash +
            random_position_[offset] *
            random_char_table_[table_index][table_element])
        % vertices_count_;
  }

  return hash;
}

void OrderedFixedSet::build_graph(GraphType& graph) {
  graph.assign(vertices_count_, AdjacencyListType());
  for (int key_index = 0; key_index < keys_.size(); ++key_index) {
    std::pair<const void*, int>& key = keys_[key_index]; 
    int tail = get_hash_by_key(key.first, key.second, 0);
    int head = get_hash_by_key(key.first, key.second, 1);
    assert(0 <= tail);
    assert(tail < vertices_count_);
    assert(0 <= head && head < vertices_count_);
    graph[tail].push_back(Arc(head, key_index));
    graph[head].push_back(Arc(tail, key_index));
  }
}

bool OrderedFixedSet::bfs(
    const GraphType& graph,
    int start) {
  std::queue<int> active_vertices;
  active_vertices.push(start);
  label_[start] = 0;

  while (!active_vertices.empty()) {
    int tail = active_vertices.front();
    active_vertices.pop();
    const AdjacencyListType& adjacency_list = graph[tail];
    for (typename AdjacencyListType::const_iterator arc_iterator =
             adjacency_list.begin();
         arc_iterator != adjacency_list.end();
         ++arc_iterator) {
      int needed_label = (arc_iterator->index -
             label_[tail] +
             vertices_count_) % vertices_count_;

      if (label_[arc_iterator->head] != -1) {
        if (label_[arc_iterator->head] != needed_label) {
          return false;
        }
      } else {
        label_[arc_iterator->head] = needed_label;
        active_vertices.push(arc_iterator->head);
      }
    }
  }

  return true;
}
     
bool OrderedFixedSet::assign_labels_to_vertices(
    const GraphType& graph) {
  label_.assign(vertices_count_, -1);
 
  for (int vertice = 0; vertice < vertices_count_; ++vertice) {
    if (label_[vertice] == -1 && !graph[vertice].empty()) {
      if (!bfs(graph, vertice)) {
        return false;
      }
    }
  }
  
  return true;
}
  

void OrderedFixedSet::init(
    const std::vector< std::pair<const void*, int> >& keys) {
  keys_ = keys;
  vertices_count_ = 3 * keys.size();

  for (;;) {
    generate_random_tables();
    GraphType graph;
    build_graph(graph);
    if (assign_labels_to_vertices(graph)) {
      break;
    }
  }
}

int OrderedFixedSet::get_index(const void* key_data, int key_length) const {
  int tail = get_hash_by_key(key_data, key_length, 0);
  int head = get_hash_by_key(key_data, key_length, 1);

  if (label_[tail] == -1 || label_[head] == -1) {
    return -1;
  }
  
  int index = (label_[tail] + label_[head]) % vertices_count_;

  if ((keys_[index].second != key_length) ||
      (memcmp(keys_[index].first, key_data, key_length) != 0)) {
    return -1;
  }

  return index;
}

}  // namespace bicycle
