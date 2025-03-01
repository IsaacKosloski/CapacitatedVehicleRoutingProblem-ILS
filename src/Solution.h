/*Created by Isaac on 03/02/2025.*/
#pragma once

#include "CVRP.h"

class Solution
{
public:

    int fleetSize;
    double totalCost;
    vector<double> routesCosts;  // Total cost of the solution.
    vector<vector<int>> routes; // Each route corresponds to a vehicle’s route.

    Solution();

    double computeCost(int dimensionOfNodes, vector<double> &matrixCost);
    void printSolution(const char* fileName, double elapsedTime, int iterations, int dimensionOfNodes);

};
