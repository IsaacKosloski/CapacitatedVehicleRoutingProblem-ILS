/**
 * @file Vehicle.h
 * @brief Classe que representa um veículo na frota do CVRP
 * @author Isaac (Original), Melhorado em 2025
 * @date 28/01/2025 (Original), Melhorado em Outubro/2025
 */

#pragma once

#include "Node.h"
#include "Component.h"
#include <vector>
#include <memory>
#include <string>

/**
 * @class Vehicle
 * @brief Representa um veículo da frota que realiza entregas/coletas
 *
 * Cada veículo possui:
 * - Capacidade máxima
 * - Carga atual
 * - Rota (sequência de nós visitados)
 *
 * O veículo deve sempre começar e terminar no depósito e respeitar
 * sua restrição de capacidade.
 *
 * @invariant currentLoad >= 0
 * @invariant currentLoad <= capacity
 * @invariant Se route não-vazia, route[0] == depósito
 * @invariant Se route completa, route.back() == depósito
 */
class Vehicle
{
public:
    // ========================================================================
    // ATRIBUTOS
    // ========================================================================

    int capacity;                ///< Capacidade máxima do veículo
    int currentLoad;             ///< Carga atual do veículo
    std::vector<Node> route;     ///< Sequência de nós visitados

    // ========================================================================
    // CONSTRUTORES E DESTRUTOR
    // ========================================================================

    /**
     * @brief Construtor padrão
     *
     * Cria um veículo com capacidade e carga zero.
     */
    Vehicle();

    /**
     * @brief Construtor parametrizado
     *
     * @param capacity Capacidade máxima do veículo
     * @param currentLoad Carga inicial (padrão: 0)
     *
     * @pre capacity > 0
     * @pre currentLoad >= 0
     * @pre currentLoad <= capacity
     *
     * @post this->capacity == capacity
     * @post this->currentLoad == currentLoad
     */
    Vehicle(int capacity, int currentLoad = 0);

    /**
     * @brief Destrutor padrão
     */
    ~Vehicle() = default;

    /**
     * @brief Construtor de cópia
     */
    Vehicle(const Vehicle& other) = default;

    /**
     * @brief Operador de atribuição
     */
    Vehicle& operator=(const Vehicle& other) = default;

    /**
     * @brief Construtor de movimento
     */
    Vehicle(Vehicle&& other) noexcept = default;

    /**
     * @brief Operador de atribuição por movimento
     */
    Vehicle& operator=(Vehicle&& other) noexcept = default;

    // ========================================================================
    // MÉTODOS DE MANIPULAÇÃO DE ROTA
    // ========================================================================

    /**
     * @brief Adiciona um nó à rota
     *
     * Verifica se a adição respeitaria a capacidade antes de adicionar.
     *
     * @param node Nó a ser adicionado
     * @return true se o nó foi adicionado com sucesso
     *
     * @post Se retornou true, currentLoad foi incrementado
     * @post Se retornou true, route contém o novo nó
     */
    bool addNode(const Node& node);

    /**
     * @brief Tenta adicionar um nó forçadamente (ignora capacidade)
     *
     * Útil para reconstrução de rotas conhecidas válidas.
     * AVISO: Pode violar restrição de capacidade!
     *
     * @param node Nó a ser adicionado
     */
    void forceAddNode(const Node& node);

    /**
     * @brief Remove o último nó da rota (exceto depósito)
     *
     * @return true se um nó foi removido
     *
     * @post currentLoad foi decrementado apropriadamente
     */
    bool removeLastNode();

    /**
     * @brief Remove um nó específico da rota por índice
     *
     * @param index Índice do nó na rota
     * @return true se removido com sucesso
     *
     * @pre index >= 0 && index < route.size()
     * @pre route[index] não é depósito (primeiro/último)
     */
    bool removeNodeAt(int index);

    /**
     * @brief Insere um nó em posição específica
     *
     * @param node Nó a inserir
     * @param position Posição onde inserir (0 = início, size = fim)
     * @return true se inserido com sucesso
     *
     * @pre position >= 0 && position <= route.size()
     */
    bool insertNodeAt(const Node& node, int position);

    /**
     * @brief Limpa a rota e reseta a carga
     *
     * @post route.empty() == true
     * @post currentLoad == 0
     */
    void reset();

    /**
     * @brief Reseta apenas a carga (mantém a rota)
     *
     * Útil para recalcular carga após modificações na rota.
     *
     * @post currentLoad == 0
     */
    void resetLoad();

    // ========================================================================
    // MÉTODOS DE CONSULTA
    // ========================================================================

    /**
     * @brief Verifica se o veículo pode acomodar uma demanda adicional
     *
     * @param additionalDemand Demanda a verificar
     * @return true se há capacidade disponível
     */
    bool canAccommodate(int additionalDemand) const;

    /**
     * @brief Obtém capacidade disponível restante
     *
     * @return capacity - currentLoad
     */
    int getAvailableCapacity() const;

    /**
     * @brief Verifica se a rota está vazia
     *
     * @return true se não há nós na rota
     */
    bool isEmpty() const;

    /**
     * @brief Obtém número de nós na rota
     *
     * @return Tamanho da rota
     */
    int getRouteSize() const;

    /**
     * @brief Obtém número de clientes na rota (exclui depósitos)
     *
     * @return Número de clientes visitados
     */
    int getNumberOfCustomers() const;

    /**
     * @brief Verifica se a rota é válida
     *
     * Uma rota válida:
     * - Respeita a capacidade
     * - Começa no depósito (se não-vazia)
     * - Termina no depósito (se completa)
     *
     * @return true se a rota é válida
     */
    bool isRouteValid() const;

    /**
     * @brief Verifica se a rota está completa (fechada)
     *
     * Uma rota completa começa e termina no depósito.
     *
     * @return true se a rota está fechada
     */
    bool isRouteClosed() const;

    // ========================================================================
    // CÁLCULO DE CUSTO
    // ========================================================================

    /**
     * @brief Calcula o custo da rota
     *
     * Soma as distâncias entre nós consecutivos na rota.
     *
     * @param distanceMatrix Matriz de distâncias (formato: row*dimension + col)
     * @param dimension Dimensão da matriz (número de nós)
     * @return Custo total da rota
     *
     * @pre distanceMatrix.size() == dimension * dimension
     */
    double computeCost(const std::vector<double>& distanceMatrix, int dimension) const;

    /**
     * @brief Calcula custo da rota usando vetor de componentes
     *
     * Calcula distâncias euclidianas diretamente das coordenadas.
     *
     * @param components Vetor de componentes (coordenadas dos nós)
     * @return Custo total da rota
     *
     * @pre components.size() >= maior ID na rota
     */
    double computeCostFromComponents(const std::vector<Component>& components) const;

    // ========================================================================
    // RECALCULAR ESTADO
    // ========================================================================

    /**
     * @brief Recalcula a carga atual baseada na rota
     *
     * Útil após modificações manuais na rota.
     * Soma as demandas de todos os nós (exceto depósito).
     *
     * @post currentLoad == soma das demandas dos nós na rota
     */
    void recalculateLoad();

    // ========================================================================
    // UTILIDADES
    // ========================================================================

    /**
     * @brief Obtém representação em string do veículo
     *
     * @return String descrevendo o estado do veículo
     */
    std::string toString() const;

    /**
     * @brief Obtém representação da rota em string
     *
     * @return String com IDs dos nós na rota
     */
    std::string routeToString() const;

    /**
     * @brief Calcula taxa de utilização do veículo
     *
     * @return Porcentagem de capacidade utilizada (0.0 a 1.0)
     */
    double getUtilizationRate() const;

    // ========================================================================
    // OPERADORES
    // ========================================================================

    /**
     * @brief Operador de saída para stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Vehicle& vehicle);
};
