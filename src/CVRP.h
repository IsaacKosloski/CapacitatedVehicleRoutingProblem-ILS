/**
 * @file CVRP.h
 * @brief Classe que representa uma instância do problema CVRP
 * @author Isaac (Original), Melhorado em 2025
 * @date 30/01/2025 (Original), Melhorado em Outubro/2025
 */

#pragma once

#include "Node.h"
#include "Component.h"
#include <vector>
#include <string>
#include <memory>

// Forward declaration
class Scanner;

/**
 * @class CVRP
 * @brief Representa uma instância completa do problema CVRP
 *
 * Esta classe armazena todos os dados necessários para resolver um problema
 * de roteamento de veículos com restrição de capacidade:
 * - Conjunto de nós (clientes e depósito)
 * - Matriz de distâncias entre todos os nós
 * - Capacidade dos veículos
 * - Identificação do depósito
 *
 * Os dados são carregados de um arquivo no formato padrão CVRP.
 *
 * @invariant nodesDimension > 0
 * @invariant capacityOfVehicle > 0
 * @invariant distanceMatrix.size() == nodesDimension * nodesDimension
 * @invariant nodes.size() == nodesDimension
 * @invariant depotID >= 0 && depotID < nodesDimension
 */
class CVRP
{
public:
    // ========================================================================
    // ATRIBUTOS
    // ========================================================================

    std::vector<Node> nodes;              ///< Todos os nós (depósito + clientes)
    std::vector<double> distanceMatrix;   ///< Matriz de distâncias (formato linear)
    int depotID;                          ///< ID do nó depósito
    int nodesDimension;                   ///< Número total de nós
    int capacityOfVehicle;                ///< Capacidade máxima dos veículos

    // ========================================================================
    // CONSTRUTORES E DESTRUTOR
    // ========================================================================

    /**
     * @brief Construtor padrão
     *
     * Cria uma instância vazia. Use loadInstance() para carregar dados.
     */
    CVRP();

    /**
     * @brief Construtor com arquivo
     *
     * Carrega a instância diretamente de um arquivo.
     *
     * @param fileName Caminho do arquivo de instância
     * @throw std::runtime_error se arquivo não puder ser carregado
     *
     * @post Instância carregada e validada
     */
    explicit CVRP(const std::string& fileName);

    /**
     * @brief Destrutor
     */
    ~CVRP() = default;

    /**
     * @brief Construtor de cópia
     */
    CVRP(const CVRP& other) = default;

    /**
     * @brief Operador de atribuição
     */
    CVRP& operator=(const CVRP& other) = default;

    /**
     * @brief Construtor de movimento
     */
    CVRP(CVRP&& other) noexcept = default;

    /**
     * @brief Operador de atribuição por movimento
     */
    CVRP& operator=(CVRP&& other) noexcept = default;

    // ========================================================================
    // CARREGAMENTO DE DADOS
    // ========================================================================

    /**
     * @brief Carrega instância de um arquivo
     *
     * Lê arquivo no formato padrão CVRP e popula todas as estruturas de dados.
     *
     * @param fileName Caminho do arquivo
     * @throw std::runtime_error se erro ao ler arquivo
     *
     * @post nodes, distanceMatrix, capacityOfVehicle e depotID são preenchidos
     */
    void loadInstance(const std::string& fileName);

    /**
     * @brief Cria instância manualmente (para testes)
     *
     * @param numNodes Número de nós
     * @param capacity Capacidade dos veículos
     * @param depotID ID do depósito
     */
    void createInstance(int numNodes, int capacity, int depotID);

    // ========================================================================
    // CONSULTAS DA MATRIZ DE DISTÂNCIAS
    // ========================================================================

    /**
     * @brief Obtém distância entre dois nós
     *
     * @param fromNode ID do nó origem
     * @param toNode ID do nó destino
     * @return Distância entre os nós
     *
     * @pre fromNode >= 0 && fromNode < nodesDimension
     * @pre toNode >= 0 && toNode < nodesDimension
     */
    double getDistance(int fromNode, int toNode) const;

    /**
     * @brief Define distância entre dois nós
     *
     * @param fromNode ID do nó origem
     * @param toNode ID do nó destino
     * @param distance Distância a ser definida
     *
     * @pre fromNode >= 0 && fromNode < nodesDimension
     * @pre toNode >= 0 && toNode < nodesDimension
     * @pre distance >= 0
     */
    void setDistance(int fromNode, int toNode, double distance);

    /**
     * @brief Obtém índice na matriz linear para par de nós
     *
     * @param fromNode ID do nó origem
     * @param toNode ID do nó destino
     * @return Índice na matriz linear
     */
    int getMatrixIndex(int fromNode, int toNode) const;

    // ========================================================================
    // CONSULTAS DE NÓDOS
    // ========================================================================

    /**
     * @brief Obtém referência para um nó específico
     *
     * @param nodeID ID do nó
     * @return Referência ao nó
     *
     * @pre nodeID >= 0 && nodeID < nodesDimension
     */
    const Node& getNode(int nodeID) const;

    /**
     * @brief Obtém referência mutável para um nó
     *
     * @param nodeID ID do nó
     * @return Referência mutável ao nó
     */
    Node& getNode(int nodeID);

    /**
     * @brief Obtém o nó depósito
     *
     * @return Referência ao depósito
     */
    const Node& getDepot() const;

    /**
     * @brief Obtém número de clientes (exclui depósito)
     *
     * @return Número de clientes
     */
    int getNumberOfCustomers() const;

    /**
     * @brief Verifica se um ID é o depósito
     *
     * @param nodeID ID a verificar
     * @return true se é o depósito
     */
    bool isDepot(int nodeID) const;

    // ========================================================================
    // VALIDAÇÃO
    // ========================================================================

    /**
     * @brief Valida a consistência da instância
     *
     * Verifica:
     * - Todos os nós têm IDs válidos
     * - Matriz de distâncias tem tamanho correto
     * - Capacidade é positiva
     * - Depósito é válido
     * - Demandas são não-negativas
     *
     * @return true se a instância é válida
     */
    bool validateInstance() const;

    /**
     * @brief Verifica se demandas podem ser atendidas
     *
     * Verifica se nenhum cliente tem demanda maior que a capacidade.
     *
     * @return true se todas as demandas são atendíveis
     */
    bool areDemandsValid() const;

    // ========================================================================
    // INFORMAÇÕES E ESTATÍSTICAS
    // ========================================================================

    /**
     * @brief Calcula demanda total de todos os clientes
     *
     * @return Soma de todas as demandas
     */
    int getTotalDemand() const;

    /**
     * @brief Calcula número mínimo teórico de veículos
     *
     * Baseado na razão entre demanda total e capacidade.
     *
     * @return Limite inferior de veículos necessários
     */
    int getMinimumVehicles() const;

    /**
     * @brief Obtém distância máxima na matriz
     *
     * @return Maior distância entre quaisquer dois nós
     */
    double getMaxDistance() const;

    /**
     * @brief Obtém distância mínima na matriz (exclui zeros)
     *
     * @return Menor distância não-zero
     */
    double getMinDistance() const;

    /**
     * @brief Calcula distância média da matriz
     *
     * @return Média de todas as distâncias
     */
    double getAverageDistance() const;

    /**
     * @brief Obtém estatísticas da instância
     */
    struct Statistics {
        int numNodes;
        int numCustomers;
        int capacity;
        int totalDemand;
        int minVehicles;
        double maxDistance;
        double minDistance;
        double avgDistance;
        int maxDemand;
        int minDemand;
        double avgDemand;
    };

    Statistics getStatistics() const;

    // ========================================================================
    // SAÍDA E FORMATAÇÃO
    // ========================================================================

    /**
     * @brief Imprime informações da instância
     *
     * @param os Stream de saída
     */
    void print(std::ostream& os = std::cout) const;

    /**
     * @brief Obtém representação em string
     *
     * @return String com resumo da instância
     */
    std::string toString() const;

    /**
     * @brief Imprime matriz de distâncias formatada
     *
     * @param os Stream de saída
     */
    void printDistanceMatrix(std::ostream& os = std::cout) const;

    /**
     * @brief Imprime lista de nós com demandas
     *
     * @param os Stream de saída
     */
    void printNodes(std::ostream& os = std::cout) const;

    /**
     * @brief Salva instância em arquivo
     *
     * @param fileName Nome do arquivo
     * @return true se salvo com sucesso
     */
    bool saveToFile(const std::string& fileName) const;

    // ========================================================================
    // OPERADORES
    // ========================================================================

    /**
     * @brief Operador de saída para stream
     */
    friend std::ostream& operator<<(std::ostream& os, const CVRP& cvrp);

private:
    // ========================================================================
    // MÉTODOS AUXILIARES PRIVADOS
    // ========================================================================

    /**
     * @brief Valida índice de nó
     *
     * @param nodeID ID a validar
     * @return true se válido
     */
    bool isValidNodeID(int nodeID) const;

    /**
     * @brief Valida índice da matriz
     *
     * @param index Índice a validar
     * @return true se válido
     */
    bool isValidMatrixIndex(int index) const;

    static constexpr double EPSILON = 1e-9;  ///< Tolerância para comparações
};
