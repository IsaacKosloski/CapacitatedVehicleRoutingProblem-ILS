/*Created by Isaac on 30/01/2025.*/

#include "Functions.h"
#include "Solver.h"

#define MAX_ITERATIONS 1000


int main(int argc, char **argv)
{
    // Setting Instance, Solver, and Solution
    auto *cvrp = new CVRP(argv[1]);
    auto *solver = new Solver();
    auto *solution = new Solution();
    auto *bestSolution = new Solution();

    //Getting the current time before the algorithm starts
    auto start = chrono::high_resolution_clock::now();

    // Solving the Problem by test and getting the best solution
    solver->initialSolution_Greedy(cvrp, solution);
    //solver->localSearch_TwoOpt(cvrp, solution, bestSolution);
    solver->localSearch_TwoOpt (cvrp, solution, bestSolution);
    for(int i = 0; i < MAX_ITERATIONS; i++)
    {
        solver->pertubation_DoubleBridge(cvrp, bestSolution, solution);
        solver->localSearch_TwoOpt(cvrp, solution, solution);
        solver->acceptanceCriterion_BestSolution(bestSolution, solution);
    }


    //Getting the current time after the algorithm ends
    auto finish = chrono::high_resolution_clock::now();

    //Calculating the time duration by subtracting the start time from the finish time
    chrono::duration<double> elapsed = finish - start;

    // Printing the result on an output file
    bestSolution->printSolution(argv[2], elapsed.count(), cvrp->nodesDimension, cvrp->nodesDimension);

    // Memory flush
    delete bestSolution;
    delete solution;
    delete solver;
    delete cvrp;

    // EOF
    return 0;
}