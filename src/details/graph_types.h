#ifndef CIRCINC_GRAPH_TYPES_H
#define CIRCINC_GRAPH_TYPES_H

#include "boost/graph/adjacency_list.hpp"
#include <forward_list>

using OutEdgesListType  = boost::vecS;
using VertexListType    = boost::vecS;
using GraphType         = boost::directedS;
using Graph             = boost::adjacency_list<OutEdgesListType, VertexListType, GraphType>;

using EdgeComposantType = unsigned long;
using Edge              = std::pair<EdgeComposantType, EdgeComposantType>;

using Vertex            = boost::graph_traits<Graph>::vertex_descriptor;

using Cycle             = std::vector<Vertex>;
using CycleEdgeSet      = std::set<Edge>;
using CyclesContainer   = std::forward_list<CycleEdgeSet>;

using CycleRemovingAlgorithmType = std::function<std::vector<Edge> /*edges_to_remove*/(
        CyclesContainer /*cycle_list*/)>;

#endif //CIRCINC_GRAPH_TYPES_H
