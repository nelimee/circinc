#include "boost/program_options.hpp"
#include "boost/program_options/version.hpp"
#include "boost/wave/wave_version.hpp"
#include "boost/filesystem/config.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/convenience.hpp"


#include <iostream>

#include "src/SourceManager.h"
#include "details/algorithms/greedy.h"

#define CIRCINC_VERSION_STRING "0.1"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, char const * const argv[]) {

    using filenames = std::vector< fs::path >;

    fs::path const current_directory{fs::current_path()};

    // Input arguments parsing.

    // First we describe all the possible named options
    po::options_description generic_options("Generic options");
    po::options_description config_options("Configuration");
    po::options_description hidden_options("Hidden options");
    generic_options.add_options()
            ("help,h",                                                         "Produce help message.")
            ("version,v",                                                      "Output the version and exit.");
    config_options.add_options()
            ("include-directory,I", po::value< filenames >()
                    ->default_value( filenames{current_directory}, current_directory.string() ),                      "Directories that will be searched for user #included files.")
            ("sysinclude-directory", po::value< filenames >()
                    ->default_value( filenames{current_directory}, current_directory.string() ),                      "Directories that will be searched for system #included files.");
    hidden_options.add_options()
            ("entry_point",      po::value< std::string >(),                   "The entry point of the program.");
    // We describe all the possible positional options.
    po::positional_options_description positional_description;
    positional_description.add("entry_point", 1);

    // We aggregate our options into groups
    po::options_description all_options("Allowed options");
    po::options_description visible_options("Allowed options");
    all_options.add(generic_options).add(config_options).add(hidden_options);
    visible_options.add(generic_options).add(config_options);

    // We create the variable map that will receive the values.
    po::variables_map vmap;

    // And we store the values in the variable map
    po::store(po::command_line_parser(argc, argv)
                      .options(all_options)
                      .positional(positional_description).run(),
              vmap);
    po::notify(vmap);

    // We react to our options and check their validity.
    if(vmap.count("help")) {
        std::cout << visible_options;
        return 0;
    }
    if(vmap.count("version")) {
        std::cout << "CircInc " << (CIRCINC_VERSION_STRING) << " build with: " << std::endl
                  << "\tboost::wave " << (BOOST_WAVE_VERSION_MAJOR) << "." << (BOOST_WAVE_VERSION_MINOR) << "."
                  << (BOOST_WAVE_VERSION_SUBMINOR) << std::endl
                  << "\tboost::program_options " << (BOOST_PROGRAM_OPTIONS_VERSION) << std::endl
                  << "\tboost::filesystem " << (BOOST_FILESYSTEM_VERSION) << std::endl
                  << "\tboost::graph " << "Last version available" << std::endl;
        return 0;
    }
    if( !vmap.count("entry_point") ) {
        std::cout << "You should give the entry point of the program as a positional argument." << std::endl;
        return 0;
    }

    // And start the analysis.
    circinc::SourceManager source_manager(vmap["include-directory"].as< filenames >(),
                                          vmap["sysinclude-directory"].as< filenames >(),
                                          vmap["entry_point"].as< std::string >());

    source_manager.output_dependencies_to_remove(greedy_algorithm, std::cout);

}