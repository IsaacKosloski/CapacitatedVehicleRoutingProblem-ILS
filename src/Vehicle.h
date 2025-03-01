/*Created by Isaac on 28/01/2025.*/
#pragma once

#include <vector>
#include "Node.h"

// Represents a vehicle in the fleet.
class Vehicle
{
public:
    int         capacity;       // Maximum capacity of the vehicle.
    int         currentLoad;    // Current load of the vehicle.
    vector<Node> route;          // The sequence of nodes visited by the vehicle.

    Vehicle(int capacity, int currentLoad);
    void addNode(Node newNode); // Method to add a node to the route (checks capacity constraints).
    void reset();               // Reset function to clear the route and load.
    double computeCost(vector<int>& distanceMatrix, int dimension);  // Method to compute the cost of the route (requires a distance matrix).
};


