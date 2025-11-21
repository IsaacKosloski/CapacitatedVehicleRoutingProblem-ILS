/**
 * @file Node.cpp
 * @brief Implementação da classe Node
 * @author Isaac (Original), Melhorado em 2025
 */

#include "Node.h"
#include <sstream>

// ============================================================================
// CONSTRUTORES
// ============================================================================

Node::Node()
    : ID(-1)
    , demand(0)
    , isDepot(false)
    , isAvailable(false)
{
    // Construtor padrão - inicializa nó inválido
}

Node::Node(int ID, int demand)
    : ID(ID)
    , demand(demand)
    , isDepot(false)
    , isAvailable(true)
{
    // Validação de entrada
    if (ID < 0)
    {
        std::cerr << "AVISO: ID negativo (" << ID
                  << ") definido para nó. Usando 0." << std::endl;
        this->ID = 0;
    }

    if (demand < 0)
    {
        std::cerr << "AVISO: Demanda negativa (" << demand
                  << ") definida para nó " << this->ID
                  << ". Usando 0." << std::endl;
        this->demand = 0;
        demand = 0; // Para a lógica abaixo
    }

    // Determina automaticamente se é depósito baseado na demanda
    if (demand == 0)
    {
        this->isDepot = true;
        this->isAvailable = false; // Depósito não é "visitável"
    }
    else
    {
        this->isDepot = false;
        this->isAvailable = true;  // Cliente disponível para visitação
    }
}

// ============================================================================
// MÉTODOS PÚBLICOS
// ============================================================================

bool Node::isValid() const
{
    // Verifica invariantes da classe
    bool valid = true;

    if (ID < 0)
    {
        std::cerr << "Nó inválido: ID negativo (" << ID << ")" << std::endl;
        valid = false;
    }

    if (demand < 0)
    {
        std::cerr << "Nó inválido: Demanda negativa no nó " << ID
                  << " (" << demand << ")" << std::endl;
        valid = false;
    }

    if (isDepot && demand != 0)
    {
        std::cerr << "Nó inválido: Depósito (ID=" << ID
                  << ") com demanda não-zero (" << demand << ")" << std::endl;
        valid = false;
    }

    if (isDepot && isAvailable)
    {
        std::cerr << "Nó inválido: Depósito (ID=" << ID
                  << ") marcado como disponível" << std::endl;
        valid = false;
    }

    return valid;
}

void Node::markAsVisited()
{
    if (isDepot)
    {
        std::cerr << "AVISO: Tentativa de marcar depósito (ID=" << ID
                  << ") como visitado. Ignorando." << std::endl;
        return;
    }

    isAvailable = false;
}

void Node::markAsAvailable()
{
    if (isDepot)
    {
        std::cerr << "AVISO: Tentativa de marcar depósito (ID=" << ID
                  << ") como disponível. Ignorando." << std::endl;
        return;
    }

    isAvailable = true;
}

void Node::reset()
{
    // Reseta disponibilidade para estado inicial
    if (!isDepot)
    {
        isAvailable = true;
    }
    // Depósito permanece indisponível
}

std::string Node::toString() const
{
    std::ostringstream oss;
    oss << "Node[ID=" << ID
        << ", demand=" << demand
        << ", depot=" << (isDepot ? "true" : "false")
        << ", available=" << (isAvailable ? "true" : "false")
        << "]";
    return oss.str();
}

// ============================================================================
// OPERADORES DE COMPARAÇÃO
// ============================================================================

bool Node::operator==(const Node& other) const
{
    // Dois nós são iguais se possuem o mesmo ID
    return this->ID == other.ID;
}

bool Node::operator!=(const Node& other) const
{
    return !(*this == other);
}

bool Node::operator<(const Node& other) const
{
    // Ordena por ID (útil para ordenação e busca)
    return this->ID < other.ID;
}

// ============================================================================
// OPERADORES DE STREAM
// ============================================================================

std::ostream& operator<<(std::ostream& os, const Node& node)
{
    os << node.toString();
    return os;
}