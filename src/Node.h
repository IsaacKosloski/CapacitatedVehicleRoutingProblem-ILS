/*Created by Isaac on 28/01/2025.*/
#pragma once

#include <bits/stdc++.h> //This library is used to get a lot of functions and methods used by all code
#include <vector>
using namespace std;

// Represents a customer or the depot.
class Node
{

public:
    int  ID;                  // Unique identifier of the node.
    int  demand;              // Demand of the node (0 for the depot).
    bool isDepot;             // Indicates if the node is the depot.
    bool isAvailable;         // Indicates if the node is available to use

    Node();
    Node(int ID, int demand);
};
