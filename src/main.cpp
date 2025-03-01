/*Created by Isaac on 30/01/2025.*/

#include "Functions.h"
#include "Solver.h"


int main(int argc, char **argv)
{
    // Setting Instance, Solver, and Solution
    auto *cvrp = new CVRP(argv[1]);
    auto *solver = new Solver();
    auto *solution = new Solution();

    //Getting the current time before the algorithm starts
    auto start = chrono::high_resolution_clock::now();

    // Generating All the Permutations
    solver->generatePermutations(cvrp->nodes);
    // Solving the Problem by test and getting the best solution
    solver->solve(cvrp, solution);

    //Getting the current time after the algorithm ends
    auto finish = chrono::high_resolution_clock::now();

    //Calculating the time duration by subtracting the start time from the finish time
    chrono::duration<double> elapsed = finish - start;

    // Printing the result on an output file
    solution->printSolution(argv[2], elapsed.count(), cvrp->nodesDimension, cvrp->nodesDimension);

    // Memory flush
    delete solution;
    delete solver;
    delete cvrp;

    // EOF
    return 0;
}