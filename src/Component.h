/*Created by Isaac on 30/01/2025.*/
#pragma once

#include <vector>
#include <string>

using namespace std;

class Component
{

public:
    int             numberOfComponents{}; // Components of distance type format
    vector<double>  positionComponents; // Graph coordinates components

    Component();
    explicit Component(vector<double> &positionComponents);

    vector<double> getPosition();
};