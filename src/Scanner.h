/*Created by Isaac on 30/01/2025.*/
#pragma once

#include "Component.h"
#include "Vehicle.h"
#include <string>
#include <sstream>

class Scanner
{
public:
    int dimensionOfNodes;
    int capacityOfVehicles;
    int col;
    int row;
    int depot;
    string fileName;
    vector<Component> components;
    vector<Node> nodes;
    vector<double> nodesDistance;

    explicit Scanner(string fileName);

    void readFile(const string &fileName, vector<Component> &components, vector<double> &nodesDistance, vector<Node> &nodes);
    bool specificationPart(string line);

};
