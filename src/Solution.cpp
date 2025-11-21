/*Created by Isaac on 03/02/2025.*/

#include "Solution.h"

Solution::Solution()
    : fleetSize(0)
    , totalCost(0.0)
{
}

double Solution::computeCost(int dimensionOfNodes, vector<double> &matrixCost)
{
    totalCost = 0.0;
    routesCosts.clear();

    for (const auto &route : routes)
    {
        if (route.size() < 2)
        {
            cerr << "Warning: Route is too short to compute cost!" << endl;
            continue;
        }

        double routeCost = 0.0;

        for (size_t i = 0; i < route.size() - 1; ++i)
        {
            auto fromNode = route[i];
            auto toNode = route[i + 1];

            if (fromNode < 0 || fromNode >= dimensionOfNodes ||
                toNode < 0 || toNode >= dimensionOfNodes)
            {
                cerr << "Error: Invalid node index in route! [" << fromNode
                     << " -> " << toNode << "]" << endl;
                continue;
            }

            int matrixIndex = fromNode * dimensionOfNodes + toNode;
            if (matrixIndex >= 0 && matrixIndex < (int)matrixCost.size())
            {
                routeCost += matrixCost[matrixIndex];
            }
        }

        routesCosts.push_back(routeCost);
        totalCost += routeCost;
    }

    return totalCost;
}

void Solution::printSolution(const char* fileName, double elapsedTime,
                             int iterations, int dimensionOfNodes)
{
    ofstream outputFile(fileName);

    if (outputFile.is_open())
    {
        int routeNum = 1;
        for (const auto &route : routes)
        {
            outputFile << "Route #" << routeNum << ":";

            // Print only customer nodes (skip depot at start and end)
            for (size_t i = 1; i < route.size() - 1; ++i)
            {
                outputFile << " " << route[i];
            }
            outputFile << endl;
            routeNum++;
        }

        outputFile << "Cost " << static_cast<int>(round(totalCost)) << endl;
        outputFile << "Time " << fixed << setprecision(4) << elapsedTime << endl;

        outputFile << "\n\n * Other infos *\n\n Iterations: " << iterations;
        outputFile << " Time: " << fixed << setprecision(4) << elapsedTime
                   << " sec - " << elapsedTime/60 << " min - "
                   << elapsedTime/3600 << " hours" << endl;
        outputFile << floor(elapsedTime/3600) << " h "
                   << ((elapsedTime/3600) - floor(elapsedTime/3600)) * 60
                   << " min" << endl;
        outputFile << "Problem dimension: " << dimensionOfNodes << endl;
    }

    outputFile.close();
}
