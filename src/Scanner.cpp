/*Created by Isaac on 30/01/2025.*/

#include "Scanner.h"

// Constructor that initializes the scanner and reads the instance file
Scanner::Scanner(string fileName)
{
    this->fileName = fileName;
    dimensionOfNodes = -1;   // Initializes the number of nodes to an undefined state
    capacityOfVehicles = -1; // Initializes vehicle capacity to an undefined state
    col = -1;                // Column counter initialization
    row = -1;                // Row counter initialization

    // Reads the file and extracts necessary data
    readFile(fileName, components, nodesDistance, nodes);
}

// Method to read instance file and populate data structures
void
Scanner::readFile(const string &fileName, vector<Component> &components, vector<double> &nodesDistance, vector<Node> &nodes)
{
    int ID = 1; // Node ID counter
    int demand; // Variable to store demand of a node
    vector<double> positionComponents(2); // Stores position components (e.g., x, y coordinates)
    string line{}; // Stores each line read from the file

    ifstream inputFile(fileName, ios::in);

    if (inputFile.is_open())
    {
        // Skip lines until reaching the specification part
        while(getline(inputFile, line) && !specificationPart((line)));

        if (dimensionOfNodes <= 0)
            throw std::runtime_error("Error: Invalid or missing DIMENSION in file.");

        // Resize vectors based on the number of nodes
        components.resize(dimensionOfNodes);
        nodesDistance.resize(dimensionOfNodes * dimensionOfNodes);

        // Read node coordinates
        while (getline(inputFile, line) && (line.find("DEMAND_SECTION") == string::npos))
        {
            istringstream ssLine(line);

            if (!(ssLine >> ID >> positionComponents[0] >> positionComponents[1]))
            {
                cerr << "Error: Malformed line: " << line << endl;
                throw std::runtime_error("Invalid data format.");
            }

            if (ID - 1 >= 0 && ID - 1 < (int)components.size())
            {
                components[ID - 1] = Component(positionComponents);
            }
            else
            {
                cerr << "Error: ID out of range (" << ID << ")" << endl;
                throw std::runtime_error("Invalid node ID in input file.");
            }
        }

        // Compute the distance matrix
        for (row = 0; row < dimensionOfNodes; row++)
        {
            for (col = 0; col < dimensionOfNodes; col++)
            {
                vector<double> posA = components[row].getPosition();
                vector<double> posB = components[col].getPosition();

                if ((row * dimensionOfNodes) + col < (int)nodesDistance.size())
                    nodesDistance[(row * dimensionOfNodes) + col] = sqrt(pow((posA[0] - posB[0]), 2) + pow((posA[1] - posB[1]), 2));
                else
                    cerr << "Warning: nodesDistance index out of range!" << endl;

                //nodesDistance[(row * dimensionOfNodes) + col] = sqrt((pow((components[row].positionComponents[0] - components[col].positionComponents[0]), 2) + (pow((components[row].positionComponents[1] - components[col].positionComponents[1]), 2))));
            }
        }

        // Read node demands
        while(getline(inputFile, line) && (line.find("DEPOT_SECTION") == string::npos))
        {
            istringstream ssLine(line);
            if (ssLine >> ID >> demand)
            {
                nodes.push_back(Node(ID, demand));
            }
            else
            {
                cerr << "Warning: Invalid line format in DEMAND_SECTION: " << line << endl;
            }
        }

        // Identify depot node
        getline(inputFile, line);
        istringstream ssLine(line);
        ssLine >> ID;
        this->depot = ID;
        if (ID - 1 >= 0 && ID - 1 < (int)nodes.size())
        {
            nodes[ID - 1].isDepot = true;
            nodes[ID - 1].isAvailable = false;
        }
        else
        {
            cerr << "Error: Depot index out of range: " << ID - 1 << endl;
        }/**/
    }
    else
    {
        cerr << "Error: Could not open file " << fileName << endl;
        throw std::runtime_error("Unable to open file");
    }

    inputFile.close();
}

// Parses the file header to extract instance parameters
bool
Scanner::specificationPart(string line)
{
    size_t found = 0;
    string stringData{};

    if (line.find("DIMENSION") != string::npos)
    {
        found = line.find(": ");
        if (found != string::npos)
        {
            stringData = line.substr(found + 2);
            dimensionOfNodes = stoi(stringData);

        }
    }
    else if (line.find("CAPACITY") != string::npos)
    {
        found = line.find(": ");
        if (found != string::npos)
        {
            stringData = line.substr(found + 2);
            capacityOfVehicles = stoi(stringData);
        }
    }
    else if (line.find("NODE_COORD_SECTION") !=string::npos)
        return true;

    return false;
}
