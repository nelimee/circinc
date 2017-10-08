#include "HawickVisitor.h"

HawickVisitor::HawickVisitor(CyclesContainer &cycles)
        : _cycles(cycles) {}

void HawickVisitor::cycle(Cycle const &cycle, Graph const &) {
    this->_cycles.emplace_front();
    for (std::size_t i{1}; i < cycle.size(); ++i)
        this->_cycles.front().insert(Edge(cycle[i - 1], cycle[i]));
}
