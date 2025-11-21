/**
 * @file CVRP.cpp
 * @brief Implementação da classe CVRP
 * @author Isaac (Original), Melhorado em 2025
 */

#include "CVRP.h"
#include "Scanner.h"
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <cmath>

// ============================================================================
// CONSTRUTORES
// ============================================================================

CVRP::CVRP()
    : nodes()
    , distanceMatrix()
    , depotID(0)
    , nodesDimension(0)
    , capacityOfVehicle(0)
{
}

CVRP::CVRP(const std::string& fileName)
    : CVRP()
{
    loadInstance(fileName);
}

// ============================================================================
// CARREGAMENTO DE DADOS
// ============================================================================

void CVRP::loadInstance(const std::string& fileName)
{
    // Usa Scanner para ler o arquivo
    auto scanner = std::make_unique<Scanner>(fileName);

    // Copia dados do scanner
    this->nodesDimension = scanner->dimensionOfNodes;
    this->capacityOfVehicle = scanner->capacityOfVehicles;
    this->depotID = scanner->depot;

    // Valida dados básicos
    if (nodesDimension <= 0)
    {
        throw std::runtime_error("Dimensão inválida: " + std::to_string(nodesDimension));
    }

    if (capacityOfVehicle <= 0)
    {
        throw std::runtime_error("Capacidade inválida: " + std::to_string(capacityOfVehicle));
    }

    // Copia matriz de distâncias
    this->distanceMatrix = scanner->nodesDistance;

    // Copia nós
    this->nodes = scanner->nodes;

    // Valida instância carregada
    if (!validateInstance())
    {
        throw std::runtime_error("Instância carregada não é válida!");
    }

    std::cout << "Instância carregada com sucesso: " << fileName << std::endl;
    std::cout << "  - Nós: " << nodesDimension << std::endl;
    std::cout << "  - Clientes: " << getNumberOfCustomers() << std::endl;
    std::cout << "  - Capacidade: " << capacityOfVehicle << std::endl;
    std::cout << "  - Depósito: " << depotID << std::endl;
}

void CVRP::createInstance(int numNodes, int capacity, int depotID)
{
    this->nodesDimension = numNodes;
    this->capacityOfVehicle = capacity;
    this->depotID = depotID;

    // Inicializa nós
    nodes.clear();
    nodes.reserve(numNodes);

    for (int i = 0; i < numNodes; ++i)
    {
        int demand = (i == depotID) ? 0 : 1; // Demanda padrão
        nodes.emplace_back(i, demand);
    }

    // Inicializa matriz de distâncias com zeros
    distanceMatrix.resize(numNodes * numNodes, 0.0);
}

// ============================================================================
// CONSULTAS DA MATRIZ DE DISTÂNCIAS
// ============================================================================

double CVRP::getDistance(int fromNode, int toNode) const
{
    if (!isValidNodeID(fromNode) || !isValidNodeID(toNode))
    {
        std::cerr << "ERRO: IDs de nós inválidos: " << fromNode
                  << " -> " << toNode << std::endl;
        return 0.0;
    }

    int index = getMatrixIndex(fromNode, toNode);

    if (!isValidMatrixIndex(index))
    {
        std::cerr << "ERRO: Índice de matriz inválido: " << index << std::endl;
        return 0.0;
    }

    return distanceMatrix[index];
}

void CVRP::setDistance(int fromNode, int toNode, double distance)
{
    if (!isValidNodeID(fromNode) || !isValidNodeID(toNode))
    {
        std::cerr << "ERRO: IDs de nós inválidos: " << fromNode
                  << " -> " << toNode << std::endl;
        return;
    }

    if (distance < 0)
    {
        std::cerr << "AVISO: Distância negativa definida: " << distance
                  << ". Usando 0." << std::endl;
        distance = 0;
    }

    int index = getMatrixIndex(fromNode, toNode);

    if (isValidMatrixIndex(index))
    {
        distanceMatrix[index] = distance;
    }
}

int CVRP::getMatrixIndex(int fromNode, int toNode) const
{
    return fromNode * nodesDimension + toNode;
}

// ============================================================================
// CONSULTAS DE NÓDOS
// ============================================================================

const Node& CVRP::getNode(int nodeID) const
{
    if (!isValidNodeID(nodeID))
    {
        throw std::out_of_range("ID de nó fora do intervalo: " + std::to_string(nodeID));
    }
    return nodes[nodeID];
}

Node& CVRP::getNode(int nodeID)
{
    if (!isValidNodeID(nodeID))
    {
        throw std::out_of_range("ID de nó fora do intervalo: " + std::to_string(nodeID));
    }
    return nodes[nodeID];
}

const Node& CVRP::getDepot() const
{
    return getNode(depotID);
}

int CVRP::getNumberOfCustomers() const
{
    int count = 0;
    for (const auto& node : nodes)
    {
        if (!node.isDepot)
        {
            count++;
        }
    }
    return count;
}

bool CVRP::isDepot(int nodeID) const
{
    return nodeID == depotID;
}

// ============================================================================
// VALIDAÇÃO
// ============================================================================

bool CVRP::validateInstance() const
{
    bool isValid = true;

    // 1. Valida dimensão
    if (nodesDimension <= 0)
    {
        std::cerr << "ERRO: Dimensão inválida: " << nodesDimension << std::endl;
        isValid = false;
    }

    // 2. Valida capacidade
    if (capacityOfVehicle <= 0)
    {
        std::cerr << "ERRO: Capacidade inválida: " << capacityOfVehicle << std::endl;
        isValid = false;
    }

    // 3. Valida depósito
    if (!isValidNodeID(depotID))
    {
        std::cerr << "ERRO: ID de depósito inválido: " << depotID << std::endl;
        isValid = false;
    }

    // 4. Valida tamanho da matriz de distâncias
    int expectedSize = nodesDimension * nodesDimension;
    if (static_cast<int>(distanceMatrix.size()) != expectedSize)
    {
        std::cerr << "ERRO: Tamanho da matriz de distâncias incorreto. "
                  << "Esperado: " << expectedSize
                  << ", Atual: " << distanceMatrix.size() << std::endl;
        isValid = false;
    }

    // 5. Valida número de nós
    if (static_cast<int>(nodes.size()) != nodesDimension)
    {
        std::cerr << "ERRO: Número de nós incorreto. "
                  << "Esperado: " << nodesDimension
                  << ", Atual: " << nodes.size() << std::endl;
        isValid = false;
    }

    // 6. Valida cada nó
    for (int i = 0; i < static_cast<int>(nodes.size()); ++i)
    {
        if (!nodes[i].isValid())
        {
            std::cerr << "ERRO: Nó " << i << " não é válido!" << std::endl;
            isValid = false;
        }

        if (nodes[i].ID != i)
        {
            std::cerr << "AVISO: ID do nó " << i << " não corresponde ao índice. "
                     << "ID: " << nodes[i].ID << std::endl;
        }
    }

    // 7. Valida que depósito tem demanda zero
    if (isValidNodeID(depotID))
    {
        if (nodes[depotID].demand != 0)
        {
            std::cerr << "ERRO: Depósito tem demanda não-zero: "
                     << nodes[depotID].demand << std::endl;
            isValid = false;
        }

        if (!nodes[depotID].isDepot)
        {
            std::cerr << "ERRO: Nó depósito não está marcado como depósito!" << std::endl;
            isValid = false;
        }
    }

    // 8. Valida distâncias
    for (int i = 0; i < nodesDimension; ++i)
    {
        for (int j = 0; j < nodesDimension; ++j)
        {
            double dist = getDistance(i, j);

            if (dist < 0)
            {
                std::cerr << "ERRO: Distância negativa encontrada: "
                         << i << " -> " << j << " = " << dist << std::endl;
                isValid = false;
            }

            // Distância de um nó para ele mesmo deve ser zero
            if (i == j && std::abs(dist) > EPSILON)
            {
                std::cerr << "AVISO: Distância diagonal não é zero: "
                         << i << " -> " << j << " = " << dist << std::endl;
            }
        }
    }

    // 9. Valida demandas
    if (!areDemandsValid())
    {
        isValid = false;
    }

    return isValid;
}

bool CVRP::areDemandsValid() const
{
    bool valid = true;

    for (const auto& node : nodes)
    {
        if (node.isDepot) continue;

        if (node.demand > capacityOfVehicle)
        {
            std::cerr << "ERRO: Demanda do nó " << node.ID
                     << " (" << node.demand << ") excede capacidade ("
                     << capacityOfVehicle << ")" << std::endl;
            valid = false;
        }

        if (node.demand < 0)
        {
            std::cerr << "ERRO: Demanda negativa no nó " << node.ID
                     << ": " << node.demand << std::endl;
            valid = false;
        }
    }

    return valid;
}

// ============================================================================
// INFORMAÇÕES E ESTATÍSTICAS
// ============================================================================

int CVRP::getTotalDemand() const
{
    int total = 0;
    for (const auto& node : nodes)
    {
        if (!node.isDepot)
        {
            total += node.demand;
        }
    }
    return total;
}

int CVRP::getMinimumVehicles() const
{
    if (capacityOfVehicle == 0)
    {
        return 0;
    }

    int totalDemand = getTotalDemand();
    return (totalDemand + capacityOfVehicle - 1) / capacityOfVehicle; // Ceiling division
}

double CVRP::getMaxDistance() const
{
    if (distanceMatrix.empty())
    {
        return 0.0;
    }

    return *std::max_element(distanceMatrix.begin(), distanceMatrix.end());
}

double CVRP::getMinDistance() const
{
    if (distanceMatrix.empty())
    {
        return 0.0;
    }

    double minDist = std::numeric_limits<double>::max();

    for (double dist : distanceMatrix)
    {
        if (dist > EPSILON && dist < minDist)
        {
            minDist = dist;
        }
    }

    return (minDist == std::numeric_limits<double>::max()) ? 0.0 : minDist;
}

double CVRP::getAverageDistance() const
{
    if (distanceMatrix.empty())
    {
        return 0.0;
    }

    double sum = std::accumulate(distanceMatrix.begin(), distanceMatrix.end(), 0.0);
    return sum / distanceMatrix.size();
}

CVRP::Statistics CVRP::getStatistics() const
{
    Statistics stats = {};

    stats.numNodes = nodesDimension;
    stats.numCustomers = getNumberOfCustomers();
    stats.capacity = capacityOfVehicle;
    stats.totalDemand = getTotalDemand();
    stats.minVehicles = getMinimumVehicles();
    stats.maxDistance = getMaxDistance();
    stats.minDistance = getMinDistance();
    stats.avgDistance = getAverageDistance();

    // Estatísticas de demanda
    if (!nodes.empty())
    {
        int maxDemand = 0;
        int minDemand = std::numeric_limits<int>::max();
        int totalDemand = 0;
        int customerCount = 0;

        for (const auto& node : nodes)
        {
            if (!node.isDepot)
            {
                maxDemand = std::max(maxDemand, node.demand);
                minDemand = std::min(minDemand, node.demand);
                totalDemand += node.demand;
                customerCount++;
            }
        }

        stats.maxDemand = maxDemand;
        stats.minDemand = (minDemand == std::numeric_limits<int>::max()) ? 0 : minDemand;
        stats.avgDemand = (customerCount > 0) ?
            static_cast<double>(totalDemand) / customerCount : 0.0;
    }

    return stats;
}

// ============================================================================
// SAÍDA E FORMATAÇÃO
// ============================================================================

void CVRP::print(std::ostream& os) const
{
    os << toString() << std::endl;
}

std::string CVRP::toString() const
{
    std::ostringstream oss;
    oss << "CVRP Instance["
        << "nodes=" << nodesDimension
        << ", customers=" << getNumberOfCustomers()
        << ", capacity=" << capacityOfVehicle
        << ", depot=" << depotID
        << ", totalDemand=" << getTotalDemand()
        << ", minVehicles=" << getMinimumVehicles()
        << "]";
    return oss.str();
}

void CVRP::printDistanceMatrix(std::ostream& os) const
{
    os << "\n========================================" << std::endl;
    os << "MATRIZ DE DISTÂNCIAS" << std::endl;
    os << "========================================\n" << std::endl;

    // Cabeçalho
    os << std::setw(6) << " ";
    for (int j = 0; j < nodesDimension; ++j)
    {
        os << std::setw(8) << j;
    }
    os << std::endl;

    os << std::string(6 + nodesDimension * 8, '-') << std::endl;

    // Linhas
    for (int i = 0; i < nodesDimension; ++i)
    {
        os << std::setw(4) << i << " |";
        for (int j = 0; j < nodesDimension; ++j)
        {
            os << std::setw(8) << std::fixed << std::setprecision(2)
               << getDistance(i, j);
        }
        os << std::endl;
    }

    os << std::endl;
}

void CVRP::printNodes(std::ostream& os) const
{
    os << "\n========================================" << std::endl;
    os << "LISTA DE NÓS" << std::endl;
    os << "========================================\n" << std::endl;

    os << std::setw(6) << "ID"
       << std::setw(12) << "Demanda"
       << std::setw(12) << "Tipo" << std::endl;
    os << std::string(30, '-') << std::endl;

    for (const auto& node : nodes)
    {
        os << std::setw(6) << node.ID
           << std::setw(12) << node.demand
           << std::setw(12) << (node.isDepot ? "Depósito" : "Cliente")
           << std::endl;
    }

    os << std::string(30, '-') << std::endl;
    os << "Total de clientes: " << getNumberOfCustomers() << std::endl;
    os << "Demanda total: " << getTotalDemand() << std::endl;
    os << std::endl;
}

bool CVRP::saveToFile(const std::string& fileName) const
{
    std::ofstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "ERRO: Não foi possível abrir arquivo para escrita: "
                  << fileName << std::endl;
        return false;
    }

    file << "NAME : " << fileName << std::endl;
    file << "COMMENT : CVRP Instance" << std::endl;
    file << "TYPE : CVRP" << std::endl;
    file << "DIMENSION : " << nodesDimension << std::endl;
    file << "EDGE_WEIGHT_TYPE : EXPLICIT" << std::endl;
    file << "EDGE_WEIGHT_FORMAT : FULL_MATRIX" << std::endl;
    file << "CAPACITY : " << capacityOfVehicle << std::endl;

    file << "EDGE_WEIGHT_SECTION" << std::endl;
    for (int i = 0; i < nodesDimension; ++i)
    {
        for (int j = 0; j < nodesDimension; ++j)
        {
            file << std::fixed << std::setprecision(2) << getDistance(i, j) << " ";
        }
        file << std::endl;
    }

    file << "DEMAND_SECTION" << std::endl;
    for (const auto& node : nodes)
    {
        file << node.ID << " " << node.demand << std::endl;
    }

    file << "DEPOT_SECTION" << std::endl;
    file << depotID << std::endl;
    file << "-1" << std::endl;
    file << "EOF" << std::endl;

    file.close();
    return true;
}

// ============================================================================
// MÉTODOS AUXILIARES PRIVADOS
// ============================================================================

bool CVRP::isValidNodeID(int nodeID) const
{
    return nodeID >= 0 && nodeID < nodesDimension;
}

bool CVRP::isValidMatrixIndex(int index) const
{
    return index >= 0 && index < static_cast<int>(distanceMatrix.size());
}

// ============================================================================
// OPERADOR DE STREAM
// ============================================================================

std::ostream& operator<<(std::ostream& os, const CVRP& cvrp)
{
    os << cvrp.toString();
    return os;
}