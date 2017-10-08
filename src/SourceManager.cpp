#include "SourceManager.h"

#include "boost/filesystem/fstream.hpp"

#include "boost/wave/cpplexer/cpp_lex_token.hpp"
#include "boost/wave/cpplexer/cpp_lex_iterator.hpp"
#include "boost/wave/cpp_context.hpp"
#include "boost/wave/cpp_iteration_context.hpp"

#include "details/wave_context_policies/TraceIncludes.h"

#include <set>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace fs = boost::filesystem;

circinc::SourceManager::SourceManager(std::vector<fs::path> const & include_directories,
                                      std::vector<fs::path> const & sysinclude_directories,
                                      fs::path const &entry_point) {

    std::set< fs::path > files_to_process, processed_files;
    files_to_process.insert(entry_point);

    // While there is at least one file to process
    do {
        auto     const & begin_iterator(files_to_process.begin());
        fs::path const & file_path{*begin_iterator};

        auto const included_files = this->get_included_files(include_directories,
                                                             sysinclude_directories,
                                                             file_path);

        // Mark the current file as processed
        processed_files.insert(file_path);

        // Update the graph structure
        m_graph.add_relations(file_path, included_files);

        // For all the dependencies discovered, pick the ones we never saw
        std::set< fs::path > new_file_paths_to_visit;
        std::set_difference(included_files.begin(), included_files.end(),
                            processed_files.begin(), processed_files.end(),
                            std::inserter(new_file_paths_to_visit, new_file_paths_to_visit.begin()));

        // Finally erase the file that is processed and update the files to process.
        files_to_process.erase(begin_iterator);
        files_to_process.insert(new_file_paths_to_visit.begin(), new_file_paths_to_visit.end());
    } while( ! files_to_process.empty() );

}

TraceIncludes::files_container_type circinc::SourceManager::get_included_files(
        std::vector<fs::path> const &include_directories,
        std::vector<fs::path> const &sysinclude_directories,
        fs::path const &file_path) {
    // First read the whole file into a string to give this string to boost::wave.
    fs::ifstream instream(file_path);
    std::string instring;

    if (!instream.is_open())
        throw std::runtime_error(std::string("Could not open input file: ") + file_path.string());

    instream.unsetf(std::ios::skipws);
    instring = std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                           std::istreambuf_iterator<char>());

    // Declare types helpers for lexer and context.
    using lex_iterator_type = boost::wave::cpplexer::lex_iterator< boost::wave::cpplexer::lex_token<> >;
    using context_type =  boost::wave::context< std::string::iterator,
            lex_iterator_type,
            boost::wave::iteration_context_policies::load_file_to_string,
            TraceIncludes >;

    // Create the #included file tracker.
    TraceIncludes::files_container_type included_files;
    TraceIncludes tracer(included_files);

    // Create the context and add all the include directories
    context_type context( instring.begin(), instring.end(), file_path.c_str(), tracer );
    for(auto const & include_path : include_directories)
        context.add_include_path(include_path.c_str());
    for(auto const & sysinclude_path : sysinclude_directories)
        context.add_sysinclude_path(sysinclude_path.c_str());
    context.set_language(boost::wave::language_support::support_option_single_line, false);

    // Generate the tokens and fill "included_files"
    for(auto const & _ : context) {}

    return included_files;
}

void circinc::SourceManager::output_dependencies_to_remove(CycleRemovingAlgorithmType const &algorithm,
                                                           std::ostream &os) const {
    std::vector<Edge> const cycles_to_remove( algorithm(m_graph.get_cycles()) );

    if(cycles_to_remove.empty()) {
        os << "No circular dependencies! Well done!" << std::endl;
        return;
    }

    os << "To delete the circular dependencies, break the dependencies listed below:\n";
    for(Edge const & edge : cycles_to_remove)
        os << m_graph.get_file_path(edge.first) << " -> " << m_graph.get_file_path(edge.second) << "\n";
}
