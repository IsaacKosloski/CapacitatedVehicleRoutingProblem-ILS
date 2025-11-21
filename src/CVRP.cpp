/*Created by Isaac on 30/01/2025.*/

#include "CVRP.h"

CVRP::CVRP(string fileName)
{
    loadInstance(fileName);
}

CVRP::~CVRP()
{
    delete scanner;
}

void CVRP::loadInstance(string fileName)
{
    scanner = new Scanner(fileName);

    this->nodesDimension = scanner->dimensionOfNodes;
    this->capacityOfVehicle = scanner->capacityOfVehicles;
    this->distanceMatrix = scanner->nodesDistance;
    this->nodes = scanner->nodes;

    // Converte depotID de 1-indexed para 0-indexed
    this->depotID = scanner->depot - 1;

    // Ajusta IDs dos nós para 0-indexed
    for (int i = 0; i < nodesDimension; ++i)
    {
        nodes[i].ID = i;
    }

    // Marca depósito corretamente
    if (depotID >= 0 && depotID < nodesDimension)
    {
        nodes[depotID].isDepot = true;
        nodes[depotID].isAvailable = false;
        nodes[depotID].demand = 0;
    }

    cout << "Instance loaded successfully!" << endl;
    cout << "  Nodes: " << nodesDimension << endl;
    cout << "  Capacity: " << capacityOfVehicle << endl;
    cout << "  Depot: " << depotID << " (0-indexed)" << endl;
}
