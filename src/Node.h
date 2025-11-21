/**
 * @file Node.h
 * @brief Classe que representa um nó (cliente ou depósito) no problema CVRP
 * @author Isaac (Original), Melhorado em 2025
 * @date 28/01/2025 (Original), Melhorado em Outubro/2025
 */

#pragma once

#include <iostream>
#include <string>

/**
 * @class Node
 * @brief Representa um cliente ou o depósito no problema CVRP
 *
 * Cada nó possui um identificador único, uma demanda (quantidade a ser
 * coletada/entregue) e flags indicando se é o depósito e se está disponível
 * para ser visitado.
 *
 * @invariant ID >= 0
 * @invariant demand >= 0
 * @invariant Se isDepot == true, então demand == 0
 * @invariant Se isDepot == true, então isAvailable == false
 */
class Node
{
public:
    // ========================================================================
    // ATRIBUTOS
    // ========================================================================

    int  ID;                  ///< Identificador único do nó (0 = depósito típico)
    int  demand;              ///< Demanda do nó (0 para depósito)
    bool isDepot;             ///< true se o nó é o depósito
    bool isAvailable;         ///< true se o nó está disponível para visitação

    // ========================================================================
    // CONSTRUTORES E DESTRUTOR
    // ========================================================================

    /**
     * @brief Construtor padrão
     *
     * Inicializa um nó vazio com valores padrão:
     * - ID = -1 (inválido)
     * - demand = 0
     * - isDepot = false
     * - isAvailable = false
     */
    Node();

    /**
     * @brief Construtor parametrizado
     *
     * Cria um nó com ID e demanda especificados.
     * Automaticamente determina se é depósito baseado na demanda:
     * - Se demand == 0: isDepot = true, isAvailable = false
     * - Se demand > 0: isDepot = false, isAvailable = true
     *
     * @param ID Identificador único do nó
     * @param demand Demanda do nó (0 para depósito)
     *
     * @pre ID >= 0
     * @pre demand >= 0
     * @post Se demand == 0, então isDepot == true
     */
    Node(int ID, int demand);

    /**
     * @brief Destrutor padrão
     */
    ~Node() = default;

    /**
     * @brief Construtor de cópia (padrão)
     */
    Node(const Node& other) = default;

    /**
     * @brief Operador de atribuição (padrão)
     */
    Node& operator=(const Node& other) = default;

    // ========================================================================
    // MÉTODOS PÚBLICOS
    // ========================================================================

    /**
     * @brief Verifica se o nó é válido
     *
     * Um nó é válido se:
     * - ID >= 0
     * - demand >= 0
     * - Se isDepot, então demand == 0
     *
     * @return true se o nó é válido
     */
    bool isValid() const;

    /**
     * @brief Marca o nó como visitado (indisponível)
     *
     * @pre !isDepot (depósito não pode ser marcado como visitado)
     * @post isAvailable == false
     */
    void markAsVisited();

    /**
     * @brief Marca o nó como não visitado (disponível)
     *
     * @pre !isDepot (depósito sempre permanece indisponível)
     * @post isAvailable == true
     */
    void markAsAvailable();

    /**
     * @brief Reseta o nó para estado inicial disponível
     *
     * Torna o nó disponível novamente, útil para reiniciar
     * algoritmos que marcam nós como visitados.
     *
     * @note Depósitos permanecem indisponíveis
     */
    void reset();

    /**
     * @brief Obtém uma representação em string do nó
     *
     * @return String no formato "Node[ID=X, demand=Y, depot=Z]"
     */
    std::string toString() const;

    // ========================================================================
    // OPERADORES DE COMPARAÇÃO
    // ========================================================================

    /**
     * @brief Operador de igualdade
     *
     * Dois nós são iguais se possuem o mesmo ID.
     *
     * @param other Outro nó para comparar
     * @return true se os IDs são iguais
     */
    bool operator==(const Node& other) const;

    /**
     * @brief Operador de desigualdade
     *
     * @param other Outro nó para comparar
     * @return true se os IDs são diferentes
     */
    bool operator!=(const Node& other) const;

    /**
     * @brief Operador de menor que (para ordenação)
     *
     * Ordena por ID.
     *
     * @param other Outro nó para comparar
     * @return true se este ID é menor que o outro
     */
    bool operator<(const Node& other) const;

    // ========================================================================
    // OPERADORES DE STREAM
    // ========================================================================

    /**
     * @brief Operador de saída para stream
     *
     * Permite usar Node com std::cout e outros streams.
     * Formato: "Node[ID=X, demand=Y, depot=Z, available=W]"
     *
     * @param os Stream de saída
     * @param node Nó a ser impresso
     * @return Referência ao stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Node& node);
};
