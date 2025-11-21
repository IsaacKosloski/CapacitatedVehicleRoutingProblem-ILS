/*Created by Isaac on 03/02/2025.*/
#pragma once

#include "Solution.h"
#include <algorithm>
#include <iostream>

class Solver
{
public:
    CVRP *instance;         // The problem instance.
    Solution *bestSolution; // Stores the best solution found.

    // ILS Solver Constructor
    Solver();

    // Initial Solution
    void initialSolution_RandomInsertion();
    void initialSolution_Greedy(CVRP *instance , Solution *initialSolution);
    void initialSolution_NNH();
    void initialSolution_ClarkeWright();

    // Local Search
    void localSearch_TwoOpt(CVRP *instance , Solution *initialSolution, Solution *bestSolution);
    void localSearch_ThreeOpt(CVRP *instance , Solution *initialSolution, Solution *bestSolution);
    void localSearch_SwapStar(CVRP *instance, Solution *solution, int chain_length);
    void localSearch_OrOpt();
    void localSearch_Realocation();
    void localSearch_LinKernighan();

    double calculate_swap_star_delta(CVRP *instance, const vector<int>& route1, const vector<int>& route2, int i, int j, int k);

    // Perturbation
    void pertubation_DoubleBridge(CVRP *instance, Solution *bestSolution, Solution *perturbedSolution);
    void pertubation_LargeNeighborhoodSearch();

    // Acceptance Criterion
    void acceptanceCriterion_BestSolution(Solution *bestSolution, Solution *newSolution);



};

