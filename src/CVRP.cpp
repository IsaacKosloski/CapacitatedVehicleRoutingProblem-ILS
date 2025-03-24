/*Created by Isaac on 30/01/2025.*/

#include "CVRP.h"

// Constructor that initializes the CVRP instance by loading data from the given file
CVRP::CVRP(string fileName)
{
    loadInstance(fileName);
}

// Method to load instance data (from file or manually)
void
CVRP::loadInstance(string fileName)
{
    scanner = new Scanner(fileName);

    this->nodesDimension = scanner->dimensionOfNodes;
    this->capacityOfVehicle = scanner->capacityOfVehicles;
    this->depotID = scanner->depot;

    for (auto n : scanner->nodesDistance)
        this->distanceMatrix.push_back(n);

    for (auto n : scanner->nodes)
    {
        this->nodes.push_back(n);
        n.isDepot ? depotID = n.ID : 0;
    }



    delete scanner;
}
