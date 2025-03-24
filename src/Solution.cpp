/*Created by Isaac on 03/02/2025.*/

#include "Solution.h"

Solution::Solution()
{

}

/*
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
}*/

double Solution::computeCost(int dimensionOfNodes, vector<double> &matrixCost)
{
    totalCost = 0.0;
    routesCosts.clear();  // Clear previous costs

    for (const auto &route : routes)
    {
        if (route.size() < 2) // Skip empty or incomplete routes
        {
            cerr << "Warning: Route is too short to compute cost!" << endl;
            continue;
        }

        double routeCost = 0.0;

        for (size_t i = 0; i < route.size() - 1; ++i)
        {
            auto fromNode = route[i];
            auto toNode = route[i + 1];

            // Ensure matrix index is valid
            if (fromNode < 0 || fromNode >= dimensionOfNodes || toNode < 0 || toNode >= dimensionOfNodes)
            {
                cerr << "Error: Invalid node index in route! [" << fromNode << " -> " << toNode << "]" << endl;
                continue; // Skip invalid edges
            }

            routeCost += matrixCost[fromNode * dimensionOfNodes + toNode];
        }

        // Ensure the route **returns** to the depot
        if (!route.empty() && route.back() != route.front())
        {
            routeCost += matrixCost[route.back() * dimensionOfNodes + route.front()];
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


