#include "DependencyGraph.h"
#include "boost/graph/hawick_circuits.hpp"
#include "details/graph_visitors/HawickVisitor.h"

void DependencyGraph::add_relation(fs::path const &main_file, fs::path const &included_file) {
    add_file_in_graph_if_not_present(main_file);
    add_file_in_graph_if_not_present(included_file);

    boost::add_edge(m_vertices_names.right.at(main_file), m_vertices_names.right.at(included_file), m_dependency_graph);
}

void DependencyGraph::add_file_in_graph_if_not_present(fs::path const & path) {
    if(m_vertices_names.right.find(path) == m_vertices_names.right.end())
        m_vertices_names.insert( bimap_type::value_type(boost::add_vertex(m_dependency_graph), path) );
}

CyclesContainer DependencyGraph::get_cycles() const {
    // Compute all the cycles from the graph
    CyclesContainer cycles;
    boost::hawick_circuits(m_dependency_graph, HawickVisitor(cycles));
    return cycles;
}

fs::path DependencyGraph::get_file_path(Vertex const &vertex) const {
    return m_vertices_names.left.at(vertex);
}
