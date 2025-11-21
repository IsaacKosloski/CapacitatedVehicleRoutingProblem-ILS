/**
 * @file Solution.cpp
 * @brief Implementação da classe Solution
 * @author Isaac (Original), Melhorado em 2025
 * @date 03/02/2025 (Original), Melhorado em Outubro/2025
 */

#include "Solution.h"
#include "CVRP.h"
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <fstream>
#include <set>

// ============================================================================
// CONSTRUTORES
// ============================================================================

Solution::Solution()
    : fleetSize(0)
    , totalCost(0.0)
    , routeLoads()
    , routesCosts()
    , routes()
{
}

// ============================================================================
// CÁLCULO DE CUSTO
// ============================================================================

double Solution::computeCost(int dimensionOfNodes, std::vector<double>& matrixCost)
{
    totalCost = 0.0;
    routesCosts.clear();
    routesCosts.resize(routes.size(), 0.0);

    for (size_t r = 0; r < routes.size(); ++r)
    {
        double routeCost = computeRouteCost(r, dimensionOfNodes, matrixCost);
        routesCosts[r] = routeCost;
        totalCost += routeCost;
    }

    return totalCost;
}

double Solution::computeRouteCost(int routeIndex, int dimensionOfNodes,
                                 std::vector<double>& matrixCost) const
{
    if (routeIndex < 0 || routeIndex >= static_cast<int>(routes.size()))
    {
        std::cerr << "ERRO: Índice de rota inválido: " << routeIndex << std::endl;
        return 0.0;
    }

    const auto& route = routes[routeIndex];
    double cost = 0.0;

    for (size_t i = 0; i < route.size() - 1; ++i)
    {
        int fromNode = route[i];
        int toNode = route[i + 1];

        if (fromNode < 0 || fromNode >= dimensionOfNodes ||
            toNode < 0 || toNode >= dimensionOfNodes)
        {
            std::cerr << "ERRO: ID de nó inválido na rota: "
                     << fromNode << " -> " << toNode << std::endl;
            continue;
        }

        int matrixIndex = fromNode * dimensionOfNodes + toNode;

        if (matrixIndex >= 0 && matrixIndex < static_cast<int>(matrixCost.size()))
        {
            cost += matrixCost[matrixIndex];
        }
    }

    return cost;
}

// ============================================================================
// VALIDAÇÃO
// ============================================================================

bool Solution::validateSolution(CVRP* instance) const
{
    if (!instance)
    {
        std::cerr << "ERRO: Instância nula na validação!" << std::endl;
        return false;
    }

    if (!isInternallyConsistent())
    {
        std::cerr << "ERRO: Solução não é internamente consistente!" << std::endl;
        return false;
    }

    // Verifica que todos os clientes são visitados exatamente uma vez
    std::vector<int> visitCount(instance->nodesDimension, 0);

    for (const auto& route : routes)
    {
        // Verifica se rota começa e termina no depósito
        if (route.size() < 2)
        {
            std::cerr << "ERRO: Rota com menos de 2 nós!" << std::endl;
            return false;
        }

        if (route.front() != instance->depotID || route.back() != instance->depotID)
        {
            std::cerr << "ERRO: Rota não começa/termina no depósito!" << std::endl;
            std::cerr << "  Início: " << route.front() << ", Fim: " << route.back()
                     << ", Depósito: " << instance->depotID << std::endl;
            return false;
        }

        // Conta visitas
        for (size_t i = 1; i < route.size() - 1; ++i)
        {
            int nodeID = route[i];
            if (nodeID >= 0 && nodeID < instance->nodesDimension)
            {
                visitCount[nodeID]++;
            }
        }
    }

    // Verifica que cada cliente é visitado exatamente uma vez
    for (int i = 0; i < instance->nodesDimension; ++i)
    {
        if (instance->nodes[i].isDepot) continue;

        if (visitCount[i] == 0)
        {
            std::cerr << "ERRO: Cliente " << i << " não foi visitado!" << std::endl;
            return false;
        }

        if (visitCount[i] > 1)
        {
            std::cerr << "ERRO: Cliente " << i << " visitado "
                     << visitCount[i] << " vezes!" << std::endl;
            return false;
        }
    }

    // Verifica capacidade de cada rota
    for (size_t r = 0; r < routes.size(); ++r)
    {
        int load = 0;
        for (size_t i = 1; i < routes[r].size() - 1; ++i)
        {
            int nodeID = routes[r][i];
            if (nodeID >= 0 && nodeID < instance->nodesDimension)
            {
                load += instance->nodes[nodeID].demand;
            }
        }

        if (load > instance->capacityOfVehicle)
        {
            std::cerr << "ERRO: Rota " << r << " excede capacidade! "
                     << "Carga: " << load << ", Capacidade: "
                     << instance->capacityOfVehicle << std::endl;
            return false;
        }
    }

    return true;
}

bool Solution::isInternallyConsistent() const
{
    if (routes.size() != routeLoads.size())
    {
        std::cerr << "ERRO: Inconsistência no tamanho de routes vs routeLoads" << std::endl;
        return false;
    }

    if (routes.size() != routesCosts.size())
    {
        std::cerr << "ERRO: Inconsistência no tamanho de routes vs routesCosts" << std::endl;
        return false;
    }

    if (static_cast<int>(routes.size()) != fleetSize)
    {
        std::cerr << "ERRO: fleetSize não corresponde ao número de rotas" << std::endl;
        return false;
    }

    if (totalCost < 0)
    {
        std::cerr << "ERRO: Custo total negativo!" << std::endl;
        return false;
    }

    return true;
}

// ============================================================================
// SAÍDA E FORMATAÇÃO
// ============================================================================

bool Solution::printSolution(const char* fileName, double elapsedTime,
                            int iterations, int dimensionOfNodes) const
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "ERRO: Não foi possível abrir arquivo para escrita: "
                  << fileName << std::endl;
        return false;
    }

    // Escreve rotas (formato compatível com o arquivo .sol de exemplo)
    for (size_t i = 0; i < routes.size(); ++i)
    {
        file << "Route #" << (i + 1) << ":";

        // Escreve nós da rota (exceto depósitos inicial e final)
        for (size_t j = 1; j < routes[i].size() - 1; ++j)
        {
            file << " " << routes[i][j];
        }

        file << std::endl;
    }

    // Escreve custo total
    file << "Cost " << static_cast<int>(std::round(totalCost)) << std::endl;

    file.close();
    return true;
}

void Solution::printToConsole() const
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "SOLUÇÃO" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "Custo total: " << totalCost << std::endl;
    std::cout << "Número de rotas: " << fleetSize << std::endl;
    std::cout << "Número de clientes: " << getTotalCustomers() << std::endl;

    for (size_t i = 0; i < routes.size(); ++i)
    {
        std::cout << "\nRota #" << (i + 1) << ": ";
        for (size_t j = 0; j < routes[i].size(); ++j)
        {
            std::cout << routes[i][j];
            if (j < routes[i].size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
        std::cout << "  Custo: " << routesCosts[i]
                  << ", Carga: " << routeLoads[i] << std::endl;
    }

    std::cout << "========================================\n" << std::endl;
}

std::string Solution::toString() const
{
    std::ostringstream oss;
    oss << "Solution[cost=" << std::fixed << std::setprecision(2) << totalCost
        << ", routes=" << fleetSize
        << ", customers=" << getTotalCustomers()
        << "]";
    return oss.str();
}

std::string Solution::routeToString(int routeIndex) const
{
    if (routeIndex < 0 || routeIndex >= static_cast<int>(routes.size()))
    {
        return "[invalid route index]";
    }

    std::ostringstream oss;
    const auto& route = routes[routeIndex];

    oss << "[";
    for (size_t i = 0; i < route.size(); ++i)
    {
        oss << route[i];
        if (i < route.size() - 1) oss << " -> ";
    }
    oss << "]";

    return oss.str();
}

// ============================================================================
// MANIPULAÇÃO DE ROTAS
// ============================================================================

void Solution::addEmptyRoute(int depotID)
{
    routes.push_back({depotID, depotID});
    routeLoads.push_back(0);
    routesCosts.push_back(0.0);
    fleetSize++;
}

bool Solution::removeRoute(int routeIndex)
{
    if (routeIndex < 0 || routeIndex >= static_cast<int>(routes.size()))
    {
        return false;
    }

    routes.erase(routes.begin() + routeIndex);
    routeLoads.erase(routeLoads.begin() + routeIndex);
    routesCosts.erase(routesCosts.begin() + routeIndex);
    fleetSize--;

    return true;
}

int Solution::removeEmptyRoutes()
{
    int removed = 0;

    for (int i = static_cast<int>(routes.size()) - 1; i >= 0; --i)
    {
        // Rota vazia tem apenas 2 nós (depósito inicial e final)
        if (routes[i].size() <= 2)
        {
            removeRoute(i);
            removed++;
        }
    }

    return removed;
}

void Solution::clear()
{
    routes.clear();
    routeLoads.clear();
    routesCosts.clear();
    fleetSize = 0;
    totalCost = 0.0;
}

// ============================================================================
// ESTATÍSTICAS
// ============================================================================

Solution::Statistics Solution::computeStatistics() const
{
    Statistics stats = {};

    if (routes.empty())
    {
        return stats;
    }

    // Custos
    if (!routesCosts.empty())
    {
        stats.minRouteCost = *std::min_element(routesCosts.begin(), routesCosts.end());
        stats.maxRouteCost = *std::max_element(routesCosts.begin(), routesCosts.end());
        stats.avgRouteCost = std::accumulate(routesCosts.begin(), routesCosts.end(), 0.0) / routesCosts.size();

        // Desvio padrão
        stats.stdDevRouteCost = calculateStdDev();
    }

    // Cargas
    if (!routeLoads.empty())
    {
        stats.minRouteLoad = *std::min_element(routeLoads.begin(), routeLoads.end());
        stats.maxRouteLoad = *std::max_element(routeLoads.begin(), routeLoads.end());
        stats.avgRouteLoad = std::accumulate(routeLoads.begin(), routeLoads.end(), 0.0) / routeLoads.size();
    }

    stats.totalCustomers = getTotalCustomers();
    stats.totalDistance = static_cast<int>(std::round(totalCost));

    return stats;
}

int Solution::getTotalCustomers() const
{
    int total = 0;

    for (const auto& route : routes)
    {
        // Conta nós excluindo depósitos inicial e final
        if (route.size() > 2)
        {
            total += route.size() - 2;
        }
    }

    return total;
}

bool Solution::isEmpty() const
{
    return routes.empty();
}

int Solution::getMaxLoad() const
{
    if (routeLoads.empty())
    {
        return 0;
    }

    return *std::max_element(routeLoads.begin(), routeLoads.end());
}

int Solution::getMinLoad() const
{
    if (routeLoads.empty())
    {
        return 0;
    }

    return *std::min_element(routeLoads.begin(), routeLoads.end());
}

// ============================================================================
// COMPARAÇÃO
// ============================================================================

bool Solution::isBetterThan(const Solution& other) const
{
    return totalCost < other.totalCost - EPSILON;
}

double Solution::gapTo(const Solution& other) const
{
    if (std::abs(other.totalCost) < EPSILON)
    {
        return 0.0;
    }

    return ((this->totalCost - other.totalCost) / other.totalCost) * 100.0;
}

bool Solution::operator<(const Solution& other) const
{
    return totalCost < other.totalCost;
}

bool Solution::operator>(const Solution& other) const
{
    return totalCost > other.totalCost;
}

bool Solution::operator==(const Solution& other) const
{
    return std::abs(totalCost - other.totalCost) < EPSILON;
}

// ============================================================================
// CLONAGEM
// ============================================================================

std::unique_ptr<Solution> Solution::clone() const
{
    auto newSolution = std::make_unique<Solution>();

    newSolution->fleetSize = this->fleetSize;
    newSolution->totalCost = this->totalCost;
    newSolution->routeLoads = this->routeLoads;
    newSolution->routesCosts = this->routesCosts;
    newSolution->routes = this->routes;

    return newSolution;
}

// ============================================================================
// MÉTODOS AUXILIARES PRIVADOS
// ============================================================================

double Solution::calculateStdDev() const
{
    if (routesCosts.size() < 2)
    {
        return 0.0;
    }

    double mean = std::accumulate(routesCosts.begin(), routesCosts.end(), 0.0) / routesCosts.size();

    double sumSquares = 0.0;
    for (double cost : routesCosts)
    {
        double diff = cost - mean;
        sumSquares += diff * diff;
    }

    return std::sqrt(sumSquares / (routesCosts.size() - 1));
}

std::string Solution::formatTime(double seconds)
{
    std::ostringstream oss;

    if (seconds < 60.0)
    {
        oss << std::fixed << std::setprecision(2) << seconds << "s";
    }
    else if (seconds < 3600.0)
    {
        int minutes = static_cast<int>(seconds / 60);
        double secs = seconds - (minutes * 60);
        oss << minutes << "m " << std::fixed << std::setprecision(1) << secs << "s";
    }
    else
    {
        int hours = static_cast<int>(seconds / 3600);
        int minutes = static_cast<int>((seconds - hours * 3600) / 60);
        double secs = seconds - (hours * 3600) - (minutes * 60);
        oss << hours << "h " << minutes << "m " << std::fixed << std::setprecision(0) << secs << "s";
    }

    return oss.str();
}

// ============================================================================
// OPERADOR DE STREAM
// ============================================================================

std::ostream& operator<<(std::ostream& os, const Solution& solution)
{
    os << solution.toString();
    return os;
}
