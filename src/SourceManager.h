#ifndef CIRCINC_SOURCEMANAGER_H
#define CIRCINC_SOURCEMANAGER_H

#include "boost/filesystem/path.hpp"
#include "details/wave_context_policies/TraceIncludes.h"
#include "DependencyGraph.h"

#include <vector>
#include <string>
#include <map>
#include <ostream>

namespace circinc {

    namespace fs = boost::filesystem;

    class SourceManager {

    public:

        /**
         * Start from the given entry_point, process all the files included in the entry_point
         * recursively and construct the dependency graph.
         * @param include_directories    The directories where we should search for #included files.
         * @param sysinclude_directories The directories where we should search for system #included files.
         * @param entry_point            The entry point of the program. Typically a file containing a
         * main function or the main header of a library.
         */
        SourceManager(std::vector<fs::path> const &include_directories,
                      std::vector<fs::path> const & sysinclude_directories,
                      fs::path const &entry_point);

        /**
         * Compute the dependencies to remove to break the cycle dependencies and output them.
         * @param algorithm The algorithm used to choose the dependencies to remove.
         *  The algorithm should take in parameter a CyclesContainer (which is basically an iterable
         *  over each cycle, a cycle being represented as an iterable over the edges present in this cycle)
         *  and return a std::vector containing the edge(s) to remove to break the cycle dependency.
         * @param os        The output stream on which the instructions will be wrote.
         */
        void output_dependencies_to_remove(CycleRemovingAlgorithmType const & algorithm,
                                           std::ostream & os = std::cout) const;

    private:

        /**
         * Analyse the file given by file_path and extract all the "valid" #include directives.
         *
         * A "valid" #include directive is an #include directive whose parameter (the #included file)
         * can be found on the disk (with the given include_directories and sysinclude_directories).
         * @param include_directories    The not-system include directories (i.e. used with #include "...")
         * @param sysinclude_directories The system include directories (i.e. used with #include <...> and when
         *  #include "..." found nothing in the include_directories).
         * @param file_path              The file to analyse.
         * @return                       The list of all the found #included files as absolute paths.
         */
        TraceIncludes::files_container_type get_included_files(std::vector<fs::path> const &include_directories,
                                                               std::vector<fs::path> const &sysinclude_directories,
                                                               fs::path const &file_path);

        DependencyGraph m_graph;

    };

}

#endif //CIRCINC_SOURCEMANAGER_H
