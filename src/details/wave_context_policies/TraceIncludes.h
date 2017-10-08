#ifndef CIRCINC_TRACEINCLUDES_H
#define CIRCINC_TRACEINCLUDES_H

#include "boost/wave/cpplexer/cpp_lex_token.hpp"
#include "boost/wave/preprocessing_hooks.hpp"

#include <set>
#include <string>

#include <iostream>

class TraceIncludes :
        public boost::wave::context_policies::default_preprocessing_hooks
{

public:

    using files_container_type = std::set< std::string >;

    explicit TraceIncludes(files_container_type & files)
            : m_files(files)
    { }

    template <typename ContextT>
    bool found_include_directive(ContextT & context,
                                 std::string const & filename,
                                 bool /*include_next*/)
    {
        bool is_sysinclude{filename.front() == '<'}, is_normalinclude{filename.front() == '"'};
        std::string complete_filename;

        if(is_normalinclude || is_sysinclude) {
            std::string included_file{filename.substr(1, filename.size() - 2)};
            std::string dir_path, native_name;

            if(this->locate_include_file(context, included_file, is_sysinclude, nullptr, dir_path, native_name))
                m_files.insert(included_file);
//            else
//                std::cout << "Couldn't locate " << included_file << std::endl;
        }
        else
            m_files.insert(filename);

        /* We don't want to #include the file and so recursively find the #included files.
         * We only want to know the files that are DIRECTLY #included. Normally we specify this
         * behaviour on the top level, with the context's set_max_include_nesting_depth method,
         * but we need to return a boolean here and with this we ensure that we will have the
         * correct behaviour.
         */
        return true;
    }

private:

    files_container_type & m_files;
};


#endif //CIRCINC_TRACEINCLUDES_H
