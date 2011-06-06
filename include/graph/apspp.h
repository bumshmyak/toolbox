#ifndef _TOOLBOX_GRAPH_APSPP_H_
#define _TOOLBOX_GRAPH_APSPP_H_

#include <vector>
#include "graph/common.h"

// shortest paths between all pairs of vertices
void floyd(const Graph& graph,
           std::vector< std::vector<int> >& shortest_paths);
void johnson(const Graph& graph,
             std::vector< std::vector<int> >& shortest_paths);

#endif  // _TOOLBOX_GRAPH_APSPP_H_

