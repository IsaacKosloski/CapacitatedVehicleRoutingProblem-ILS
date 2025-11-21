/**
 * @file Functions.cpp
 * @brief Implementação das funções utilitárias
 * @author Isaac (Original), Melhorado em 2025
 */

#include "Functions.h"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <fstream>
#include <cmath>

namespace CVRPUtils
{

// ============================================================================
// IMPRESSÃO DE INSTÂNCIA
// ============================================================================

void printInstance(const CVRP* cvrp, bool showDistanceMatrix, bool showNodes)
{
    if (!cvrp)
    {
        std::cerr << "ERRO: Ponteiro CVRP nulo!" << std::endl;
        return;
    }

    printHeader("INFORMAÇÕES DA INSTÂNCIA");

    std::cout << "Dimensão: " << cvrp->nodesDimension << " nós" << std::endl;
    std::cout << "Clientes: " << cvrp->getNumberOfCustomers() << std::endl;
    std::cout << "Capacidade do veículo: " << cvrp->capacityOfVehicle << std::endl;
    std::cout << "Depósito: " << cvrp->depotID << std::endl;
    std::cout << "Demanda total: " << cvrp->getTotalDemand() << std::endl;
    std::cout << "Veículos mínimos (limite inferior): "
              << cvrp->getMinimumVehicles() << std::endl;

    printSeparator();

    if (showDistanceMatrix)
    {
        printDistanceMatrix(cvrp);
    }

    if (showNodes)
    {
        printNodesList(cvrp);
    }
}

void printInstanceSummary(const CVRP* cvrp)
{
    if (!cvrp)
    {
        std::cerr << "ERRO: Ponteiro CVRP nulo!" << std::endl;
        return;
    }

    std::cout << "Instância: " << cvrp->nodesDimension << " nós, "
              << "capacidade " << cvrp->capacityOfVehicle
              << ", demanda total " << cvrp->getTotalDemand() << std::endl;
}

void printDistanceMatrix(const CVRP* cvrp, int maxNodesToShow)
{
    if (!cvrp)
    {
        std::cerr << "ERRO: Ponteiro CVRP nulo!" << std::endl;
        return;
    }

    printHeader("MATRIZ DE DISTÂNCIAS");

    int nodesToShow = (maxNodesToShow > 0 && maxNodesToShow < cvrp->nodesDimension)
                      ? maxNodesToShow : cvrp->nodesDimension;

    // Cabeçalho
    std::cout << std::setw(6) << " ";
    for (int j = 0; j < nodesToShow; ++j)
    {
        std::cout << std::setw(8) << j;
    }
    if (nodesToShow < cvrp->nodesDimension)
    {
        std::cout << "  ...";
    }
    std::cout << std::endl;

    printSeparator(6 + nodesToShow * 8, '-');

    // Linhas
    for (int i = 0; i < nodesToShow; ++i)
    {
        std::cout << std::setw(4) << i << " |";
        for (int j = 0; j < nodesToShow; ++j)
        {
            std::cout << std::setw(8) << std::fixed << std::setprecision(2)
                     << cvrp->getDistance(i, j);
        }
        if (nodesToShow < cvrp->nodesDimension)
        {
            std::cout << "  ...";
        }
        std::cout << std::endl;
    }

    if (nodesToShow < cvrp->nodesDimension)
    {
        std::cout << " ... (mostrando apenas " << nodesToShow
                 << " de " << cvrp->nodesDimension << " nós)" << std::endl;
    }

    printSeparator();
}

void printNodesList(const CVRP* cvrp)
{
    if (!cvrp)
    {
        std::cerr << "ERRO: Ponteiro CVRP nulo!" << std::endl;
        return;
    }

    printHeader("LISTA DE NÓS");

    std::cout << std::setw(6) << "ID"
             << std::setw(12) << "Demanda"
             << std::setw(15) << "Tipo" << std::endl;
    printSeparator(33, '-');

    for (const auto& node : cvrp->nodes)
    {
        std::cout << std::setw(6) << node.ID
                 << std::setw(12) << node.demand
                 << std::setw(15) << (node.isDepot ? "Depósito" : "Cliente")
                 << std::endl;
    }

    printSeparator(33, '-');
    std::cout << "Total de clientes: " << cvrp->getNumberOfCustomers() << std::endl;
    std::cout << "Demanda total: " << cvrp->getTotalDemand() << std::endl;
    printSeparator();
}

// ============================================================================
// IMPRESSÃO DE SOLUÇÃO
// ============================================================================

void printSolution(const Solution* solution, bool detailed)
{
    if (!solution)
    {
        std::cerr << "ERRO: Ponteiro Solution nulo!" << std::endl;
        return;
    }

    printHeader("SOLUÇÃO");

    std::cout << "Número de rotas: " << solution->fleetSize << std::endl;
    std::cout << "Custo total: " << std::fixed << std::setprecision(2)
             << solution->totalCost << std::endl;
    std::cout << "Clientes atendidos: " << solution->getTotalCustomers() << std::endl;

    printSeparator();

    printRoutes(solution, detailed, detailed);

    if (detailed)
    {
        printStatistics(solution);
    }
}

void printSolutionSummary(const Solution* solution)
{
    if (!solution)
    {
        std::cerr << "ERRO: Ponteiro Solution nulo!" << std::endl;
        return;
    }

    std::cout << "Solução: " << solution->fleetSize << " rotas, "
             << "custo " << std::fixed << std::setprecision(2) << solution->totalCost
             << ", " << solution->getTotalCustomers() << " clientes" << std::endl;
}

void printRoutes(const Solution* solution, bool showCosts, bool showLoads)
{
    if (!solution)
    {
        std::cerr << "ERRO: Ponteiro Solution nulo!" << std::endl;
        return;
    }

    printHeader("ROTAS");

    for (size_t r = 0; r < solution->routes.size(); ++r)
    {
        const auto& route = solution->routes[r];

        std::cout << "Rota #" << (r + 1) << ": ";

        // Imprime sequência de nós
        for (size_t i = 0; i < route.size(); ++i)
        {
            std::cout << route[i];
            if (i < route.size() - 1)
            {
                std::cout << " → ";
            }
        }

        // Informações adicionais
        std::cout << " ";

        if (showCosts && r < solution->routesCosts.size())
        {
            std::cout << "[custo: " << std::fixed << std::setprecision(2)
                     << solution->routesCosts[r] << "]";
        }

        if (showLoads && r < solution->routeLoads.size())
        {
            std::cout << " [carga: " << solution->routeLoads[r] << "]";
        }

        std::cout << std::endl;
    }

    printSeparator();
}

// ============================================================================
// ESTATÍSTICAS E ANÁLISE
// ============================================================================

void printStatistics(const Solution* solution, const CVRP* cvrp)
{
    if (!solution)
    {
        std::cerr << "ERRO: Ponteiro Solution nulo!" << std::endl;
        return;
    }

    printHeader("ESTATÍSTICAS DA SOLUÇÃO");

    auto stats = solution->computeStatistics();

    std::cout << std::fixed << std::setprecision(2);

    // Estatísticas de custo
    if (!solution->routesCosts.empty())
    {
        std::cout << "\n=== Custos ===" << std::endl;
        std::cout << "Custo total: " << solution->totalCost << std::endl;
        std::cout << "Custo médio por rota: " << stats.avgRouteCost << std::endl;
        std::cout << "Custo máximo: " << stats.maxRouteCost << std::endl;
        std::cout << "Custo mínimo: " << stats.minRouteCost << std::endl;
        std::cout << "Desvio padrão: " << stats.stdDevRouteCost << std::endl;
    }

    // Estatísticas de carga
    if (!solution->routeLoads.empty())
    {
        std::cout << "\n=== Cargas ===" << std::endl;
        std::cout << "Carga média por rota: " << stats.avgRouteLoad << std::endl;
        std::cout << "Carga máxima: " << stats.maxRouteLoad << std::endl;
        std::cout << "Carga mínima: " << stats.minRouteLoad << std::endl;

        if (cvrp)
        {
            double utilization = static_cast<double>(stats.maxRouteLoad) /
                               cvrp->capacityOfVehicle * 100.0;
            std::cout << "Taxa de utilização máxima: " << utilization << "%" << std::endl;
        }
    }

    // Informações gerais
    std::cout << "\n=== Geral ===" << std::endl;
    std::cout << "Número de rotas: " << solution->fleetSize << std::endl;
    std::cout << "Total de clientes: " << stats.totalCustomers << std::endl;

    if (cvrp)
    {
        std::cout << "Clientes por rota (média): "
                 << static_cast<double>(stats.totalCustomers) / solution->fleetSize
                 << std::endl;
    }

    printSeparator();
}

void printQualityAnalysis(const Solution* solution, const CVRP* cvrp,
                         double bestKnownCost)
{
    if (!solution || !cvrp)
    {
        std::cerr << "ERRO: Ponteiro nulo!" << std::endl;
        return;
    }

    printHeader("ANÁLISE DE QUALIDADE");

    std::cout << std::fixed << std::setprecision(2);

    // Custo atual
    std::cout << "Custo da solução: " << solution->totalCost << std::endl;

    // Limite inferior teórico
    int minVehicles = cvrp->getMinimumVehicles();
    std::cout << "\nVeículos usados: " << solution->fleetSize << std::endl;
    std::cout << "Veículos mínimos (teórico): " << minVehicles << std::endl;

    if (solution->fleetSize > minVehicles)
    {
        std::cout << "  → " << (solution->fleetSize - minVehicles)
                 << " veículos acima do mínimo teórico" << std::endl;
    }
    else if (solution->fleetSize == minVehicles)
    {
        std::cout << "  ✓ Número ótimo de veículos alcançado!" << std::endl;
    }

    // Comparação com best known
    if (bestKnownCost > 0)
    {
        std::cout << "\nMelhor custo conhecido: " << bestKnownCost << std::endl;
        double gap = ((solution->totalCost - bestKnownCost) / bestKnownCost) * 100.0;
        std::cout << "Gap: " << std::setprecision(2) << gap << "%" << std::endl;

        if (gap < 0.01)
        {
            std::cout << "  ✓ Solução ótima ou muito próxima!" << std::endl;
        }
        else if (gap < 1.0)
        {
            std::cout << "  ✓ Solução de alta qualidade!" << std::endl;
        }
        else if (gap < 5.0)
        {
            std::cout << "  → Solução de qualidade boa" << std::endl;
        }
        else
        {
            std::cout << "  → Há espaço para melhoria" << std::endl;
        }
    }

    printSeparator();
}

void printLowerBounds(const CVRP* cvrp)
{
    if (!cvrp)
    {
        std::cerr << "ERRO: Ponteiro CVRP nulo!" << std::endl;
        return;
    }

    printHeader("LIMITES INFERIORES");

    // Limite baseado em capacidade
    int minVehiclesByCapacity = cvrp->getMinimumVehicles();
    std::cout << "Limite por capacidade: " << minVehiclesByCapacity
             << " veículos" << std::endl;

    // Limite baseado em distância (simplificado)
    // Cada cliente deve ser visitado pelo menos uma vez
    double minDistanceSum = 0.0;
    for (int i = 1; i < cvrp->nodesDimension; ++i)
    {
        double minDist = cvrp->getDistance(cvrp->depotID, i);
        minDistanceSum += 2.0 * minDist; // Ida e volta
    }

    std::cout << "Limite por distância (simplificado): "
             << std::fixed << std::setprecision(2)
             << minDistanceSum << std::endl;

    printSeparator();
}

// ============================================================================
// VALIDAÇÃO
// ============================================================================

bool validateAndPrint(const Solution* solution, const CVRP* cvrp)
{
    if (!solution || !cvrp)
    {
        std::cerr << "ERRO: Ponteiro nulo!" << std::endl;
        return false;
    }

    printHeader("VALIDAÇÃO DA SOLUÇÃO");

    bool isValid = solution->validateSolution(const_cast<CVRP*>(cvrp));

    if (isValid)
    {
        std::cout << "✓ SOLUÇÃO VÁLIDA" << std::endl;
    }
    else
    {
        std::cout << "✗ SOLUÇÃO INVÁLIDA - Verifique os erros acima" << std::endl;
    }

    printSeparator();

    return isValid;
}

int checkCapacityViolations(const Solution* solution, const CVRP* cvrp)
{
    if (!solution || !cvrp)
    {
        std::cerr << "ERRO: Ponteiro nulo!" << std::endl;
        return -1;
    }

    int violations = 0;

    printHeader("VERIFICAÇÃO DE CAPACIDADE");

    for (size_t r = 0; r < solution->routes.size(); ++r)
    {
        int load = 0;

        for (size_t i = 1; i < solution->routes[r].size() - 1; ++i)
        {
            int nodeID = solution->routes[r][i];
            if (nodeID >= 0 && nodeID < cvrp->nodesDimension)
            {
                load += cvrp->nodes[nodeID].demand;
            }
        }

        if (load > cvrp->capacityOfVehicle)
        {
            std::cout << "✗ Rota #" << (r + 1) << ": Carga " << load
                     << " excede capacidade " << cvrp->capacityOfVehicle
                     << " (violação: " << (load - cvrp->capacityOfVehicle) << ")"
                     << std::endl;
            violations++;
        }
    }

    if (violations == 0)
    {
        std::cout << "✓ Todas as rotas respeitam a capacidade" << std::endl;
    }
    else
    {
        std::cout << "\nTotal de violações: " << violations << std::endl;
    }

    printSeparator();

    return violations;
}

std::vector<int> checkUnvisitedCustomers(const Solution* solution, const CVRP* cvrp)
{
    std::vector<int> unvisited;

    if (!solution || !cvrp)
    {
        std::cerr << "ERRO: Ponteiro nulo!" << std::endl;
        return unvisited;
    }

    printHeader("VERIFICAÇÃO DE COBERTURA");

    std::vector<bool> visited(cvrp->nodesDimension, false);
    visited[cvrp->depotID] = true;

    // Marca todos os visitados
    for (const auto& route : solution->routes)
    {
        for (int nodeID : route)
        {
            if (nodeID >= 0 && nodeID < cvrp->nodesDimension)
            {
                visited[nodeID] = true;
            }
        }
    }

    // Encontra não-visitados
    for (int i = 0; i < cvrp->nodesDimension; ++i)
    {
        if (!visited[i] && !cvrp->nodes[i].isDepot)
        {
            unvisited.push_back(i);
        }
    }

    if (unvisited.empty())
    {
        std::cout << "✓ Todos os clientes foram visitados" << std::endl;
    }
    else
    {
        std::cout << "✗ Clientes não visitados (" << unvisited.size() << "): ";
        for (size_t i = 0; i < unvisited.size(); ++i)
        {
            std::cout << unvisited[i];
            if (i < unvisited.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    printSeparator();

    return unvisited;
}

// ============================================================================
// COMPARAÇÃO DE SOLUÇÕES
// ============================================================================

void compareSolutions(const Solution* solution1, const Solution* solution2,
                     const std::string& label1, const std::string& label2)
{
    if (!solution1 || !solution2)
    {
        std::cerr << "ERRO: Ponteiro nulo!" << std::endl;
        return;
    }

    printHeader("COMPARAÇÃO DE SOLUÇÕES");

    std::cout << std::setw(30) << " "
             << std::setw(20) << label1
             << std::setw(20) << label2
             << std::setw(15) << "Diferença" << std::endl;
    printSeparator();

    std::cout << std::fixed << std::setprecision(2);

    // Custo
    std::cout << std::setw(30) << "Custo total:"
             << std::setw(20) << solution1->totalCost
             << std::setw(20) << solution2->totalCost
             << std::setw(15) << (solution2->totalCost - solution1->totalCost)
             << std::endl;

    // Número de rotas
    std::cout << std::setw(30) << "Número de rotas:"
             << std::setw(20) << solution1->fleetSize
             << std::setw(20) << solution2->fleetSize
             << std::setw(15) << (solution2->fleetSize - solution1->fleetSize)
             << std::endl;

    // Clientes
    int customers1 = solution1->getTotalCustomers();
    int customers2 = solution2->getTotalCustomers();
    std::cout << std::setw(30) << "Clientes atendidos:"
             << std::setw(20) << customers1
             << std::setw(20) << customers2
             << std::setw(15) << (customers2 - customers1)
             << std::endl;

    // Gap percentual
    double gap = ((solution2->totalCost - solution1->totalCost) /
                  solution1->totalCost) * 100.0;
    std::cout << "\nGap: " << std::setprecision(2) << gap << "%" << std::endl;

    if (solution2->totalCost < solution1->totalCost)
    {
        std::cout << "✓ " << label2 << " é melhor" << std::endl;
    }
    else if (solution2->totalCost > solution1->totalCost)
    {
        std::cout << "✓ " << label1 << " é melhor" << std::endl;
    }
    else
    {
        std::cout << "= Soluções equivalentes" << std::endl;
    }

    printSeparator();
}

void printGap(double currentCost, double referenceCost,
             const std::string& referenceLabel)
{
    if (referenceCost <= 0)
    {
        std::cerr << "Custo de referência inválido" << std::endl;
        return;
    }

    double gap = ((currentCost - referenceCost) / referenceCost) * 100.0;

    std::cout << "Gap em relação a " << referenceLabel << ": "
             << std::fixed << std::setprecision(2) << gap << "%"
             << " (diferença absoluta: " << (currentCost - referenceCost) << ")"
             << std::endl;
}

// ============================================================================
// FORMATAÇÃO E UTILITÁRIOS
// ============================================================================

void printSeparator(int length, char character)
{
    std::cout << std::string(length, character) << std::endl;
}

void printHeader(const std::string& title, int length)
{
    printSeparator(length, '=');
    int padding = (length - title.length()) / 2;
    std::cout << std::string(padding, ' ') << title << std::endl;
    printSeparator(length, '=');
}

std::string formatTime(double seconds)
{
    int hours = static_cast<int>(seconds / 3600);
    int minutes = static_cast<int>((seconds - hours * 3600) / 60);
    double secs = seconds - hours * 3600 - minutes * 60;

    std::ostringstream oss;

    if (hours > 0)
    {
        oss << hours << "h ";
    }
    if (minutes > 0 || hours > 0)
    {
        oss << minutes << "min ";
    }
    oss << std::fixed << std::setprecision(2) << secs << "s";

    return oss.str();
}

std::string formatNumber(double value, int decimals)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimals) << value;
    return oss.str();
}

std::string formatPercentage(double value, int decimals)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimals) << (value * 100.0) << "%";
    return oss.str();
}

// ============================================================================
// EXPORTAÇÃO
// ============================================================================

bool exportToCSV(const Solution* solution, const std::string& fileName)
{
    if (!solution)
    {
        std::cerr << "ERRO: Ponteiro Solution nulo!" << std::endl;
        return false;
    }

    std::ofstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "ERRO: Não foi possível abrir arquivo: " << fileName << std::endl;
        return false;
    }

    // Cabeçalho CSV
    file << "route_id,node_sequence,cost,load" << std::endl;

    // Dados
    for (size_t r = 0; r < solution->routes.size(); ++r)
    {
        std::ostringstream sequence;
        for (size_t i = 0; i < solution->routes[r].size(); ++i)
        {
            sequence << solution->routes[r][i];
            if (i < solution->routes[r].size() - 1)
            {
                sequence << "-";
            }
        }

        file << (r + 1) << ","
             << "\"" << sequence.str() << "\",";

        if (r < solution->routesCosts.size())
        {
            file << solution->routesCosts[r];
        }
        else
        {
            file << "0";
        }

        file << ",";

        if (r < solution->routeLoads.size())
        {
            file << solution->routeLoads[r];
        }
        else
        {
            file << "0";
        }

        file << std::endl;
    }

    file.close();
    std::cout << "✓ Solução exportada para CSV: " << fileName << std::endl;
    return true;
}

bool exportRoutesToText(const Solution* solution, const std::string& fileName)
{
    if (!solution)
    {
        std::cerr << "ERRO: Ponteiro Solution nulo!" << std::endl;
        return false;
    }

    std::ofstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "ERRO: Não foi possível abrir arquivo: " << fileName << std::endl;
        return false;
    }

    for (size_t r = 0; r < solution->routes.size(); ++r)
    {
        file << "Route " << (r + 1) << ": ";

        for (size_t i = 0; i < solution->routes[r].size(); ++i)
        {
            file << solution->routes[r][i];
            if (i < solution->routes[r].size() - 1)
            {
                file << " ";
            }
        }

        file << std::endl;
    }

    file << "Cost " << std::fixed << std::setprecision(2)
         << solution->totalCost << std::endl;

    file.close();
    std::cout << "✓ Rotas exportadas para: " << fileName << std::endl;
    return true;
}

bool exportStatisticsToJSON(const Solution* solution, const CVRP* cvrp,
                           const std::string& fileName)
{
    if (!solution)
    {
        std::cerr << "ERRO: Ponteiro Solution nulo!" << std::endl;
        return false;
    }

    std::ofstream file(fileName);

    if (!file.is_open())
    {
        std::cerr << "ERRO: Não foi possível abrir arquivo: " << fileName << std::endl;
        return false;
    }

    auto stats = solution->computeStatistics();

    file << "{" << std::endl;
    file << "  \"totalCost\": " << solution->totalCost << "," << std::endl;
    file << "  \"fleetSize\": " << solution->fleetSize << "," << std::endl;
    file << "  \"totalCustomers\": " << stats.totalCustomers << "," << std::endl;
    file << "  \"avgRouteCost\": " << stats.avgRouteCost << "," << std::endl;
    file << "  \"maxRouteCost\": " << stats.maxRouteCost << "," << std::endl;
    file << "  \"minRouteCost\": " << stats.minRouteCost << "," << std::endl;
    file << "  \"avgRouteLoad\": " << stats.avgRouteLoad << "," << std::endl;
    file << "  \"maxRouteLoad\": " << stats.maxRouteLoad << "," << std::endl;
    file << "  \"minRouteLoad\": " << stats.minRouteLoad;

    if (cvrp)
    {
        file << "," << std::endl;
        file << "  \"instanceDimension\": " << cvrp->nodesDimension << "," << std::endl;
        file << "  \"vehicleCapacity\": " << cvrp->capacityOfVehicle << "," << std::endl;
        file << "  \"totalDemand\": " << cvrp->getTotalDemand() << std::endl;
    }
    else
    {
        file << std::endl;
    }

    file << "}" << std::endl;

    file.close();
    std::cout << "✓ Estatísticas exportadas para JSON: " << fileName << std::endl;
    return true;
}

// ============================================================================
// VISUALIZAÇÃO
// ============================================================================

bool generatePlotData(const Solution* solution, const CVRP* cvrp,
                     const std::string& fileName)
{
    // Implementação simplificada - pode ser expandida para suportar
    // diferentes formatos de visualização
    std::cout << "Geração de dados de plotagem não implementada nesta versão" << std::endl;
    return false;
}

bool generateGnuplotScript(const std::string& dataFileName,
                          const std::string& outputImageFile,
                          const std::string& scriptFileName)
{
    // Implementação simplificada
    std::cout << "Geração de script gnuplot não implementada nesta versão" << std::endl;
    return false;
}

} // namespace CVRPUtils