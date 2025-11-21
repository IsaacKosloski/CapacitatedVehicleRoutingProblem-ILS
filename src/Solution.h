/**
 * @file Solution.h
 * @brief Classe que representa uma solução para o problema CVRP
 * @author Isaac (Original), Melhorado em 2025
 * @date 03/02/2025 (Original), Melhorado em Outubro/2025
 */

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>

// Forward declaration
class CVRP;

/**
 * @class Solution
 * @brief Representa uma solução completa para o problema CVRP
 *
 * Uma solução consiste em um conjunto de rotas, onde cada rota é uma
 * sequência de IDs de nós visitados por um veículo. Todas as rotas devem
 * começar e terminar no depósito.
 *
 * @invariant routes.size() == routeLoads.size() == routesCosts.size()
 * @invariant totalCost == sum(routesCosts)
 * @invariant fleetSize == routes.size()
 * @invariant Cada rota começa e termina no depósito
 */
class Solution
{
public:
    // ========================================================================
    // ATRIBUTOS
    // ========================================================================

    int fleetSize;                          ///< Número de veículos utilizados
    double totalCost;                       ///< Custo total da solução
    std::vector<int> routeLoads;            ///< Carga de cada rota
    std::vector<double> routesCosts;        ///< Custo individual de cada rota
    std::vector<std::vector<int>> routes;   ///< Conjunto de rotas (sequências de IDs)

    // ========================================================================
    // CONSTRUTORES E DESTRUTOR
    // ========================================================================

    /**
     * @brief Construtor padrão
     *
     * Cria uma solução vazia.
     *
     * @post fleetSize == 0
     * @post totalCost == 0.0
     * @post routes.empty() == true
     */
    Solution();

    /**
     * @brief Destrutor padrão
     */
    ~Solution() = default;

    /**
     * @brief Construtor de cópia
     */
    Solution(const Solution& other) = default;

    /**
     * @brief Operador de atribuição
     */
    Solution& operator=(const Solution& other) = default;

    /**
     * @brief Construtor de movimento
     */
    Solution(Solution&& other) noexcept = default;

    /**
     * @brief Operador de atribuição por movimento
     */
    Solution& operator=(Solution&& other) noexcept = default;

    // ========================================================================
    // CÁLCULO DE CUSTO
    // ========================================================================

    /**
     * @brief Calcula o custo total da solução
     *
     * Percorre todas as rotas e calcula o custo de cada uma usando
     * a matriz de distâncias fornecida.
     *
     * @param dimensionOfNodes Número total de nós no problema
     * @param matrixCost Matriz de distâncias (formato: [row*dim + col])
     * @return Custo total calculado
     *
     * @pre dimensionOfNodes > 0
     * @pre matrixCost.size() == dimensionOfNodes * dimensionOfNodes
     *
     * @post totalCost == soma de todos os routesCosts
     * @post routesCosts.size() == routes.size()
     */
    double computeCost(int dimensionOfNodes, std::vector<double>& matrixCost);

    /**
     * @brief Calcula apenas o custo de uma rota específica
     *
     * @param routeIndex Índice da rota
     * @param dimensionOfNodes Dimensão do problema
     * @param matrixCost Matriz de distâncias
     * @return Custo da rota especificada
     *
     * @pre routeIndex >= 0 && routeIndex < routes.size()
     */
    double computeRouteCost(int routeIndex, int dimensionOfNodes,
                           std::vector<double>& matrixCost) const;

    // ========================================================================
    // VALIDAÇÃO
    // ========================================================================

    /**
     * @brief Valida se a solução é viável
     *
     * Verifica:
     * - Todas as rotas começam e terminam no depósito
     * - Nenhum cliente é visitado mais de uma vez
     * - Todos os clientes são visitados exatamente uma vez
     * - Capacidade de cada rota é respeitada
     * - Consistência entre atributos (fleetSize, routeLoads, etc)
     *
     * @param instance Instância do problema CVRP
     * @return true se a solução é válida
     */
    bool validateSolution(CVRP* instance) const;

    /**
     * @brief Verifica apenas a consistência interna da solução
     *
     * Não requer acesso à instância, apenas verifica:
     * - Consistência entre número de rotas e outros atributos
     * - Rotas não-vazias
     * - Custos não-negativos
     *
     * @return true se internamente consistente
     */
    bool isInternallyConsistent() const;

    // ========================================================================
    // SAÍDA E FORMATAÇÃO
    // ========================================================================

    /**
     * @brief Salva a solução em arquivo formatado
     *
     * Formato de saída:
     * - Cabeçalho com informações gerais
     * - Lista de rotas com custos e cargas
     * - Estatísticas detalhadas
     * - Informações de tempo e iterações
     *
     * @param fileName Nome do arquivo de saída
     * @param elapsedTime Tempo de execução em segundos
     * @param iterations Número de iterações executadas
     * @param dimensionOfNodes Dimensão do problema
     *
     * @return true se arquivo foi salvo com sucesso
     */
    bool printSolution(const char* fileName, double elapsedTime,
                      int iterations, int dimensionOfNodes) const;

    /**
     * @brief Imprime solução no console
     *
     * Versão simplificada para debugging.
     */
    void printToConsole() const;

    /**
     * @brief Obtém representação em string da solução
     *
     * @return String formatada com resumo da solução
     */
    std::string toString() const;

    /**
     * @brief Obtém string formatada de uma rota específica
     *
     * @param routeIndex Índice da rota
     * @return String com a rota
     */
    std::string routeToString(int routeIndex) const;

    // ========================================================================
    // MANIPULAÇÃO DE ROTAS
    // ========================================================================

    /**
     * @brief Adiciona uma nova rota vazia
     *
     * @param depotID ID do depósito
     * @post routes contém nova rota [depotID, depotID]
     * @post fleetSize incrementado
     */
    void addEmptyRoute(int depotID);

    /**
     * @brief Remove uma rota específica
     *
     * @param routeIndex Índice da rota a remover
     * @return true se removida com sucesso
     *
     * @pre routeIndex >= 0 && routeIndex < routes.size()
     * @post fleetSize decrementado
     */
    bool removeRoute(int routeIndex);

    /**
     * @brief Remove rotas vazias (apenas depósito)
     *
     * @return Número de rotas removidas
     */
    int removeEmptyRoutes();

    /**
     * @brief Limpa completamente a solução
     *
     * @post routes.empty() == true
     * @post fleetSize == 0
     * @post totalCost == 0.0
     */
    void clear();

    // ========================================================================
    // ESTATÍSTICAS
    // ========================================================================

    /**
     * @brief Calcula estatísticas da solução
     *
     * Struct retornada contém:
     * - Custo médio, mínimo e máximo por rota
     * - Carga média, mínima e máxima
     * - Taxa de utilização média
     * - Desvio padrão dos custos
     *
     * @return Estrutura com estatísticas
     */
    struct Statistics {
        double avgRouteCost;
        double minRouteCost;
        double maxRouteCost;
        double stdDevRouteCost;

        double avgRouteLoad;
        int minRouteLoad;
        int maxRouteLoad;

        double avgUtilization;
        int totalCustomers;
        int totalDistance;
    };

    Statistics computeStatistics() const;

    /**
     * @brief Obtém número total de clientes na solução
     *
     * @return Soma do número de clientes em todas as rotas
     */
    int getTotalCustomers() const;

    /**
     * @brief Verifica se a solução está vazia
     *
     * @return true se não há rotas
     */
    bool isEmpty() const;

    /**
     * @brief Obtém maior carga entre todas as rotas
     *
     * @return Carga máxima
     */
    int getMaxLoad() const;

    /**
     * @brief Obtém menor carga entre todas as rotas
     *
     * @return Carga mínima
     */
    int getMinLoad() const;

    // ========================================================================
    // COMPARAÇÃO
    // ========================================================================

    /**
     * @brief Compara com outra solução (por custo)
     *
     * @param other Outra solução
     * @return true se esta solução é melhor (menor custo)
     */
    bool isBetterThan(const Solution& other) const;

    /**
     * @brief Calcula gap percentual em relação a outra solução
     *
     * Gap = (thisCost - otherCost) / otherCost * 100
     *
     * @param other Solução de referência
     * @return Gap percentual
     */
    double gapTo(const Solution& other) const;

    /**
     * @brief Operador de comparação por custo
     */
    bool operator<(const Solution& other) const;

    /**
     * @brief Operador de comparação por custo
     */
    bool operator>(const Solution& other) const;

    /**
     * @brief Operador de igualdade (compara custos com tolerância)
     */
    bool operator==(const Solution& other) const;

    // ========================================================================
    // CLONAGEM
    // ========================================================================

    /**
     * @brief Cria uma cópia profunda da solução
     *
     * @return Ponteiro único para nova solução
     */
    std::unique_ptr<Solution> clone() const;

    // ========================================================================
    // OPERADOR DE STREAM
    // ========================================================================

    /**
     * @brief Operador de saída para stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Solution& solution);

private:
    // ========================================================================
    // MÉTODOS AUXILIARES PRIVADOS
    // ========================================================================

    /**
     * @brief Calcula desvio padrão dos custos das rotas
     */
    double calculateStdDev() const;

    /**
     * @brief Formata tempo em string legível
     */
    static std::string formatTime(double seconds);

    static constexpr double EPSILON = 1e-9;  ///< Tolerância para comparações
};
