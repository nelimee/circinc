#ifndef CIRCINC_DEPENDENCYGRAPH_H
#define CIRCINC_DEPENDENCYGRAPH_H

#include "boost/graph/adjacency_list.hpp"
#include "details/graph_types.h"
#include "boost/bimap.hpp"
#include "boost/filesystem/path.hpp"

#include <set>

namespace fs = boost::filesystem;

class DependencyGraph {

    using bimap_type = boost::bimap< Vertex , fs::path >;

public:

    /**
     * Add a relation of dependence between main_file and included_file.
     *
     * After the call, main_file depends on included_file.
     *
     * @param main_file     The file path of the file that #includes included_file.
     * @param included_file The file path of the file #included by main_file.
     */
    void add_relation(fs::path const & main_file, fs::path const & included_file);

    /**
     * Add a relation of dependence between main_file and all the entries of files.
     *
     * After the call, main_file depends on all the files present in files.
     * @tparam FileContainer The type of the container used to store the files.
     *  This should be an iterable over instances of boost::filesystem::path.
     * @param main_file The file path of the file that #includes the entries of files.
     * @param files     Files path of some of the files (or all of them) #included by
     *  main_file.
     */
    template <typename FileContainer>
    void add_relations(fs::path const & main_file, FileContainer const & files) {
        add_file_in_graph_if_not_present(main_file);
        for(fs::path const & included_file : files) {
            add_file_in_graph_if_not_present(included_file);
            boost::add_edge(m_vertices_names.right.at(main_file), m_vertices_names.right.at(included_file), m_dependency_graph);
        }
    }

    /**
     * Compute all the cycles with the current dependencies and return them.
     * @return All the cycles in the dependencies.
     */
    CyclesContainer get_cycles() const;

    /**
     * Make the link between a vertex (i.e. a file) and the corresponding file path.
     * @param vertex The vertex we want to translate as a file path.
     * @return       The file path of the file represented by vertex.
     */
    fs::path get_file_path(Vertex const & vertex) const;

private:

    /**
     * Check if the given file path already represents a vertex in the dependency graph.
     * If not, then add a vertex for it and update the data_structures.
     * @param path The path we want to add to our dependency graph.
     */
    void add_file_in_graph_if_not_present(fs::path const & path);

    Graph m_dependency_graph;

    bimap_type m_vertices_names;
};


#endif //CIRCINC_DEPENDENCYGRAPH_H
