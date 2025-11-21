/**
 * @file Component.h
 * @brief Classe que armazena coordenadas espaciais de um nó
 * @author Isaac (Original), Melhorado em 2025
 * @date 30/01/2025 (Original), Melhorado em Outubro/2025
 */

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

/**
 * @class Component
 * @brief Representa as coordenadas espaciais de um nó no problema CVRP
 *
 * Esta classe armazena as componentes de posição (tipicamente x, y) de um nó,
 * permitindo o cálculo de distâncias euclidianas entre nós.
 *
 * @note Por padrão, trabalha com coordenadas 2D (x, y), mas suporta dimensões
 *       arbitrárias através do vetor de componentes.
 *
 * @invariant positionComponents.size() >= 2 (mínimo 2D)
 */
class Component
{
public:
    // ========================================================================
    // ATRIBUTOS
    // ========================================================================

    int numberOfComponents;                  ///< Número de dimensões (2 = 2D, 3 = 3D, etc)
    std::vector<double> positionComponents;  ///< Coordenadas do nó (x, y, z, ...)

    // ========================================================================
    // CONSTRUTORES E DESTRUTOR
    // ========================================================================

    /**
     * @brief Construtor padrão
     *
     * Inicializa um componente vazio.
     * @post numberOfComponents == 0
     * @post positionComponents.empty() == true
     */
    Component();

    /**
     * @brief Construtor com vetor de posições
     *
     * Cria um componente com coordenadas especificadas.
     *
     * @param positionComponents Vetor com coordenadas (x, y, ...)
     *
     * @pre positionComponents.size() >= 2
     * @post this->numberOfComponents == positionComponents.size()
     */
    explicit Component(const std::vector<double>& positionComponents);

    /**
     * @brief Construtor para coordenadas 2D
     *
     * Conveniência para criar componente 2D diretamente.
     *
     * @param x Coordenada X
     * @param y Coordenada Y
     *
     * @post numberOfComponents == 2
     */
    Component(double x, double y);

    /**
     * @brief Construtor para coordenadas 3D
     *
     * Conveniência para criar componente 3D.
     *
     * @param x Coordenada X
     * @param y Coordenada Y
     * @param z Coordenada Z
     *
     * @post numberOfComponents == 3
     */
    Component(double x, double y, double z);

    /**
     * @brief Destrutor padrão
     */
    ~Component() = default;

    /**
     * @brief Construtor de cópia (padrão)
     */
    Component(const Component& other) = default;

    /**
     * @brief Operador de atribuição (padrão)
     */
    Component& operator=(const Component& other) = default;

    /**
     * @brief Construtor de movimento (padrão)
     */
    Component(Component&& other) noexcept = default;

    /**
     * @brief Operador de atribuição por movimento (padrão)
     */
    Component& operator=(Component&& other) noexcept = default;

    // ========================================================================
    // MÉTODOS PÚBLICOS
    // ========================================================================

    /**
     * @brief Obtém o vetor de posições
     *
     * @return Cópia do vetor de coordenadas
     */
    std::vector<double> getPosition() const;

    /**
     * @brief Obtém uma coordenada específica
     *
     * @param index Índice da coordenada (0 = x, 1 = y, 2 = z, ...)
     * @return Valor da coordenada
     *
     * @pre index >= 0 && index < numberOfComponents
     * @throw std::out_of_range se índice inválido
     */
    double getCoordinate(int index) const;

    /**
     * @brief Define uma coordenada específica
     *
     * @param index Índice da coordenada
     * @param value Novo valor
     *
     * @pre index >= 0 && index < numberOfComponents
     * @throw std::out_of_range se índice inválido
     */
    void setCoordinate(int index, double value);

    /**
     * @brief Obtém coordenada X (primeira dimensão)
     *
     * @return Valor de X
     * @pre numberOfComponents >= 1
     */
    double getX() const;

    /**
     * @brief Obtém coordenada Y (segunda dimensão)
     *
     * @return Valor de Y
     * @pre numberOfComponents >= 2
     */
    double getY() const;

    /**
     * @brief Obtém coordenada Z (terceira dimensão)
     *
     * @return Valor de Z
     * @pre numberOfComponents >= 3
     */
    double getZ() const;

    /**
     * @brief Define coordenada X
     *
     * @param x Novo valor de X
     * @pre numberOfComponents >= 1
     */
    void setX(double x);

    /**
     * @brief Define coordenada Y
     *
     * @param y Novo valor de Y
     * @pre numberOfComponents >= 2
     */
    void setY(double y);

    /**
     * @brief Define coordenada Z
     *
     * @param z Novo valor de Z
     * @pre numberOfComponents >= 3
     */
    void setZ(double z);

    /**
     * @brief Calcula distância euclidiana para outro componente
     *
     * Distância euclidiana n-dimensional:
     * d = sqrt(sum((xi - yi)^2))
     *
     * @param other Outro componente
     * @return Distância euclidiana
     *
     * @pre this->numberOfComponents == other.numberOfComponents
     */
    double distanceTo(const Component& other) const;

    /**
     * @brief Calcula distância Manhattan para outro componente
     *
     * Distância Manhattan (city block):
     * d = sum(|xi - yi|)
     *
     * @param other Outro componente
     * @return Distância Manhattan
     *
     * @pre this->numberOfComponents == other.numberOfComponents
     */
    double manhattanDistanceTo(const Component& other) const;

    /**
     * @brief Verifica se o componente é válido
     *
     * Um componente é válido se possui ao menos 2 coordenadas.
     *
     * @return true se válido
     */
    bool isValid() const;

    /**
     * @brief Obtém representação em string
     *
     * @return String no formato "(x, y)" ou "(x, y, z)" etc
     */
    std::string toString() const;

    /**
     * @brief Normaliza o vetor de posição
     *
     * Transforma o vetor em um vetor unitário (magnitude = 1).
     * Útil para cálculos de direção.
     *
     * @return Componente normalizado
     * @throw std::runtime_error se magnitude é zero
     */
    Component normalize() const;

    /**
     * @brief Calcula a magnitude (norma) do vetor
     *
     * @return ||v|| = sqrt(x^2 + y^2 + z^2 + ...)
     */
    double magnitude() const;

    // ========================================================================
    // OPERADORES
    // ========================================================================

    /**
     * @brief Operador de igualdade
     *
     * @param other Outro componente
     * @return true se todas as coordenadas são iguais (com tolerância)
     */
    bool operator==(const Component& other) const;

    /**
     * @brief Operador de desigualdade
     */
    bool operator!=(const Component& other) const;

    /**
     * @brief Operador de adição (soma vetorial)
     */
    Component operator+(const Component& other) const;

    /**
     * @brief Operador de subtração (diferença vetorial)
     */
    Component operator-(const Component& other) const;

    /**
     * @brief Operador de multiplicação por escalar
     */
    Component operator*(double scalar) const;

    /**
     * @brief Operador de divisão por escalar
     */
    Component operator/(double scalar) const;

    /**
     * @brief Operador de acesso por índice (leitura)
     */
    double operator[](int index) const;

    /**
     * @brief Operador de acesso por índice (escrita)
     */
    double& operator[](int index);

    /**
     * @brief Operador de saída para stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Component& comp);

private:
    // ========================================================================
    // CONSTANTES PRIVADAS
    // ========================================================================

    static constexpr double EPSILON = 1e-9;  ///< Tolerância para comparações
};
