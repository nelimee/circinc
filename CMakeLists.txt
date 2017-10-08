cmake_minimum_required(VERSION 3.2)
project(circinc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/SourceManager.cpp
        src/SourceManager.h
        src/details/wave_context_policies/TraceIncludes.h
        src/DependencyGraph.cpp
        src/DependencyGraph.h
        src/details/graph_types.h
        src/details/graph_visitors/HawickVisitor.h
        src/details/graph_visitors/HawickVisitor.cpp
        src/details/algorithms/greedy.cpp
        src/details/algorithms/greedy.h)

# Search for the cppan tool
find_program(CPPAN_PROGRAM_COMMAND cppan)

include_directories(src)

add_executable(circinc main.cpp ${SOURCE_FILES})

if(DEFINED CPPAN_PROGRAM_COMMAND)
    add_subdirectory(.cppan)
    target_link_libraries(circinc __cppan)
else()
    # If you don't have cppan, remove this error and link the libraries listed in cppan.yml.
    message(FATAL_ERROR "CPPAN was not found on your machine. You should provide and link the libraries listed in cppan.yml by yourself."
            "Edit the CMakeLists.txt file to delete this warning and link the libraries.")
endif()