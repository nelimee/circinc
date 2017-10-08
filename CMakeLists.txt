cmake_minimum_required(VERSION 3.8)
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

add_subdirectory(.cppan)
include_directories(src)

add_executable(circinc main.cpp ${SOURCE_FILES})

target_link_libraries(circinc __cppan)
