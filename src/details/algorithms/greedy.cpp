#include "greedy.h"


std::vector<Edge> greedy_algorithm(CyclesContainer cycles) {

    std::vector<Edge> edges_to_remove;

    if (cycles.empty())
        return edges_to_remove;

    using ConstIt = CyclesContainer::const_iterator;

    do {

        // For each edge of each cycle, record every cycle in which this edge appears.
        std::map<Edge, std::vector<ConstIt> > edge_appears_in_cycles;
        for (ConstIt previous = cycles.before_begin(), current = cycles.begin();
             current != cycles.end();
             ++current, previous = current) {
            for (Edge const &current_edge : *current) {
                edge_appears_in_cycles[current_edge].push_back(previous);
            }
        }

        using EdgeAndCycles = std::pair<Edge, std::vector<ConstIt> >;

        // Then take the edge that appears in the most cycles.
        Edge const edge_to_remove{std::max_element(edge_appears_in_cycles.begin(),
                                                   edge_appears_in_cycles.end(),
                                                   [](EdgeAndCycles const &lhs, EdgeAndCycles const &rhs) {
                                                       return lhs.second.size() < rhs.second.size();
                                                   })->first};

        // And delete all the cycles that contain this edge.
        cycles.remove_if(
                [&edge_to_remove](CycleEdgeSet const &cycle) { return cycle.find(edge_to_remove) != cycle.end(); });

        edges_to_remove.push_back(edge_to_remove);

    } while (!cycles.empty());

    return edges_to_remove;
}