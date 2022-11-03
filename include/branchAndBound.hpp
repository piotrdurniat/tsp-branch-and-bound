#ifndef BRANCH_AND_BOUND_H
#define BRANCH_AND_BOUND_H
#include "GraphMatrix.hpp"
#include <iostream>
#include <vector>
#include "Path.hpp"

class BranchAndBound
{
private:
    // Array holding minimum weights of all out-going edges for every vertex
    static int *minWeights;

    static GraphMatrix *graph;
    static int startingVertex;

    static int upperBound;

    static int firstImprovement;

    /**
     * @brief Solves the TSP using B&B algorithm
     *
     * @param vertex current vertex index
     * @param lowerBound lowerBound for the current vertex
     */

    static void printMinWeights();

    static void printPath(std::vector<int> path);

    static void printImprovement(int weight);

public:
    /**
     * @brief Finds the shortest Hamiltonian path in the graph using the Branch and Bound algorithm
     *
     * @param graph the graph on which the algorithm will be executed
     * @param startingVertex Index of the starting vertex
     */
    static Path execute(GraphMatrix *graph, int startingVertex);
};

#endif
