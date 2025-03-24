/*Created by Isaac on 03/02/2025.*/
#include "Functions.h"

void printInstance(CVRP *cvrp)
{
    int row(0);
    int col(0);
    /*info print*/
    cout << "Dimension: " << cvrp->nodesDimension << endl;
    cout << "Capacity:  " << cvrp->capacityOfVehicle << endl;
    cout << "Distances" << endl;
    cout << "COL[ " << col << " ]: \n" << endl;
    for (const auto &value : cvrp->distanceMatrix)
    {
        if (row == cvrp->nodesDimension)
        {
            row = 0;
            col++;
            cout << "\n\n" << endl;
            cout << "COL[ " << col << " ]: \n" << endl;
        }
        cout << value << " | ";
        row++;
    }

    cout << "\n\n" << endl;
    row = 0;
    for (const auto &value : cvrp->nodes)
    {
        cout << "Cli [ " << row << " ]: " << value.demand << endl;
        row++;
    }

    cout << "\n" << endl;
}

void printSolution(Solution *solution)
{
    int route = 1;
    auto routes = solution->routes;

    for (const auto &n : routes)
    {
        cout << "Route #" << route << ": ";
        for (const auto &m : n)
            cout << m << " ";
        cout << endl;
        route++;
    }
    cout << endl;
    cout << "Cost " << solution->totalCost << endl;
    cout << endl;
}