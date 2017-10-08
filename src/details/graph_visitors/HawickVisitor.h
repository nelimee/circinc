#ifndef CIRCINC_GRAPHVISITORS_HAWICKVISITOR_H
#define CIRCINC_GRAPHVISITORS_HAWICKVISITOR_H

#include "details/graph_types.h"

/**
 * Class implementing the Visitor model for the boost::hawick_circuits procedure.
 *
 * This Visitor implement a cycle method that will be called by the boost::hawick_circuit
 * procedure when a cycle is found. The instance will be copied (passed by value), so we need
 * to store a reference and not a value.
 */
class HawickVisitor {

public:

    /**
     * Constructor of the Hawick visitor.
     * @param cycles The structure that will store the cycles found by the algorithm.
     * This parameter should be given by reference because the visitors are copied
     * by the hawick algorithm's implementation.
     */
    explicit HawickVisitor(CyclesContainer &cycles);

    /**
     * Method called by the Hawick algorithm when a cycle is found.
     * @param cycle The cycle detected by the algorithm.
     */
    void cycle(Cycle const &cycle, Graph const &);

private:

    CyclesContainer &_cycles;
};


#endif //CIRCINC_GRAPHVISITORS_HAWICKVISITOR_H
