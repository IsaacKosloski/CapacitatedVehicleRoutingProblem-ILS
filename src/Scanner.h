/**
 * @file Scanner.h
 * @brief Classe para leitura de arquivos de instância CVRP
 * @author Isaac (Original), Melhorado em 2025
 * @date 30/01/2025 (Original), Melhorado em Outubro/2025
 */

#pragma once

#include "Component.h"
#include "Node.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @class Scanner
 * @brief Responsável por ler e parsear arquivos de instância CVRP
 *
 * Lê arquivos no formato padrão CVRPLIB e extrai:
 * - Dimensão do problema (DIMENSION)
 * - Capacidade dos veículos (CAPACITY)
 * - Coordenadas dos nós (NODE_COORD_SECTION)
 * - Demandas dos clientes (DEMAND_SECTION)
 * - ID do depósito (DEPOT_SECTION)
 *
 * Calcula automaticamente a matriz de distâncias euclidianas entre todos os nós.
 *
 * @invariant dimensionOfNodes > 0 após construção bem-sucedida
 * @invariant capacityOfVehicles > 0 após construção bem-sucedida
 * @invariant nodesDistance.size() == dimensionOfNodes * dimensionOfNodes
 * @invariant nodes.size() == dimensionOfNodes
 * @invariant components.size() == dimensionOfNodes
 */
class Scanner
{
public:
    // ========================================================================
    // ATRIBUTOS PÚBLICOS
    // ========================================================================

    int dimensionOfNodes;                   ///< Número total de nós
    int capacityOfVehicles;                 ///< Capacidade máxima dos veículos
    int depot;                              ///< ID do nó depósito
    std::string fileName;                   ///< Nome do arquivo lido

    std::vector<Component> components;      ///< Coordenadas (x,y) de cada nó
    std::vector<Node> nodes;                ///< Lista de nós com ID e demanda
    std::vector<double> nodesDistance;      ///< Matriz de distâncias (formato linear)

    // ========================================================================
    // CONSTRUTOR E DESTRUTOR
    // ========================================================================

    /**
     * @brief Construtor que carrega e processa arquivo de instância
     *
     * Lê o arquivo especificado e popula todas as estruturas de dados.
     * O processo inclui:
     * 1. Leitura das especificações (DIMENSION, CAPACITY)
     * 2. Leitura das coordenadas dos nós
     * 3. Cálculo da matriz de distâncias euclidianas
     * 4. Leitura das demandas dos clientes
     * 5. Identificação do depósito
     * 6. Validação final dos dados
     *
     * @param fileName Caminho completo ou relativo do arquivo .vrp
     *
     * @throw std::runtime_error se arquivo não puder ser aberto
     * @throw std::runtime_error se formato do arquivo for inválido
     * @throw std::runtime_error se dados essenciais estiverem faltando
     *
     * @post dimensionOfNodes > 0
     * @post capacityOfVehicles > 0
     * @post depot >= 0 && depot < dimensionOfNodes
     * @post Todas as estruturas de dados estão preenchidas e consistentes
     */
    explicit Scanner(const std::string& fileName);

    /**
     * @brief Destrutor padrão
     */
    ~Scanner() = default;

    /**
     * @brief Construtor de cópia (deletado - use com cuidado devido ao tamanho)
     */
    Scanner(const Scanner& other) = delete;

    /**
     * @brief Operador de atribuição (deletado)
     */
    Scanner& operator=(const Scanner& other) = delete;

    /**
     * @brief Construtor de movimento
     */
    Scanner(Scanner&& other) noexcept = default;

    /**
     * @brief Operador de atribuição por movimento
     */
    Scanner& operator=(Scanner&& other) noexcept = default;

private:
    // ========================================================================
    // ATRIBUTOS PRIVADOS
    // ========================================================================

    int col;  ///< Contador de colunas (usado internamente)
    int row;  ///< Contador de linhas (usado internamente)

    // ========================================================================
    // MÉTODOS PRIVADOS
    // ========================================================================

    /**
     * @brief Lê e processa o arquivo de instância completo
     *
     * Metodo principal que coordenatodo o processo de leitura:
     * - Fase 1: Lê especificações do cabeçalho
     * - Fase 2: Lê coordenadas dos nós
     * - Fase 3: Calcula matriz de distâncias
     * - Fase 4: Lê demandas dos clientes
     * - Fase 5: Identifica depósito
     * - Fase 6: Valida consistência dos dados
     *
     * @param fileName Nome do arquivo a ler
     * @param components Vetor de componentes (coordenadas) a preencher
     * @param nodesDistance Matriz de distâncias a preencher
     * @param nodes Vetor de nós a preencher
     *
     * @throw std::runtime_error em caso de erro de leitura ou formato inválido
     */
    void readFile(const std::string& fileName,
                  std::vector<Component>& components,
                  std::vector<double>& nodesDistance,
                  std::vector<Node>& nodes);

    /**
     * @brief Processa linhas da seção de especificações
     *
     * Identifica e extrai informações das linhas do tipo:
     * - DIMENSION : valor
     * - CAPACITY : valor
     * - TYPE : CVRP
     * - EDGE_WEIGHT_TYPE : ...
     * - etc.
     *
     * @param line Linha a processar
     * @return true se encontrou NODE_COORD_SECTION (fim das especificações)
     * @return false caso contrário
     */
    bool specificationPart(const std::string& line);

    /**
     * @brief Calcula matriz de distâncias euclidianas
     *
     * Para cada par de nós (i, j), calcula:
     * distance[i][j] = sqrt((x_i - x_j)² + (y_i - y_j)²)
     *
     * A matriz é armazenada em formato linear:
     * index = row * dimensionOfNodes + col
     *
     * @throw std::runtime_error se componentes não estiverem inicializados
     * @throw std::runtime_error se encontrar distâncias NaN ou infinitas
     *
     * @post nodesDistance está completamente preenchida
     * @post Todas as distâncias são >= 0
     * @post Diagonal principal contém zeros
     */
    void computeDistanceMatrix();

    /**
     * @brief Remove espaços em branco no início e fim de uma string
     *
     * @param str String a processar (modificada in-place)
     */
    void trim(std::string& str);

    /**
     * @brief Valida se um ID de nó está no intervalo válido
     *
     * @param nodeID ID a validar
     * @return true se nodeID >= 1 && nodeID <= dimensionOfNodes
     */
    bool isValidNodeID(int nodeID) const;

    /**
     * @brief Registra informação de progresso (opcional)
     *
     * @param message Mensagem a exibir
     * @param phase Número da fase atual (1-6)
     */
    void logProgress(const std::string& message, int phase = 0) const;
};
