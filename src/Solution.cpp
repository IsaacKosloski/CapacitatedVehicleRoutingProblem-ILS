/*Created by Isaac on 03/02/2025.*/

#include "Solution.h"

Solution::Solution()
{
    int currentLoad = 0;
    int vehicleCount = 0;
}

double
Solution::computeCost(int dimensionOfNodes, vector<double> &matrixCost)
{
    totalCost = 0.0;
    routesCosts.clear();  // Clear previous costs if needed

    for (const auto &route : routes)
    {
        double routeCost = 0.0;

        for (size_t i = 0; i < route.size() - 1; ++i)
        {
            auto fromNode = route[i];
            auto toNode = route[i + 1];

            // Ensure matrix index is valid
            routeCost += matrixCost[fromNode * dimensionOfNodes + toNode];
        }

        routesCosts.push_back(routeCost);
        totalCost += routeCost;
    }

    return totalCost;
}

void
Solution::printSolution(const char* fileName, double elapsedTime, int iterations, int dimensionOfNodes)
{
    //Writing the last result in the file
    ofstream outputFile(fileName);
    if (outputFile.is_open())
    {
        //Route #1: 21 31 19 17 13 7 26
        int route = 1;
        for (const auto &row : routes)
        {
            outputFile << "Route #" << route << ": ";
            for (const auto &element : row)
                outputFile << element << " ";
            outputFile << endl;
            route++;
        }
        outputFile << endl;
        outputFile << "Cost " << totalCost << endl;
        outputFile << "Time " << fixed << setprecision(4) << elapsedTime << endl;

        outputFile << "\n\n * Another infos *\n\n Iterations: " << iterations;
        outputFile << " Time: " << fixed << setprecision(4) << elapsedTime << " sec - " << elapsedTime/60 << " min - " << elapsedTime/3600 << " horas" << endl;
        outputFile << floor(elapsedTime/3600) << " h " << ((elapsedTime/3600) - floor(elapsedTime/3600)) * 60 << " min" << endl ;
        outputFile << "Problem dimension: " << dimensionOfNodes << endl;

    }
    outputFile.close();
}


