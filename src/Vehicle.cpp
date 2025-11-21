/**
 * @file Vehicle.cpp
 * @brief Implementação da classe Vehicle
 * @author Isaac (Original), Melhorado em 2025
 */

#include "Vehicle.h"
#include "Component.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

// ============================================================================
// CONSTRUTORES
// ============================================================================

Vehicle::Vehicle()
    : capacity(0)
    , currentLoad(0)
    , route()
{
    // Construtor padrão
}

Vehicle::Vehicle(int capacity, int currentLoad)
    : capacity(capacity)
    , currentLoad(currentLoad)
    , route()
{
    // Validação
    if (capacity < 0)
    {
        std::cerr << "AVISO: Capacidade negativa (" << capacity
                  << "). Usando 0." << std::endl;
        this->capacity = 0;
    }

    if (currentLoad < 0)
    {
        std::cerr << "AVISO: Carga inicial negativa (" << currentLoad
                  << "). Usando 0." << std::endl;
        this->currentLoad = 0;
    }

    if (currentLoad > capacity)
    {
        std::cerr << "AVISO: Carga inicial (" << currentLoad
                  << ") maior que capacidade (" << capacity
                  << "). Ajustando para capacidade." << std::endl;
        this->currentLoad = capacity;
    }
}

// ============================================================================
// MÉTODOS DE MANIPULAÇÃO DE ROTA
// ============================================================================

bool Vehicle::addNode(const Node& node)
{
    // Verifica se há capacidade disponível
    if (currentLoad + node.demand > capacity)
    {
        return false;
    }

    // Adiciona o nó à rota
    route.push_back(node);
    currentLoad += node.demand;

    return true;
}

void Vehicle::forceAddNode(const Node& node)
{
    route.push_back(node);
    currentLoad += node.demand;

    if (currentLoad > capacity)
    {
        std::cerr << "AVISO: Veículo sobre-carregado! Carga: " << currentLoad
                  << ", Capacidade: " << capacity << std::endl;
    }
}

bool Vehicle::removeLastNode()
{
    if (route.empty())
    {
        return false;
    }

    // Não remove se for o único nó (depósito inicial)
    if (route.size() == 1)
    {
        return false;
    }

    // Remove e atualiza carga
    Node removedNode = route.back();
    route.pop_back();
    currentLoad -= removedNode.demand;

    // Garante que carga não fique negativa
    if (currentLoad < 0)
    {
        currentLoad = 0;
    }

    return true;
}

bool Vehicle::removeNodeAt(int index)
{
    if (index < 0 || index >= static_cast<int>(route.size()))
    {
        std::cerr << "ERRO: Índice inválido para remoção: " << index << std::endl;
        return false;
    }

    // Não permite remover depósito inicial ou final
    if (index == 0 || index == static_cast<int>(route.size()) - 1)
    {
        if (route[index].isDepot)
        {
            std::cerr << "ERRO: Não é permitido remover depósito da rota" << std::endl;
            return false;
        }
    }

    Node removedNode = route[index];
    route.erase(route.begin() + index);
    currentLoad -= removedNode.demand;

    if (currentLoad < 0)
    {
        currentLoad = 0;
    }

    return true;
}

bool Vehicle::insertNodeAt(const Node& node, int position)
{
    if (position < 0 || position > static_cast<int>(route.size()))
    {
        std::cerr << "ERRO: Posição inválida para inserção: " << position << std::endl;
        return false;
    }

    // Verifica capacidade
    if (currentLoad + node.demand > capacity)
    {
        return false;
    }

    route.insert(route.begin() + position, node);
    currentLoad += node.demand;

    return true;
}

void Vehicle::reset()
{
    route.clear();
    currentLoad = 0;
}

void Vehicle::resetLoad()
{
    currentLoad = 0;
}

// ============================================================================
// MÉTODOS DE CONSULTA
// ============================================================================

bool Vehicle::canAccommodate(int additionalDemand) const
{
    return (currentLoad + additionalDemand <= capacity);
}

int Vehicle::getAvailableCapacity() const
{
    return capacity - currentLoad;
}

bool Vehicle::isEmpty() const
{
    return route.empty();
}

int Vehicle::getRouteSize() const
{
    return static_cast<int>(route.size());
}

int Vehicle::getNumberOfCustomers() const
{
    int count = 0;
    for (const auto& node : route)
    {
        if (!node.isDepot)
        {
            count++;
        }
    }
    return count;
}

bool Vehicle::isRouteValid() const
{
    // Verifica capacidade
    if (currentLoad > capacity)
    {
        return false;
    }

    // Se rota não está vazia, deve começar no depósito
    if (!route.empty() && !route.front().isDepot)
    {
        return false;
    }

    // Recalcula carga para verificar consistência
    int calculatedLoad = 0;
    for (const auto& node : route)
    {
        if (!node.isDepot)
        {
            calculatedLoad += node.demand;
        }
    }

    if (calculatedLoad != currentLoad)
    {
        std::cerr << "AVISO: Inconsistência na carga. Calculada: "
                  << calculatedLoad << ", Armazenada: " << currentLoad << std::endl;
    }

    if (calculatedLoad > capacity)
    {
        return false;
    }

    return true;
}

bool Vehicle::isRouteClosed() const
{
    if (route.size() < 2)
    {
        return false;
    }

    return (route.front().isDepot && route.back().isDepot);
}

// ============================================================================
// CÁLCULO DE CUSTO
// ============================================================================

double Vehicle::computeCost(const std::vector<double>& distanceMatrix,
                           int dimension) const
{
    if (route.empty())
    {
        return 0.0;
    }

    double cost = 0.0;

    // Calcula custo percorrendo a rota
    for (size_t i = 0; i < route.size() - 1; ++i)
    {
        int fromNodeID = route[i].ID;
        int toNodeID = route[i + 1].ID;

        // Validação de índices
        if (fromNodeID < 0 || fromNodeID >= dimension ||
            toNodeID < 0 || toNodeID >= dimension)
        {
            std::cerr << "ERRO: ID de nó inválido na rota: "
                      << fromNodeID << " -> " << toNodeID << std::endl;
            continue;
        }

        int matrixIndex = fromNodeID * dimension + toNodeID;

        if (matrixIndex < 0 || matrixIndex >= static_cast<int>(distanceMatrix.size()))
        {
            std::cerr << "ERRO: Índice de matriz fora do intervalo: "
                      << matrixIndex << std::endl;
            continue;
        }

        cost += distanceMatrix[matrixIndex];
    }

    return cost;
}

double Vehicle::computeCostFromComponents(const std::vector<Component>& components) const
{
    if (route.empty())
    {
        return 0.0;
    }

    double cost = 0.0;

    for (size_t i = 0; i < route.size() - 1; ++i)
    {
        int fromNodeID = route[i].ID;
        int toNodeID = route[i + 1].ID;

        // Validação
        if (fromNodeID < 0 || fromNodeID >= static_cast<int>(components.size()) ||
            toNodeID < 0 || toNodeID >= static_cast<int>(components.size()))
        {
            std::cerr << "ERRO: ID de nó inválido para componentes: "
                      << fromNodeID << " -> " << toNodeID << std::endl;
            continue;
        }

        // Calcula distância euclidiana
        cost += components[fromNodeID].distanceTo(components[toNodeID]);
    }

    return cost;
}

// ============================================================================
// RECALCULAR ESTADO
// ============================================================================

void Vehicle::recalculateLoad()
{
    currentLoad = 0;

    for (const auto& node : route)
    {
        if (!node.isDepot)
        {
            currentLoad += node.demand;
        }
    }
}

// ============================================================================
// UTILIDADES
// ============================================================================

std::string Vehicle::toString() const
{
    std::ostringstream oss;
    oss << "Vehicle[capacity=" << capacity
        << ", load=" << currentLoad
        << " (" << std::fixed << std::setprecision(1)
        << (getUtilizationRate() * 100.0) << "%)"
        << ", nodes=" << route.size()
        << ", customers=" << getNumberOfCustomers()
        << ", valid=" << (isRouteValid() ? "yes" : "no")
        << ", closed=" << (isRouteClosed() ? "yes" : "no")
        << "]";
    return oss.str();
}

std::string Vehicle::routeToString() const
{
    if (route.empty())
    {
        return "[]";
    }

    std::ostringstream oss;
    oss << "[";

    for (size_t i = 0; i < route.size(); ++i)
    {
        oss << route[i].ID;
        if (i < route.size() - 1)
        {
            oss << " -> ";
        }
    }

    oss << "]";
    return oss.str();
}

double Vehicle::getUtilizationRate() const
{
    if (capacity == 0)
    {
        return 0.0;
    }

    return static_cast<double>(currentLoad) / static_cast<double>(capacity);
}

// ============================================================================
// OPERADOR DE STREAM
// ============================================================================

std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle)
{
    os << vehicle.toString() << "\n";
    os << "Route: " << vehicle.routeToString();
    return os;
}