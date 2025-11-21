/**
 * @file Component.cpp
 * @brief Implementação da classe Component
 * @author Isaac (Original), Melhorado em 2025
 */

#include "Component.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <numeric>

// ============================================================================
// CONSTRUTORES
// ============================================================================

Component::Component()
    : numberOfComponents(0)
    , positionComponents()
{
    // Construtor padrão - componente vazio
}

Component::Component(const std::vector<double>& positionComponents)
    : numberOfComponents(static_cast<int>(positionComponents.size()))
    , positionComponents(positionComponents)
{
    if (positionComponents.size() < 2)
    {
        std::cerr << "AVISO: Component criado com menos de 2 dimensões. "
                  << "Esperado 2D ou superior." << std::endl;
    }
}

Component::Component(double x, double y)
    : numberOfComponents(2)
    , positionComponents{x, y}
{
    // Construtor 2D conveniente
}

Component::Component(double x, double y, double z)
    : numberOfComponents(3)
    , positionComponents{x, y, z}
{
    // Construtor 3D conveniente
}

// ============================================================================
// GETTERS E SETTERS
// ============================================================================

std::vector<double> Component::getPosition() const
{
    return positionComponents;
}

double Component::getCoordinate(int index) const
{
    if (index < 0 || index >= numberOfComponents)
    {
        throw std::out_of_range(
            "Índice de coordenada fora do intervalo: " + std::to_string(index) +
            " (tamanho: " + std::to_string(numberOfComponents) + ")"
        );
    }
    return positionComponents[index];
}

void Component::setCoordinate(int index, double value)
{
    if (index < 0 || index >= numberOfComponents)
    {
        throw std::out_of_range(
            "Índice de coordenada fora do intervalo: " + std::to_string(index)
        );
    }
    positionComponents[index] = value;
}

double Component::getX() const
{
    if (numberOfComponents < 1)
    {
        throw std::runtime_error("Component não possui coordenada X");
    }
    return positionComponents[0];
}

double Component::getY() const
{
    if (numberOfComponents < 2)
    {
        throw std::runtime_error("Component não possui coordenada Y");
    }
    return positionComponents[1];
}

double Component::getZ() const
{
    if (numberOfComponents < 3)
    {
        throw std::runtime_error("Component não possui coordenada Z");
    }
    return positionComponents[2];
}

void Component::setX(double x)
{
    if (numberOfComponents < 1)
    {
        throw std::runtime_error("Component não possui coordenada X");
    }
    positionComponents[0] = x;
}

void Component::setY(double y)
{
    if (numberOfComponents < 2)
    {
        throw std::runtime_error("Component não possui coordenada Y");
    }
    positionComponents[1] = y;
}

void Component::setZ(double z)
{
    if (numberOfComponents < 3)
    {
        throw std::runtime_error("Component não possui coordenada Z");
    }
    positionComponents[2] = z;
}

// ============================================================================
// CÁLCULOS DE DISTÂNCIA
// ============================================================================

double Component::distanceTo(const Component& other) const
{
    if (this->numberOfComponents != other.numberOfComponents)
    {
        std::cerr << "AVISO: Calculando distância entre componentes de "
                  << "dimensões diferentes (" << this->numberOfComponents
                  << " vs " << other.numberOfComponents << ")" << std::endl;
    }

    int minDim = std::min(this->numberOfComponents, other.numberOfComponents);
    double sumSquares = 0.0;

    for (int i = 0; i < minDim; ++i)
    {
        double diff = this->positionComponents[i] - other.positionComponents[i];
        sumSquares += diff * diff;
    }

    return std::sqrt(sumSquares);
}

double Component::manhattanDistanceTo(const Component& other) const
{
    if (this->numberOfComponents != other.numberOfComponents)
    {
        std::cerr << "AVISO: Calculando distância Manhattan entre componentes "
                  << "de dimensões diferentes" << std::endl;
    }

    int minDim = std::min(this->numberOfComponents, other.numberOfComponents);
    double sum = 0.0;

    for (int i = 0; i < minDim; ++i)
    {
        sum += std::abs(this->positionComponents[i] - other.positionComponents[i]);
    }

    return sum;
}

// ============================================================================
// MÉTODOS DE VALIDAÇÃO E UTILIDADE
// ============================================================================

bool Component::isValid() const
{
    // Um componente é válido se possui ao menos 2 coordenadas
    if (numberOfComponents < 2)
    {
        return false;
    }

    // Verifica se não há valores NaN ou infinitos
    for (double coord : positionComponents)
    {
        if (std::isnan(coord) || std::isinf(coord))
        {
            return false;
        }
    }

    return true;
}

std::string Component::toString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "(";

    for (int i = 0; i < numberOfComponents; ++i)
    {
        oss << positionComponents[i];
        if (i < numberOfComponents - 1)
        {
            oss << ", ";
        }
    }

    oss << ")";
    return oss.str();
}

double Component::magnitude() const
{
    double sumSquares = 0.0;
    for (double coord : positionComponents)
    {
        sumSquares += coord * coord;
    }
    return std::sqrt(sumSquares);
}

Component Component::normalize() const
{
    double mag = magnitude();

    if (mag < EPSILON)
    {
        throw std::runtime_error(
            "Não é possível normalizar vetor com magnitude zero ou próxima de zero"
        );
    }

    std::vector<double> normalized;
    normalized.reserve(positionComponents.size());

    for (double coord : positionComponents)
    {
        normalized.push_back(coord / mag);
    }

    return Component(normalized);
}

// ============================================================================
// OPERADORES DE COMPARAÇÃO
// ============================================================================

bool Component::operator==(const Component& other) const
{
    if (this->numberOfComponents != other.numberOfComponents)
    {
        return false;
    }

    for (int i = 0; i < numberOfComponents; ++i)
    {
        if (std::abs(this->positionComponents[i] - other.positionComponents[i]) > EPSILON)
        {
            return false;
        }
    }

    return true;
}

bool Component::operator!=(const Component& other) const
{
    return !(*this == other);
}

// ============================================================================
// OPERADORES ARITMÉTICOS
// ============================================================================

Component Component::operator+(const Component& other) const
{
    if (this->numberOfComponents != other.numberOfComponents)
    {
        throw std::runtime_error(
            "Não é possível somar componentes de dimensões diferentes"
        );
    }

    std::vector<double> result;
    result.reserve(numberOfComponents);

    for (int i = 0; i < numberOfComponents; ++i)
    {
        result.push_back(this->positionComponents[i] + other.positionComponents[i]);
    }

    return Component(result);
}

Component Component::operator-(const Component& other) const
{
    if (this->numberOfComponents != other.numberOfComponents)
    {
        throw std::runtime_error(
            "Não é possível subtrair componentes de dimensões diferentes"
        );
    }

    std::vector<double> result;
    result.reserve(numberOfComponents);

    for (int i = 0; i < numberOfComponents; ++i)
    {
        result.push_back(this->positionComponents[i] - other.positionComponents[i]);
    }

    return Component(result);
}

Component Component::operator*(double scalar) const
{
    std::vector<double> result;
    result.reserve(numberOfComponents);

    for (double coord : positionComponents)
    {
        result.push_back(coord * scalar);
    }

    return Component(result);
}

Component Component::operator/(double scalar) const
{
    if (std::abs(scalar) < EPSILON)
    {
        throw std::runtime_error("Divisão por zero ou valor muito próximo de zero");
    }

    std::vector<double> result;
    result.reserve(numberOfComponents);

    for (double coord : positionComponents)
    {
        result.push_back(coord / scalar);
    }

    return Component(result);
}

// ============================================================================
// OPERADORES DE ACESSO
// ============================================================================

double Component::operator[](int index) const
{
    if (index < 0 || index >= numberOfComponents)
    {
        throw std::out_of_range("Índice fora do intervalo: " + std::to_string(index));
    }
    return positionComponents[index];
}

double& Component::operator[](int index)
{
    if (index < 0 || index >= numberOfComponents)
    {
        throw std::out_of_range("Índice fora do intervalo: " + std::to_string(index));
    }
    return positionComponents[index];
}

// ============================================================================
// OPERADOR DE STREAM
// ============================================================================

std::ostream& operator<<(std::ostream& os, const Component& comp)
{
    os << comp.toString();
    return os;
}