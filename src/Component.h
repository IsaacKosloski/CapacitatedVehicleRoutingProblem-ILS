/*Created by Isaac on 30/01/2025.*/
#pragma once

#include <bits/stdc++.h> //This library is used to get a lot of functions and methods used by all code

using namespace std;
using ::string;

class Component
{

public:
    int             numberOfComponents{}; // Components of distance type format
    vector<double>  positionComponents; // Graph coordinates components

    Component();
    explicit Component(vector<double> &positionComponents);

    vector<double> getPosition();
};