/* Created by Isaac on 03/02/2025. */

#include "Solver.h"
#include <limits>

Solver::Solver()
{

}

void Solver::initialSolution_Greedy(CVRP *instance , Solution *initialSolution)
{
    int currentCapacity = 0;
    int counterVehicles = 0;
    double minimumCost;
    int minimumCostumer = -1;

    vector<bool> visited(instance->nodesDimension, false);

    initialSolution->routes.clear();
    initialSolution->routeLoads.clear();
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
            initialSolution->routeLoads.push_back(currentCapacity);
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
    initialSolution->routeLoads.push_back(currentCapacity);
    initialSolution->fleetSize = counterVehicles + 1;

    // Recalculate total cost accurately (including return to depot for all routes)
    initialSolution->computeCost(instance->nodesDimension, instance->distanceMatrix);
}


void Solver::localSearch_TwoOpt(CVRP *instance, Solution *initialSolution, Solution *bestSolution)
{
    *bestSolution = *initialSolution; // Start with the initial solution
    bool improvement = true;
    int maxIterations = 1000; // Prevent infinite loops
    int iterations = 0;

    while (improvement && iterations < maxIterations)
    {
        improvement = false;
        iterations++;

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

    // Step 2: Choose a route to modify (iterate through all routes)
    for (size_t r = 0; r < perturbedSolution->routes.size(); ++r)
    {
        vector<int> route = perturbedSolution->routes[r]; // COPY to avoid iterator issues
        int routeSize = route.size();

        // Ensure that we have enough nodes for a Double Bridge Move
        if (routeSize < 8) continue;

        // Step 3: Select 4 random split points ensuring proper separation
        // Use +1 to ensure minimum increment of 1 (avoid collapsed splits)
        int maxSegment = (routeSize - 2) / 4;  // Divide available space into 4 segments
        if (maxSegment < 1) maxSegment = 1;

        int split1 = 1 + (rand() % maxSegment);
        int split2 = split1 + 1 + (rand() % maxSegment);
        int split3 = split2 + 1 + (rand() % maxSegment);
        int split4 = split3 + 1 + (rand() % maxSegment);

        // Ensure splits are within bounds
        if (split4 >= routeSize - 1) continue;

        // Step 4: Create the new order of segments using COPIED route
        vector<int> newRoute;
        newRoute.insert(newRoute.end(), route.begin(), route.begin() + split1);
        newRoute.insert(newRoute.end(), route.begin() + split3, route.begin() + split4);
        newRoute.insert(newRoute.end(), route.begin() + split2, route.begin() + split3);
        newRoute.insert(newRoute.end(), route.begin() + split1, route.begin() + split2);
        newRoute.insert(newRoute.end(), route.begin() + split4, route.end());

        // Step 5: Apply the new order to the perturbed solution
        perturbedSolution->routes[r] = newRoute;
    }

    // Step 6: Recalculate cost after the change
    perturbedSolution->computeCost(instance->nodesDimension, instance->distanceMatrix);
}

void Solver::localSearch_ThreeOpt(CVRP *instance, Solution *initialSolution, Solution *bestSolution)
{
    *bestSolution = *initialSolution; // Copy the initial solution as the starting best
    double bestCost = bestSolution->totalCost;
    bool improvement = true;
    int maxIterations = 1000; // Prevent infinite loops
    int iterations = 0;

    while (improvement && iterations < maxIterations)
    {
        improvement = false;
        iterations++;

        // Iterate through ALL routes (not just routes[0])
        for (size_t routeIdx = 0; routeIdx < bestSolution->routes.size(); ++routeIdx)
        {
            vector<int> &currentRoute = bestSolution->routes[routeIdx];

            // Skip routes that are too small for 3-opt
            if (currentRoute.size() < 5) continue; // Need at least [depot, a, b, c, depot]

            // Iterate through all possible three-cut combinations
            for (size_t i = 1; i < currentRoute.size() - 3; i++)
            {
                for (size_t j = i + 1; j < currentRoute.size() - 2; j++)
                {
                    for (size_t k = j + 1; k < currentRoute.size() - 1; k++)
                    {
                        Solution newSolution = *bestSolution; // Copy current solution
                        vector<int> &route = newSolution.routes[routeIdx];

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
                            newSolution.routes[routeIdx] = newRoute;
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
}

void Solver::localSearch_SwapStar(CVRP *instance, Solution *solution, int chain_length)
{
    bool improvement = true;
    const double epsilon = 1e-5; // Tolerance for floating point comparisons
    int maxIterations = 1000; // Prevent infinite loops
    int iterations = 0;

    // Ensure routeLoads is synchronized with routes
    if (solution->routeLoads.size() != solution->routes.size())
    {
        solution->routeLoads.resize(solution->routes.size(), 0);
        for (size_t r = 0; r < solution->routes.size(); ++r)
        {
            int load = 0;
            for (size_t i = 1; i < solution->routes[r].size() - 1; ++i)
            {
                load += instance->nodes[solution->routes[r][i]].demand;
            }
            solution->routeLoads[r] = load;
        }
    }

    while (improvement && iterations < maxIterations)
    {
        improvement = false;
        iterations++;

        for (size_t r1 = 0; r1 < solution->routes.size(); ++r1)
        {
            for (size_t r2 = 0; r2 < solution->routes.size(); ++r2)
            {
                vector<int>& sourceRoute = solution->routes[r1];
                vector<int>& destRoute = solution->routes[r2];

                // Ensure chain doesn't include depot at the end (sourceRoute.size()-1 is depot)
                for (size_t i = 1; (i + chain_length) <= sourceRoute.size() - 2; ++i)
                {
                    // 1. Check capacity feasibility first
                    int chainDemand = 0;
                    for (int k = 0; k < chain_length; ++k) {
                        chainDemand += instance->nodes[sourceRoute[i + k]].demand;
                    }

                    if (r1 != r2 && solution->routeLoads[r2] + chainDemand > instance->capacityOfVehicle) {
                        continue; // Skip if it violates capacity
                    }

                    for (size_t j = 1; j < destRoute.size(); ++j)
                    {
                        if (r1 == r2 && (j >= i && j <= i + chain_length)) {
                            continue; // Avoid redundant intra-route moves
                        }

                        // 2. Calculate cost delta
                        // Simplified delta calculation for clarity
                        // See previous implementation for detailed breakdown
                        double delta = calculate_swap_star_delta(instance, sourceRoute, destRoute, i, j, chain_length);

                        // 3. If an improvement is found, execute it immediately
                        if (delta < -epsilon)
                        {
                            // --- Execute the Move ---
                            vector<int> chain(sourceRoute.begin() + i, sourceRoute.begin() + i + chain_length);

                            if (r1 == r2) {
                                // Handle complex intra-route index changes
                                if (j < i) {
                                    sourceRoute.erase(sourceRoute.begin() + i, sourceRoute.begin() + i + chain_length);
                                    sourceRoute.insert(sourceRoute.begin() + j, chain.begin(), chain.end());
                                } else { // j > i
                                    // The insertion happens first at a higher index, so the original chain's start index doesn't shift
                                    sourceRoute.insert(sourceRoute.begin() + j, chain.begin(), chain.end());
                                    // The original chain is now shifted by chain_length positions
                                    sourceRoute.erase(sourceRoute.begin() + i, sourceRoute.begin() + i + chain_length);
                                }
                            } else {
                                // Inter-route move is simpler
                                sourceRoute.erase(sourceRoute.begin() + i, sourceRoute.begin() + i + chain_length);
                                destRoute.insert(destRoute.begin() + j, chain.begin(), chain.end());

                                // Update route loads
                                solution->routeLoads[r1] -= chainDemand;
                                solution->routeLoads[r2] += chainDemand;
                            }

                            // Update total cost and signal an improvement
                            solution->totalCost += delta;
                            improvement = true;

                            // Use goto to break out of all nested loops and restart the while loop
                            // This is a common and efficient pattern in local search implementations.
                            goto next_iteration;
                        }
                    }
                }
            }
        }
        next_iteration:; // Label for the goto jump
    }
}

double Solver::calculate_swap_star_delta(CVRP *instance, const vector<int>& route1, const vector<int>& route2, int i, int j, int k)
{
    // Validate bounds before accessing
    if (i - 1 < 0 || i + k >= (int)route1.size() || j - 1 < 0 || j >= (int)route2.size())
    {
        cerr << "Error: Invalid indices in calculate_swap_star_delta!" << endl;
        return 0.0; // Return neutral delta if invalid
    }

    // Nodes from the source route
    int A = route1[i - 1];
    int B = route1[i];
    int C = route1[i + k - 1];
    int D = route1[i + k];

    // Nodes from the destination route
    int E = route2[j - 1];
    int F = route2[j];

    double costRemoved, costAdded;

    if (&route1 == &route2) { // Check if they are the same route object
        // Intra-route delta calculation (can be complex, this is a simplified version)
        if (j < i) {
            costRemoved = instance->distanceMatrix[A * instance->nodesDimension + B] +
                          instance->distanceMatrix[C * instance->nodesDimension + D] +
                          instance->distanceMatrix[E * instance->nodesDimension + F];
            costAdded = instance->distanceMatrix[E * instance->nodesDimension + B] +
                        instance->distanceMatrix[C * instance->nodesDimension + F] +
                        instance->distanceMatrix[A * instance->nodesDimension + D];
        } else {
             costRemoved = instance->distanceMatrix[A * instance->nodesDimension + B] +
                          instance->distanceMatrix[C * instance->nodesDimension + D] +
                          instance->distanceMatrix[E * instance->nodesDimension + F];
            costAdded = instance->distanceMatrix[A * instance->nodesDimension + D] +
                        instance->distanceMatrix[E * instance->nodesDimension + B] +
                        instance->distanceMatrix[C * instance->nodesDimension + F];
        }
    } else {
        // Inter-route delta calculation
        costRemoved = instance->distanceMatrix[A * instance->nodesDimension + B] +
                      instance->distanceMatrix[C * instance->nodesDimension + D] +
                      instance->distanceMatrix[E * instance->nodesDimension + F];
        costAdded = instance->distanceMatrix[A * instance->nodesDimension + D] +
                    instance->distanceMatrix[E * instance->nodesDimension + B] +
                    instance->distanceMatrix[C * instance->nodesDimension + F];
    }

    return costAdded - costRemoved;
}


void Solver::acceptanceCriterion_BestSolution(Solution *bestSolution, Solution *newSolution)
{
    // Compare total costs
    if (newSolution->totalCost < bestSolution->totalCost)
    {
        *bestSolution = *newSolution; // Update the best solution
        //cout << "New best solution found with cost: " << bestSolution->totalCost << endl;
    }

}
