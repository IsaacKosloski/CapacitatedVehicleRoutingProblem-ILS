/**
 * @file Scanner.cpp
 * @brief Implementação completa da classe Scanner
 * @author Isaac (Original), Melhorado em 2025
 * @date 30/01/2025 (Original), Melhorado em Outubro/2025
 */

#include "Scanner.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

// ============================================================================
// CONSTRUTOR
// ============================================================================

Scanner::Scanner(const std::string& fileName)
    : dimensionOfNodes(-1)
    , capacityOfVehicles(-1)
    , depot(-1)
    , fileName(fileName)
    , components()
    , nodes()
    , nodesDistance()
{
    std::cout << "Iniciando leitura do arquivo: " << fileName << std::endl;

    try
    {
        readFile(fileName, components, nodesDistance, nodes);

        // Valida dados carregados
        if (dimensionOfNodes <= 0)
        {
            throw std::runtime_error("Dimensão inválida após leitura: " +
                                   std::to_string(dimensionOfNodes));
        }

        if (capacityOfVehicles <= 0)
        {
            throw std::runtime_error("Capacidade inválida após leitura: " +
                                   std::to_string(capacityOfVehicles));
        }

        std::cout << "✓ Arquivo lido com sucesso!" << std::endl;
        std::cout << "  - Dimensão: " << dimensionOfNodes << std::endl;
        std::cout << "  - Capacidade: " << capacityOfVehicles << std::endl;
        std::cout << "  - Depósito: " << depot << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERRO ao ler arquivo: " << e.what() << std::endl;
        throw;
    }
}

// ============================================================================
// LEITURA DE ARQUIVO
// ============================================================================

void Scanner::readFile(const std::string& fileName,
                      std::vector<Component>& components,
                      std::vector<double>& nodesDistance,
                      std::vector<Node>& nodes)
{
    std::ifstream inputFile(fileName, std::ios::in);

    if (!inputFile.is_open())
    {
        throw std::runtime_error("Não foi possível abrir o arquivo: " + fileName);
    }

    std::string line;
    int lineNumber = 0;

    // ========================================================================
    // FASE 1: Lê cabeçalho (especificações)
    // ========================================================================

    std::cout << "Fase 1: Lendo especificações..." << std::endl;

    while (std::getline(inputFile, line))
    {
        lineNumber++;

        // Remove espaços em branco extras
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        // Processa linha de especificação
        if (specificationPart(line))
        {
            break; // Encontrou NODE_COORD_SECTION
        }
    }

    // Valida que encontramos as especificações necessárias
    if (dimensionOfNodes <= 0)
    {
        throw std::runtime_error("DIMENSION não encontrada ou inválida no arquivo");
    }

    if (capacityOfVehicles <= 0)
    {
        throw std::runtime_error("CAPACITY não encontrada ou inválida no arquivo");
    }

    // Prepara estruturas de dados
    components.clear();
    components.resize(dimensionOfNodes);

    nodesDistance.clear();
    nodesDistance.resize(dimensionOfNodes * dimensionOfNodes, 0.0);

    nodes.clear();
    nodes.reserve(dimensionOfNodes);

    // ========================================================================
    // FASE 2: Lê coordenadas dos nós
    // ========================================================================

    std::cout << "Fase 2: Lendo coordenadas dos nós..." << std::endl;

    int nodesRead = 0;

    while (std::getline(inputFile, line) &&
           line.find("DEMAND_SECTION") == std::string::npos)
    {
        lineNumber++;

        // Remove espaços
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        std::istringstream ssLine(line);
        int ID;
        double x, y;

        if (!(ssLine >> ID >> x >> y))
        {
            std::cerr << "AVISO: Linha " << lineNumber
                     << " mal formatada (coordenadas): " << line << std::endl;
            continue;
        }

        // Valida ID
        if (ID < 1 || ID > dimensionOfNodes)
        {
            std::cerr << "ERRO: ID fora do intervalo na linha " << lineNumber
                     << ": " << ID << " (esperado: 1-" << dimensionOfNodes << ")" << std::endl;
            throw std::runtime_error("ID de nó inválido");
        }

        // Armazena coordenadas (ID é 1-indexed, array é 0-indexed)
        std::vector<double> coords = {x, y};
        components[ID - 1] = Component(coords);
        nodesRead++;
    }

    std::cout << "  ✓ " << nodesRead << " coordenadas lidas" << std::endl;

    if (nodesRead != dimensionOfNodes)
    {
        std::cerr << "AVISO: Número de coordenadas (" << nodesRead
                 << ") difere da dimensão (" << dimensionOfNodes << ")" << std::endl;
    }

    // ========================================================================
    // FASE 3: Calcula matriz de distâncias
    // ========================================================================

    std::cout << "Fase 3: Calculando matriz de distâncias..." << std::endl;

    computeDistanceMatrix();

    // ========================================================================
    // FASE 4: Lê demandas
    // ========================================================================

    std::cout << "Fase 4: Lendo demandas..." << std::endl;

    int demandsRead = 0;

    while (std::getline(inputFile, line) &&
           line.find("DEPOT_SECTION") == std::string::npos)
    {
        lineNumber++;

        // Remove espaços
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        std::istringstream ssLine(line);
        int ID, demand;

        if (!(ssLine >> ID >> demand))
        {
            std::cerr << "AVISO: Linha " << lineNumber
                     << " mal formatada (demandas): " << line << std::endl;
            continue;
        }

        // Valida ID
        if (ID < 1 || ID > dimensionOfNodes)
        {
            std::cerr << "ERRO: ID fora do intervalo na linha " << lineNumber
                     << ": " << ID << std::endl;
            throw std::runtime_error("ID de nó inválido em DEMAND_SECTION");
        }

        // Valida demanda
        if (demand < 0)
        {
            std::cerr << "AVISO: Demanda negativa no nó " << ID
                     << ": " << demand << ". Usando 0." << std::endl;
            demand = 0;
        }

        // Cria nó
        nodes.push_back(Node(ID, demand));
        demandsRead++;
    }

    std::cout << "  ✓ " << demandsRead << " demandas lidas" << std::endl;

    if (demandsRead != dimensionOfNodes)
    {
        std::cerr << "AVISO: Número de demandas (" << demandsRead
                 << ") difere da dimensão (" << dimensionOfNodes << ")" << std::endl;
    }

    // ========================================================================
    // FASE 5: Lê depósito
    // ========================================================================

    std::cout << "Fase 5: Identificando depósito..." << std::endl;

    if (std::getline(inputFile, line))
    {
        lineNumber++;

        // Remove espaços
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        std::istringstream ssLine(line);
        int depotID;

        if (ssLine >> depotID)
        {
            this->depot = depotID;

            // Valida ID do depósito
            if (depotID < 1 || depotID > static_cast<int>(nodes.size()))
            {
                std::cerr << "ERRO: ID de depósito inválido: " << depotID << std::endl;
                throw std::runtime_error("ID de depósito fora do intervalo");
            }

            // Marca o nó como depósito (ID é 1-indexed)
            if (depotID - 1 >= 0 && depotID - 1 < static_cast<int>(nodes.size()))
            {
                nodes[depotID - 1].isDepot = true;
                nodes[depotID - 1].isAvailable = false;

                // Garante que depósito tem demanda zero
                if (nodes[depotID - 1].demand != 0)
                {
                    std::cerr << "AVISO: Depósito tinha demanda não-zero ("
                             << nodes[depotID - 1].demand
                             << "). Ajustando para 0." << std::endl;
                    nodes[depotID - 1].demand = 0;
                }

                std::cout << "  ✓ Depósito identificado: " << depotID << std::endl;
            }
        }
        else
        {
            std::cerr << "AVISO: Não foi possível ler ID do depósito. Usando 1." << std::endl;
            this->depot = 1;

            if (!nodes.empty())
            {
                nodes[0].isDepot = true;
                nodes[0].isAvailable = false;
                nodes[0].demand = 0;
            }
        }
    }

    inputFile.close();

    // ========================================================================
    // VALIDAÇÃO FINAL
    // ========================================================================

    std::cout << "Fase 6: Validação final..." << std::endl;

    // Verifica que temos dados completos
    if (components.size() != static_cast<size_t>(dimensionOfNodes))
    {
        throw std::runtime_error("Número de componentes não corresponde à dimensão");
    }

    if (nodes.size() != static_cast<size_t>(dimensionOfNodes))
    {
        throw std::runtime_error("Número de nós não corresponde à dimensão");
    }

    if (nodesDistance.size() != static_cast<size_t>(dimensionOfNodes * dimensionOfNodes))
    {
        throw std::runtime_error("Tamanho da matriz de distâncias incorreto");
    }

    // Verifica que existe ao menos um depósito
    bool hasDepot = false;
    for (const auto& node : nodes)
    {
        if (node.isDepot)
        {
            hasDepot = true;
            break;
        }
    }

    if (!hasDepot)
    {
        std::cerr << "AVISO: Nenhum nó marcado como depósito. Marcando nó 1." << std::endl;
        if (!nodes.empty())
        {
            nodes[0].isDepot = true;
            nodes[0].isAvailable = false;
            nodes[0].demand = 0;
            this->depot = nodes[0].ID;
        }
    }

    std::cout << "  ✓ Validação concluída com sucesso!" << std::endl;
}

// ============================================================================
// PROCESSAMENTO DE ESPECIFICAÇÕES
// ============================================================================

bool Scanner::specificationPart(const std::string& line)
{
    // Procura por palavras-chave no formato "KEYWORD : VALUE"

    size_t colonPos = line.find(":");

    if (colonPos != std::string::npos)
    {
        // Extrai keyword e valor
        std::string keyword = line.substr(0, colonPos);
        std::string valueStr = line.substr(colonPos + 1);

        // Remove espaços
        keyword.erase(0, keyword.find_first_not_of(" \t"));
        keyword.erase(keyword.find_last_not_of(" \t") + 1);

        valueStr.erase(0, valueStr.find_first_not_of(" \t"));
        valueStr.erase(valueStr.find_last_not_of(" \t") + 1);

        // Processa diferentes keywords
        if (keyword == "DIMENSION")
        {
            try
            {
                dimensionOfNodes = std::stoi(valueStr);
                std::cout << "  ✓ DIMENSION: " << dimensionOfNodes << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cerr << "ERRO ao parsear DIMENSION: " << valueStr << std::endl;
                throw std::runtime_error("DIMENSION inválida");
            }
        }
        else if (keyword == "CAPACITY")
        {
            try
            {
                capacityOfVehicles = std::stoi(valueStr);
                std::cout << "  ✓ CAPACITY: " << capacityOfVehicles << std::endl;
            }
            catch (const std::exception& e)
            {
                std::cerr << "ERRO ao parsear CAPACITY: " << valueStr << std::endl;
                throw std::runtime_error("CAPACITY inválida");
            }
        }
        else if (keyword == "NAME" || keyword == "COMMENT" ||
                 keyword == "TYPE" || keyword == "EDGE_WEIGHT_TYPE")
        {
            // Informações adicionais (apenas para log)
            std::cout << "  - " << keyword << ": " << valueStr << std::endl;
        }
    }

    // Verifica se chegamos à seção de coordenadas
    if (line.find("NODE_COORD_SECTION") != std::string::npos)
    {
        std::cout << "  ✓ Encontrada seção NODE_COORD_SECTION" << std::endl;
        return true;
    }

    return false;
}

// ============================================================================
// CÁLCULO DE DISTÂNCIAS
// ============================================================================

void Scanner::computeDistanceMatrix()
{
    if (components.empty() || dimensionOfNodes <= 0)
    {
        throw std::runtime_error("Componentes não inicializados para calcular distâncias");
    }

    int distancesCalculated = 0;

    for (int row = 0; row < dimensionOfNodes; ++row)
    {
        for (int col = 0; col < dimensionOfNodes; ++col)
        {
            int index = row * dimensionOfNodes + col;

            if (index < 0 || index >= static_cast<int>(nodesDistance.size()))
            {
                std::cerr << "ERRO: Índice de matriz fora do intervalo: " << index << std::endl;
                throw std::runtime_error("Índice de matriz inválido");
            }

            // Calcula distância euclidiana
            if (row == col)
            {
                // Distância de um nó para ele mesmo é zero
                nodesDistance[index] = 0.0;
            }
            else
            {
                // Valida índices de componentes
                if (row < 0 || row >= static_cast<int>(components.size()) ||
                    col < 0 || col >= static_cast<int>(components.size()))
                {
                    std::cerr << "ERRO: Índice de componente inválido: "
                             << row << ", " << col << std::endl;
                    throw std::runtime_error("Índice de componente inválido");
                }

                try
                {
                    double distance = components[row].distanceTo(components[col]);
                    nodesDistance[index] = distance;
                    distancesCalculated++;
                }
                catch (const std::exception& e)
                {
                    std::cerr << "ERRO ao calcular distância entre "
                             << row << " e " << col << ": " << e.what() << std::endl;
                    throw;
                }
            }
        }
    }

    std::cout << "  ✓ " << distancesCalculated << " distâncias calculadas" << std::endl;

    // Valida que não há distâncias negativas ou NaN
    for (size_t i = 0; i < nodesDistance.size(); ++i)
    {
        if (std::isnan(nodesDistance[i]) || std::isinf(nodesDistance[i]))
        {
            std::cerr << "ERRO: Distância inválida (NaN/Inf) no índice " << i << std::endl;
            throw std::runtime_error("Distância inválida encontrada");
        }

        if (nodesDistance[i] < 0)
        {
            std::cerr << "ERRO: Distância negativa no índice " << i
                     << ": " << nodesDistance[i] << std::endl;
            throw std::runtime_error("Distância negativa encontrada");
        }
    }
}