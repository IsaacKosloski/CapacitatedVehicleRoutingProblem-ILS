/*Created by Isaac on 30/01/2025.*/

#include "Component.h"

// Default constructor
Component::Component()
{
    // No specific initialization
}


// Constructor that initializes the position components by copying values from the input vector
Component::Component(vector<double> &positionComponents)
{
    for (auto n : positionComponents)
        this->positionComponents.push_back(n);
}

// Getter method to retrieve the position components
vector<double> Component::getPosition() {
    return positionComponents;
}
