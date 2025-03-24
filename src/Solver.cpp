/* Created by Isaac on 03/02/2025. */

#include "Solver.h"
#include <limits>

Solver::Solver()
{

}

/*/ Function to generate an initial solution using Greedy algorithm
void
Solver::initialSolution_Greedy(CVRP *instance , Solution *initialSolution)
{
    cout << "\n## INITIAL SOLUTION ##\n" << endl;
    int currentCapacity = 0; // To Update each route capacity
    int counterVehicles = 0; // To Update the vehicles amount
    int nextCustomer    = 0; // To Update the customers appended to the solution
    int minimumCostumer = 0; // To Append the next customer
    double minimumCost  = numeric_limits<double>::max();
    initialSolution->routes.resize(instance->nodesDimension, vector<int>(instance->nodesDimension, 0));

    // Setting the first node as the Depot ID
    initialSolution->routes[counterVehicles][0] = instance->depotID;
    initialSolution->totalCost = 0;

    for (int j = 0; j < instance->nodesDimension; j++)
    {
        cout << "J: " << j << endl;
        for (int i = 0; i < instance->nodesDimension; i++)
        {
            cout << "I: " << i << " ";
            if (currentCapacity + instance->nodes[i].demand > instance->capacityOfVehicle)
            {
                cout << "[1 true] ";
                counterVehicles++;
                nextCustomer = 0;
                initialSolution->routes[counterVehicles][0] = instance->depotID;
                cout << "\n#" << counterVehicles << ": " << initialSolution->routes[counterVehicles][0] << " ";
            }
            else
                cout << "[1 false] ";

            if (instance->distanceMatrix[((initialSolution->routes[counterVehicles][nextCustomer]) * instance->nodesDimension) + i] < minimumCost)
            {
                cout << "[2 true] ";
                minimumCostumer = i;
                minimumCost = instance->distanceMatrix[(j * instance->nodesDimension) + i];
            }
            else
                cout << "[2 false] ";
            cout << endl;
        }
        cout << endl;
        initialSolution->routes[counterVehicles][nextCustomer] = minimumCostumer;
        //cout << initialSolution->routes[counterVehicles][nextCustomer] << " ";
        currentCapacity += instance->nodes[minimumCostumer].demand;
        initialSolution->totalCost += instance->distanceMatrix[(j * instance->nodesDimension) + minimumCostumer];
        nextCustomer++;
    }
    initialSolution->fleetSize = counterVehicles;
}*/

/*/ Function to generate an initial solution using Nearest Neighbor heuristic
void
Solver::initialSolutionNNH(CVRP *instance , Solution *solution)
{
    std::vector<Solution> routes;
    std::vector<Node> remaining_customers = customers;

    while (!remaining_customers.empty()) {
        Solution route;
        int current_capacity = 0;
        Node current_location = {0, 0, 0, 0}; // Depot

        while (!remaining_customers.empty() && current_capacity < vehicle_capacity) {
            // Find the nearest customer
            auto nearest = std::min_element(remaining_customers.begin(), remaining_customers.end(),
                                            [&](const Customer& a, const Customer& b) {
                                                return distance(current_location, a) < distance(current_location, b);
                                            });

            if (current_capacity + nearest->demand <= vehicle_capacity) {
                route.push_back(*nearest);
                current_capacity += nearest->demand;
                current_location = *nearest;
                remaining_customers.erase(nearest);
            } else {
                break;
            }
        }

        routes.push_back(route);
    }

}*/


void Solver::initialSolution_Greedy(CVRP *instance , Solution *initialSolution)
{
    int currentCapacity = 0;
    int counterVehicles = 0;
    double minimumCost;
    int minimumCostumer = -1;

    vector<bool> visited(instance->nodesDimension, false);

    initialSolution->routes.clear();
    initialSolution->routes.push_back(vector<int>()); // Start with one vehicle
    initialSolution->routes[counterVehicles].push_back(instance->depotID); // Start at depot
    initialSolution->totalCost = 0;

    for (int j = 0; j < instance->nodesDimension - 1; j++) // Exclude depot
    {
        minimumCost = numeric_limits<double>::max();
        minimumCostumer = -1;

        int lastCustomer = initialSolution->routes[counterVehicles].back();

        for (int i = 1; i < instance->nodesDimension; i++) // Start from 1 to exclude depot
        {
            if (visited[i]) continue; // Skip if already assigned

            double distance = instance->distanceMatrix[(lastCustomer * instance->nodesDimension) + i];

            if (currentCapacity + instance->nodes[i].demand <= instance->capacityOfVehicle && distance < minimumCost)
            {
                minimumCostumer = i;
                minimumCost = distance;
            }
        }

        if (minimumCostumer == -1)
        {
            // Close current route by returning to depot
            initialSolution->routes[counterVehicles].push_back(instance->depotID);
            counterVehicles++;
            initialSolution->routes.push_back(vector<int>());
            initialSolution->routes[counterVehicles].push_back(instance->depotID);
            currentCapacity = 0;
            j--; // Retry this iteration with a new vehicle
            continue;
        }

        initialSolution->routes[counterVehicles].push_back(minimumCostumer);
        currentCapacity += instance->nodes[minimumCostumer].demand;
        visited[minimumCostumer] = true;
        initialSolution->totalCost += minimumCost;
    }

    // Close last vehicle's route
    initialSolution->routes[counterVehicles].push_back(instance->depotID);
    initialSolution->fleetSize = counterVehicles + 1;
}


void Solver::localSearch_TwoOpt(CVRP *instance, Solution *initialSolution, Solution *bestSolution)
{
    *bestSolution = *initialSolution; // Start with the initial solution
    bool improvement = true;

    while (improvement)
    {
        improvement = false;

        // Iterate through all routes in the solution
        for (auto &route : bestSolution->routes)
        {
            int routeSize = route.size();
            if (routeSize < 4) continue; // Ignore small routes (no swaps possible)

            for (int i = 1; i < routeSize - 2; i++)
            {
                for (int j = i + 1; j < routeSize - 1; j++)
                {
                    // Nodes before and after swap section
                    int A = route[i - 1], B = route[i];
                    int C = route[j], D = route[j + 1];

                    // Current cost of the two edges
                    double currentCost = instance->distanceMatrix[A * instance->nodesDimension + B] +
                                         instance->distanceMatrix[C * instance->nodesDimension + D];

                    // New cost if we swap (reverse the segment)
                    double newCost = instance->distanceMatrix[A * instance->nodesDimension + C] +
                                     instance->distanceMatrix[B * instance->nodesDimension + D];

                    if (newCost < currentCost)
                    {
                        // Perform the swap by reversing the segment
                        reverse(route.begin() + i, route.begin() + j + 1);

                        // Update solution cost
                        bestSolution->computeCost(instance->nodesDimension, instance->distanceMatrix);

                        improvement = true;
                    }
                }
            }
        }
    }
}

void Solver::pertubation_DoubleBridge(CVRP *instance, Solution *bestSolution, Solution *perturbedSolution)
{
    // Step 1: Make a copy of the best solution
    *perturbedSolution = *bestSolution;

    // Step 2: Choose a route to modify
    vector<int> &route = perturbedSolution->routes[0]; // Select the first route for simplicity
    int routeSize = route.size();

    // Ensure that we have enough nodes for a Double Bridge Move
    if (routeSize < 8) return;

    // Step 3: Select 4 random split points ensuring proper separation
    int split1 = rand() % (routeSize / 4);
    int split2 = split1 + (routeSize / 4);
    int split3 = split2 + (routeSize / 4);
    int split4 = split3 + (routeSize / 4);

    // Step 4: Create the new order of segments
    vector<int> newRoute;
    newRoute.insert(newRoute.end(), route.begin(), route.begin() + split1);
    newRoute.insert(newRoute.end(), route.begin() + split3, route.begin() + split4);
    newRoute.insert(newRoute.end(), route.begin() + split2, route.begin() + split3);
    newRoute.insert(newRoute.end(), route.begin() + split1, route.begin() + split2);
    newRoute.insert(newRoute.end(), route.begin() + split4, route.end());

    // Step 5: Apply the new order to the perturbed solution
    perturbedSolution->routes[0] = newRoute;

    // Step 6: Recalculate cost after the change
    perturbedSolution->computeCost(instance->nodesDimension, instance->distanceMatrix);
}

void Solver::localSearch_ThreeOpt(CVRP *instance, Solution *initialSolution, Solution *bestSolution)
{
    *bestSolution = *initialSolution; // Copy the initial solution as the starting best
    double bestCost = bestSolution->totalCost;
    bool improvement = true;

    while (improvement)
    {
        improvement = false;

        // Iterate through all possible three-cut combinations
        for (size_t i = 1; i < bestSolution->routes[0].size() - 3; i++)
        {
            for (size_t j = i + 1; j < bestSolution->routes[0].size() - 2; j++)
            {
                for (size_t k = j + 1; k < bestSolution->routes[0].size() - 1; k++)
                {
                    Solution newSolution = *bestSolution; // Copy current solution
                    vector<int> &route = newSolution.routes[0];

                    // Generate different 3-opt swaps
                    vector<vector<int>> possibleRoutes = {
                            route, // Keep the original route
                            route, // Reverse (i, j)
                            route, // Reverse (j, k)
                            route, // Reverse (i, j) and (j, k)
                            route, // Reverse entire segment (i, k)
                    };

                    reverse(possibleRoutes[1].begin() + i, possibleRoutes[1].begin() + j);
                    reverse(possibleRoutes[2].begin() + j, possibleRoutes[2].begin() + k);
                    reverse(possibleRoutes[3].begin() + i, possibleRoutes[3].begin() + j);
                    reverse(possibleRoutes[3].begin() + j, possibleRoutes[3].begin() + k);
                    reverse(possibleRoutes[4].begin() + i, possibleRoutes[4].begin() + k);

                    for (auto &newRoute : possibleRoutes)
                    {
                        newSolution.routes[0] = newRoute;
                        newSolution.computeCost(instance->nodesDimension, instance->distanceMatrix);

                        if (newSolution.totalCost < bestCost)
                        {
                            *bestSolution = newSolution;
                            bestCost = newSolution.totalCost;
                            improvement = true;
                        }
                    }
                }
            }
        }
    }
}


void Solver::acceptanceCriterion_BestSolution(Solution *bestSolution, Solution *newSolution)
{
    // Compare total costs
    if (newSolution->totalCost < bestSolution->totalCost)
    {
        *bestSolution = *newSolution; // Update the best solution
        cout << "New best solution found with cost: " << bestSolution->totalCost << endl;
    }
    else
    {
        cout << "New solution rejected. Current best cost remains: " << bestSolution->totalCost << endl;
    }
}
