/**
 * @file Solver.cpp
 * @brief Implementação completa da classe Solver
 * @author Isaac (Original), Melhorado em 2025
 * @date 03/02/2025 (Original), Melhorado em Outubro/2025
 */

#include "Solver.h"
#include <algorithm>
#include <limits>
#include <iostream>
#include <cmath>
#include <numeric>

// ============================================================================
// CONSTRUTOR E MÉTODOS DE SEED
// ============================================================================

Solver::Solver(unsigned int seed)
    : rng(seed)
    , dist(0.0, 1.0)
    , currentSeed(seed)
{
    // Inicialização do gerador Mersenne Twister com a seed fornecida
}

unsigned int Solver::getCurrentSeed() const
{
    return currentSeed;
}

void Solver::setSeed(unsigned int seed)
{
    currentSeed = seed;
    rng.seed(seed);
}

// ============================================================================
// MÉTODOS AUXILIARES PRIVADOS
// ============================================================================

bool Solver::validateRouteCapacity(const std::vector<int>& route, CVRP* instance) const
{
    int load = 0;

    // Ignora primeiro e último (depósitos)
    for (size_t i = 1; i < route.size() - 1; ++i)
    {
        int nodeID = route[i];
        if (nodeID >= 0 && nodeID < instance->nodesDimension)
        {
            load += instance->nodes[nodeID].demand;
        }
    }

    return load <= instance->capacityOfVehicle;
}

int Solver::calculateRouteLoad(const std::vector<int>& route, CVRP* instance) const
{
    int load = 0;

    for (size_t i = 1; i < route.size() - 1; ++i)
    {
        int nodeID = route[i];
        if (nodeID >= 0 && nodeID < instance->nodesDimension)
        {
            load += instance->nodes[nodeID].demand;
        }
    }

    return load;
}

// ============================================================================
// SOLUÇÃO INICIAL - GREEDY
// ============================================================================

void Solver::initialSolution_Greedy(CVRP* instance, Solution* solution)
{
    int currentCapacity = 0;
    int vehicleCount = 0;
    double minCost;
    int nearestCustomer = -1;

    std::vector<bool> visited(instance->nodesDimension, false);
    visited[instance->depotID] = true;

    solution->routes.clear();
    solution->routeLoads.clear();
    solution->routes.push_back(std::vector<int>());
    solution->routes[vehicleCount].push_back(instance->depotID);
    solution->totalCost = 0.0;

    int customersServed = 0;
    const int totalCustomers = instance->nodesDimension - 1;

    while (customersServed < totalCustomers)
    {
        minCost = std::numeric_limits<double>::max();
        nearestCustomer = -1;

        int lastNode = solution->routes[vehicleCount].back();

        // Encontra cliente mais próximo não visitado que respeite capacidade
        for (int i = 0; i < instance->nodesDimension; ++i)
        {
            if (visited[i] || instance->nodes[i].isDepot) continue;

            int demand = instance->nodes[i].demand;
            double distance = instance->getDistance(lastNode, i);

            if (currentCapacity + demand <= instance->capacityOfVehicle &&
                distance < minCost)
            {
                nearestCustomer = i;
                minCost = distance;
            }
        }

        // Se nenhum cliente encontrado, inicia nova rota
        if (nearestCustomer == -1)
        {
            solution->routes[vehicleCount].push_back(instance->depotID);
            solution->routeLoads.push_back(currentCapacity);

            vehicleCount++;
            solution->routes.push_back(std::vector<int>());
            solution->routes[vehicleCount].push_back(instance->depotID);
            currentCapacity = 0;
            continue;
        }

        // Adiciona cliente à rota
        solution->routes[vehicleCount].push_back(nearestCustomer);
        currentCapacity += instance->nodes[nearestCustomer].demand;
        visited[nearestCustomer] = true;
        customersServed++;
    }

    // Fecha última rota
    solution->routes[vehicleCount].push_back(instance->depotID);
    solution->routeLoads.push_back(currentCapacity);
    solution->fleetSize = vehicleCount + 1;

    // Calcula custo total
    solution->computeCost(instance->nodesDimension, instance->distanceMatrix);
}

// ============================================================================
// SOLUÇÃO INICIAL - RANDOM
// ============================================================================

void Solver::initialSolution_Random(CVRP* instance, Solution* solution)
{
    // Cria lista de clientes
    std::vector<int> customers;
    for (int i = 0; i < instance->nodesDimension; ++i)
    {
        if (!instance->nodes[i].isDepot)
            customers.push_back(i);
    }

    // Embaralha
    std::shuffle(customers.begin(), customers.end(), rng);

    // Constrói rotas
    solution->routes.clear();
    solution->routeLoads.clear();
    solution->routes.push_back(std::vector<int>());
    solution->routes[0].push_back(instance->depotID);

    int currentCapacity = 0;
    int vehicleCount = 0;

    for (int customer : customers)
    {
        int demand = instance->nodes[customer].demand;

        if (currentCapacity + demand > instance->capacityOfVehicle)
        {
            // Fecha rota atual
            solution->routes[vehicleCount].push_back(instance->depotID);
            solution->routeLoads.push_back(currentCapacity);

            // Nova rota
            vehicleCount++;
            solution->routes.push_back(std::vector<int>());
            solution->routes[vehicleCount].push_back(instance->depotID);
            currentCapacity = 0;
        }

        solution->routes[vehicleCount].push_back(customer);
        currentCapacity += demand;
    }

    // Fecha última rota
    solution->routes[vehicleCount].push_back(instance->depotID);
    solution->routeLoads.push_back(currentCapacity);
    solution->fleetSize = vehicleCount + 1;

    solution->computeCost(instance->nodesDimension, instance->distanceMatrix);
}

// ============================================================================
// BUSCA LOCAL - 2-OPT
// ============================================================================

void Solver::localSearch_TwoOpt(CVRP* instance, Solution* solution, Solution* bestSolution)
{
    *bestSolution = *solution;
    bool improvement = true;

    while (improvement)
    {
        improvement = false;

        for (auto& route : bestSolution->routes)
        {
            int routeSize = route.size();
            if (routeSize < 4) continue;

            for (int i = 1; i < routeSize - 2; ++i)
            {
                for (int j = i + 1; j < routeSize - 1; ++j)
                {
                    int A = route[i - 1], B = route[i];
                    int C = route[j], D = route[j + 1];

                    double currentCost =
                        instance->getDistance(A, B) +
                        instance->getDistance(C, D);

                    double newCost =
                        instance->getDistance(A, C) +
                        instance->getDistance(B, D);

                    if (newCost < currentCost - EPSILON)
                    {
                        std::reverse(route.begin() + i, route.begin() + j + 1);
                        bestSolution->computeCost(instance->nodesDimension,
                                                 instance->distanceMatrix);
                        improvement = true;
                    }
                }
            }
        }
    }
}

// ============================================================================
// BUSCA LOCAL - 3-OPT
// ============================================================================

void Solver::localSearch_ThreeOpt(CVRP* instance, Solution* solution, Solution* bestSolution)
{
    *bestSolution = *solution;
    bool improvement = true;

    while (improvement)
    {
        improvement = false;

        for (auto& route : bestSolution->routes)
        {
            int routeSize = route.size();
            if (routeSize < 6) continue;

            for (int i = 1; i < routeSize - 4; ++i)
            {
                for (int j = i + 2; j < routeSize - 2; ++j)
                {
                    for (int k = j + 2; k < routeSize; ++k)
                    {
                        std::vector<int> originalRoute = route;
                        double originalCost = 0.0;

                        // Calcula custo original
                        for (size_t idx = 0; idx < originalRoute.size() - 1; ++idx)
                        {
                            originalCost += instance->getDistance(
                                originalRoute[idx],
                                originalRoute[idx + 1]
                            );
                        }

                        // Testa configurações 3-opt
                        std::vector<std::vector<int>> configs(5, originalRoute);

                        // Config 1: Reverte [i, j)
                        std::reverse(configs[1].begin() + i, configs[1].begin() + j);

                        // Config 2: Reverte [j, k)
                        std::reverse(configs[2].begin() + j, configs[2].begin() + k);

                        // Config 3: Reverte ambos
                        std::reverse(configs[3].begin() + i, configs[3].begin() + j);
                        std::reverse(configs[3].begin() + j, configs[3].begin() + k);

                        // Config 4: Troca segmentos
                        std::vector<int> seg1(configs[4].begin() + i, configs[4].begin() + j);
                        std::vector<int> seg2(configs[4].begin() + j, configs[4].begin() + k);
                        configs[4].erase(configs[4].begin() + i, configs[4].begin() + k);
                        configs[4].insert(configs[4].begin() + i, seg2.begin(), seg2.end());
                        configs[4].insert(configs[4].begin() + i + seg2.size(),
                                        seg1.begin(), seg1.end());

                        // Avalia configurações
                        for (size_t c = 1; c < configs.size(); ++c)
                        {
                            double newCost = 0.0;
                            for (size_t idx = 0; idx < configs[c].size() - 1; ++idx)
                            {
                                newCost += instance->getDistance(
                                    configs[c][idx],
                                    configs[c][idx + 1]
                                );
                            }

                            if (newCost < originalCost - EPSILON)
                            {
                                route = configs[c];
                                bestSolution->computeCost(instance->nodesDimension,
                                                        instance->distanceMatrix);
                                improvement = true;
                                goto next_iteration_3opt;
                            }
                        }
                    }
                }
            }
        }
        next_iteration_3opt:;
    }
}

// ============================================================================
// BUSCA LOCAL - OR-OPT
// ============================================================================

void Solver::localSearch_OrOpt(CVRP* instance, Solution* solution)
{
    bool improvement = true;
    const int maxSequenceSize = 3;

    while (improvement)
    {
        improvement = false;

        for (size_t r = 0; r < solution->routes.size(); ++r)
        {
            auto& route = solution->routes[r];
            int routeSize = route.size();

            for (int seqSize = 1; seqSize <= maxSequenceSize; ++seqSize)
            {
                for (int i = 1; i + seqSize < routeSize - 1; ++i)
                {
                    for (int j = 1; j < routeSize - 1; ++j)
                    {
                        if (j >= i && j < i + seqSize) continue;

                        // Calcula delta
                        int before_i = route[i - 1];
                        int first_seq = route[i];
                        int last_seq = route[i + seqSize - 1];
                        int after_seq = route[i + seqSize];
                        int before_j = route[j - 1];
                        int at_j = route[j];

                        double costRemoved =
                            instance->getDistance(before_i, first_seq) +
                            instance->getDistance(last_seq, after_seq) +
                            instance->getDistance(before_j, at_j);

                        double costAdded =
                            instance->getDistance(before_i, after_seq) +
                            instance->getDistance(before_j, first_seq) +
                            instance->getDistance(last_seq, at_j);

                        if (costAdded < costRemoved - EPSILON)
                        {
                            // Aplica movimento
                            std::vector<int> sequence(route.begin() + i,
                                                     route.begin() + i + seqSize);
                            route.erase(route.begin() + i,
                                      route.begin() + i + seqSize);

                            int insertPos = (j > i) ? j - seqSize : j;
                            route.insert(route.begin() + insertPos,
                                       sequence.begin(), sequence.end());

                            solution->computeCost(instance->nodesDimension,
                                                instance->distanceMatrix);
                            improvement = true;
                            goto next_iteration_oropt;
                        }
                    }
                }
            }
        }
        next_iteration_oropt:;
    }
}

// ============================================================================
// BUSCA LOCAL - SWAP STAR
// ============================================================================

double Solver::calculateSwapStarDelta(CVRP* instance,
                                     const std::vector<int>& route1,
                                     const std::vector<int>& route2,
                                     int i, int j, int k) const
{
    // Validação de índices
    if (i < 1 || i + k > static_cast<int>(route1.size()) - 1 ||
        j < 1 || j >= static_cast<int>(route2.size()))
    {
        return std::numeric_limits<double>::max();
    }

    int A = route1[i - 1];
    int B = route1[i];
    int C = route1[i + k - 1];
    int D = (i + k < static_cast<int>(route1.size())) ? route1[i + k] : route1[0];

    int E = route2[j - 1];
    int F = route2[j];

    double costRemoved, costAdded;

    if (&route1 == &route2)
    {
        // Intra-rota
        if (j < i)
        {
            costRemoved = instance->getDistance(A, B) +
                         instance->getDistance(C, D) +
                         instance->getDistance(E, F);

            costAdded = instance->getDistance(E, B) +
                       instance->getDistance(C, F) +
                       instance->getDistance(A, D);
        }
        else if (j > i + k)
        {
            costRemoved = instance->getDistance(A, B) +
                         instance->getDistance(C, D) +
                         instance->getDistance(E, F);

            costAdded = instance->getDistance(A, D) +
                       instance->getDistance(E, B) +
                       instance->getDistance(C, F);
        }
        else
        {
            return 0.0;
        }
    }
    else
    {
        // Inter-rota
        costRemoved = instance->getDistance(A, B) +
                     instance->getDistance(C, D) +
                     instance->getDistance(E, F);

        costAdded = instance->getDistance(A, D) +
                   instance->getDistance(E, B) +
                   instance->getDistance(C, F);
    }

    return costAdded - costRemoved;
}

void Solver::localSearch_SwapStar(CVRP* instance, Solution* solution, int chainLength)
{
    bool improvement = true;

    while (improvement)
    {
        improvement = false;

        for (size_t r1 = 0; r1 < solution->routes.size(); ++r1)
        {
            for (size_t r2 = 0; r2 < solution->routes.size(); ++r2)
            {
                std::vector<int>& sourceRoute = solution->routes[r1];
                std::vector<int>& destRoute = solution->routes[r2];

                for (int i = 1; i + chainLength <= static_cast<int>(sourceRoute.size()) - 1; ++i)
                {
                    // Calcula demanda da cadeia
                    int chainDemand = 0;
                    for (int k = 0; k < chainLength; ++k)
                    {
                        chainDemand += instance->nodes[sourceRoute[i + k]].demand;
                    }

                    // Verifica capacidade para inter-rota
                    if (r1 != r2)
                    {
                        if (solution->routeLoads[r2] + chainDemand >
                            instance->capacityOfVehicle)
                        {
                            continue;
                        }
                    }

                    for (int j = 1; j < static_cast<int>(destRoute.size()); ++j)
                    {
                        if (r1 == r2 && j >= i && j <= i + chainLength)
                        {
                            continue;
                        }

                        double delta = calculateSwapStarDelta(instance, sourceRoute,
                                                             destRoute, i, j, chainLength);

                        if (delta < -EPSILON)
                        {
                            // Extrai cadeia
                            std::vector<int> chain(sourceRoute.begin() + i,
                                                  sourceRoute.begin() + i + chainLength);

                            if (r1 == r2)
                            {
                                // Intra-rota
                                if (j < i)
                                {
                                    sourceRoute.erase(sourceRoute.begin() + i,
                                                    sourceRoute.begin() + i + chainLength);
                                    sourceRoute.insert(sourceRoute.begin() + j,
                                                     chain.begin(), chain.end());
                                }
                                else
                                {
                                    sourceRoute.insert(sourceRoute.begin() + j,
                                                     chain.begin(), chain.end());
                                    sourceRoute.erase(sourceRoute.begin() + i,
                                                    sourceRoute.begin() + i + chainLength);
                                }
                            }
                            else
                            {
                                // Inter-rota
                                sourceRoute.erase(sourceRoute.begin() + i,
                                                sourceRoute.begin() + i + chainLength);
                                destRoute.insert(destRoute.begin() + j,
                                               chain.begin(), chain.end());

                                solution->routeLoads[r1] -= chainDemand;
                                solution->routeLoads[r2] += chainDemand;
                            }

                            solution->totalCost += delta;
                            improvement = true;
                            goto next_iteration_swap;
                        }
                    }
                }
            }
        }
        next_iteration_swap:;
    }
}

// ============================================================================
// PERTURBAÇÃO - DOUBLE BRIDGE
// ============================================================================

void Solver::perturbation_DoubleBridge(CVRP* instance,
                                      Solution* bestSolution,
                                      Solution* perturbedSolution)
{
    *perturbedSolution = *bestSolution;

    // Aplica Double Bridge em todas as rotas adequadas
    for (auto& route : perturbedSolution->routes)
    {
        int routeSize = route.size();

        if (routeSize < 8) continue;

        // Gera 4 pontos de corte
        std::uniform_int_distribution<int> splitDist(1, routeSize - 2);
        std::vector<int> splits(4);

        for (int& split : splits)
            split = splitDist(rng);

        std::sort(splits.begin(), splits.end());

        // Garante separação
        for (size_t i = 1; i < splits.size(); ++i)
        {
            if (splits[i] - splits[i-1] < 2)
                splits[i] = splits[i-1] + 2;
        }

        // Reconstrói rota
        std::vector<int> newRoute;
        newRoute.insert(newRoute.end(), route.begin(), route.begin() + splits[0]);
        newRoute.insert(newRoute.end(), route.begin() + splits[2], route.begin() + splits[3]);
        newRoute.insert(newRoute.end(), route.begin() + splits[1], route.begin() + splits[2]);
        newRoute.insert(newRoute.end(), route.begin() + splits[0], route.begin() + splits[1]);
        newRoute.insert(newRoute.end(), route.begin() + splits[3], route.end());

        route = newRoute;
    }

    perturbedSolution->computeCost(instance->nodesDimension, instance->distanceMatrix);
}

// ============================================================================
// PERTURBAÇÃO - RANDOM REMOVE INSERT
// ============================================================================

void Solver::perturbation_RandomRemoveInsert(CVRP* instance,
                                            Solution* bestSolution,
                                            Solution* perturbedSolution,
                                            int numNodes)
{
    *perturbedSolution = *bestSolution;

    if (perturbedSolution->routes.empty()) return;

    std::vector<int> removedNodes;

    // Remove nós
    for (int n = 0; n < numNodes; ++n)
    {
        std::uniform_int_distribution<size_t> routeDist(0, perturbedSolution->routes.size() - 1);
        size_t routeIdx = routeDist(rng);

        auto& route = perturbedSolution->routes[routeIdx];
        if (route.size() <= 2) continue;

        std::uniform_int_distribution<size_t> nodeDist(1, route.size() - 2);
        size_t nodeIdx = nodeDist(rng);

        removedNodes.push_back(route[nodeIdx]);
        route.erase(route.begin() + nodeIdx);

        perturbedSolution->routeLoads[routeIdx] -= instance->nodes[removedNodes.back()].demand;
    }

    // Reinsere
    for (int node : removedNodes)
    {
        int demand = instance->nodes[node].demand;
        bool inserted = false;

        std::vector<size_t> routeOrder(perturbedSolution->routes.size());
        std::iota(routeOrder.begin(), routeOrder.end(), 0);
        std::shuffle(routeOrder.begin(), routeOrder.end(), rng);

        for (size_t r : routeOrder)
        {
            if (perturbedSolution->routeLoads[r] + demand <= instance->capacityOfVehicle)
            {
                auto& route = perturbedSolution->routes[r];
                std::uniform_int_distribution<size_t> posDist(1, route.size() - 1);
                size_t insertPos = posDist(rng);

                route.insert(route.begin() + insertPos, node);
                perturbedSolution->routeLoads[r] += demand;
                inserted = true;
                break;
            }
        }

        if (!inserted)
        {
            std::vector<int> newRoute = {instance->depotID, node, instance->depotID};
            perturbedSolution->routes.push_back(newRoute);
            perturbedSolution->routeLoads.push_back(demand);
            perturbedSolution->fleetSize++;
        }
    }

    perturbedSolution->computeCost(instance->nodesDimension, instance->distanceMatrix);
}

// ============================================================================
// CRITÉRIOS DE ACEITAÇÃO
// ============================================================================

bool Solver::acceptanceCriterion_BestImprovement(Solution* bestSolution,
                                                 Solution* newSolution)
{
    if (newSolution->totalCost < bestSolution->totalCost - EPSILON)
    {
        *bestSolution = *newSolution;
        return true;
    }
    return false;
}

bool Solver::acceptanceCriterion_SimulatedAnnealing(Solution* bestSolution,
                                                    Solution* newSolution,
                                                    double temperature)
{
    double delta = newSolution->totalCost - bestSolution->totalCost;

    if (delta < -EPSILON)
    {
        *bestSolution = *newSolution;
        return true;
    }

    if (temperature > EPSILON)
    {
        double probability = std::exp(-delta / temperature);
        if (dist(rng) < probability)
        {
            *bestSolution = *newSolution;
            return true;
        }
    }

    return false;
}