/*Created by Isaac on 30/01/2025.*/
#pragma once

#include "Scanner.h"

// Represents the problem instance, containing all data related to nodes, vehicles, and distances.
class CVRP
{
public:
    vector<Node>    nodes;              // All nodes, including depot and customers.
    vector<double>  distanceMatrix;     // Matrix of distances between nodes.
    int             depotID;            // ID of the depot node.
    int             nodesDimension;     // The dimension of clients.
    int             capacityOfVehicle;  // Maximum capacity of the vehicle.
    Scanner         *scanner;           // Scanner of data file.

    CVRP(string fileName);

    void loadInstance(string fileName); // Method to load instance data (from file or manually).
};
