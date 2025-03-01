/*Created by Isaac on 28/01/2025.*/

#include "Vehicle.h"

Vehicle::Vehicle(int capacity, int currentLoad)
{
    this->capacity = capacity;
    this->currentLoad = currentLoad;
}

// Method to add a node to the route (checks capacity constraints).
/*void
Vehicle::addNode(Node newNode)
{
    this->currentLoad + newNode.demand < this->capacity ? this->route.push_back(newNode) : this->route.push_back();
}*/

// Reset function to clear the route and load.
void
Vehicle::reset()
{
    this->route.clear();
    this->currentLoad = 0;
}

// Method to compute the cost of the route (requires a distance matrix).
double
Vehicle::computeCost(vector<int>& distanceMatrix, int dimension)
{
    double cost{};

    if (route.empty()) return cost; // Handle the empty route

    for (auto n = this->route.begin(); n != this->route.end() - 1; n++)
        cost = distanceMatrix[(n->ID * dimension) + next(n)->ID];

    return cost;
}


