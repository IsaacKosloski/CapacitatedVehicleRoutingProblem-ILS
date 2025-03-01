/*Created by Isaac on 28/01/2025.*/

#include "Node.h"

// Default constructor
Node::Node()
{
    // Initializes an empty node
}

// Constructor for creating a node with an ID and demand
Node::Node(int ID, int demand)
{
    this->ID = ID;

    this->demand = demand;
    if (demand == 0)
    {
        this->isDepot = true;
        this->isAvailable = false;
    }
    else
    {
        this->isDepot = false;
        this->isAvailable = true;
    }
}
