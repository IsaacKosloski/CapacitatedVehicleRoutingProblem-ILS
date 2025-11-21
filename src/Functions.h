/**
 * @file Functions.h
 * @brief Funções utilitárias para impressão e validação
 * @author Isaac (Original), Melhorado em 2025
 * @date 03/02/2025 (Original), Melhorado em Outubro/2025
 */

#pragma once

#include "CVRP.h"
#include "Solution.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

/**
 * @namespace CVRPUtils
 * @brief Funções utilitárias para o solver CVRP
 */
namespace CVRPUtils
{
    // ========================================================================
    // IMPRESSÃO DE INSTÂNCIA
    // ========================================================================

    /**
     * @brief Imprime informações da instância CVRP
     *
     * Exibe:
     * - Dimensão e capacidade
     * - Matriz de distâncias
     * - Lista de nós com demandas
     * - Estatísticas gerais
     *
     * @param cvrp Ponteiro para instância CVRP
     * @param showDistanceMatrix Se true, imprime matriz completa
     * @param showNodes Se true, imprime lista de nós
     */
    void printInstance(const CVRP* cvrp,
                      bool showDistanceMatrix = false,
                      bool showNodes = true);

    /**
     * @brief Imprime apenas resumo da instância
     *
     * @param cvrp Ponteiro para instância CVRP
     */
    void printInstanceSummary(const CVRP* cvrp);

    /**
     * @brief Imprime matriz de distâncias formatada
     *
     * @param cvrp Ponteiro para instância CVRP
     * @param maxNodesToShow Número máximo de nós a mostrar (0 = todos)
     */
    void printDistanceMatrix(const CVRP* cvrp, int maxNodesToShow = 20);

    /**
     * @brief Imprime lista de nós com demandas
     *
     * @param cvrp Ponteiro para instância CVRP
     */
    void printNodesList(const CVRP* cvrp);

    // ========================================================================
    // IMPRESSÃO DE SOLUÇÃO
    // ========================================================================

    /**
     * @brief Imprime solução completa no console
     *
     * Exibe:
     * - Todas as rotas
     * - Custo total e por rota
     * - Cargas das rotas
     * - Estatísticas
     *
     * @param solution Ponteiro para solução
     * @param detailed Se true, mostra informações detalhadas
     */
    void printSolution(const Solution* solution, bool detailed = true);

    /**
     * @brief Imprime apenas resumo da solução
     *
     * @param solution Ponteiro para solução
     */
    void printSolutionSummary(const Solution* solution);

    /**
     * @brief Imprime rotas formatadas
     *
     * @param solution Ponteiro para solução
     * @param showCosts Se true, mostra custos individuais
     * @param showLoads Se true, mostra cargas individuais
     */
    void printRoutes(const Solution* solution,
                    bool showCosts = true,
                    bool showLoads = true);

    // ========================================================================
    // ESTATÍSTICAS E ANÁLISE
    // ========================================================================

    /**
     * @brief Imprime estatísticas detalhadas da solução
     *
     * @param solution Ponteiro para solução
     * @param cvrp Ponteiro para instância (para validação adicional)
     */
    void printStatistics(const Solution* solution, const CVRP* cvrp = nullptr);

    /**
     * @brief Imprime análise de qualidade da solução
     *
     * Compara com limite inferior teórico e mostra gaps.
     *
     * @param solution Ponteiro para solução
     * @param cvrp Ponteiro para instância
     * @param bestKnownCost Melhor custo conhecido (opcional)
     */
    void printQualityAnalysis(const Solution* solution,
                             const CVRP* cvrp,
                             double bestKnownCost = -1.0);

    /**
     * @brief Calcula e imprime limites inferiores
     *
     * @param cvrp Ponteiro para instância
     */
    void printLowerBounds(const CVRP* cvrp);

    // ========================================================================
    // VALIDAÇÃO
    // ========================================================================

    /**
     * @brief Valida e imprime resultado da validação
     *
     * @param solution Ponteiro para solução
     * @param cvrp Ponteiro para instância
     * @return true se solução é válida
     */
    bool validateAndPrint(const Solution* solution, const CVRP* cvrp);

    /**
     * @brief Verifica e imprime violações de capacidade
     *
     * @param solution Ponteiro para solução
     * @param cvrp Ponteiro para instância
     * @return Número de violações encontradas
     */
    int checkCapacityViolations(const Solution* solution, const CVRP* cvrp);

    /**
     * @brief Verifica e imprime clientes não visitados
     *
     * @param solution Ponteiro para solução
     * @param cvrp Ponteiro para instância
     * @return IDs dos clientes não visitados
     */
    std::vector<int> checkUnvisitedCustomers(const Solution* solution,
                                             const CVRP* cvrp);

    // ========================================================================
    // COMPARAÇÃO DE SOLUÇÕES
    // ========================================================================

    /**
     * @brief Compara duas soluções e imprime diferenças
     *
     * @param solution1 Primeira solução
     * @param solution2 Segunda solução
     * @param label1 Rótulo da primeira solução
     * @param label2 Rótulo da segunda solução
     */
    void compareSolutions(const Solution* solution1,
                         const Solution* solution2,
                         const std::string& label1 = "Solução 1",
                         const std::string& label2 = "Solução 2");

    /**
     * @brief Calcula e imprime gap entre soluções
     *
     * @param currentCost Custo atual
     * @param referenceCost Custo de referência
     * @param referenceLabel Rótulo da referência
     */
    void printGap(double currentCost,
                 double referenceCost,
                 const std::string& referenceLabel = "Referência");

    // ========================================================================
    // FORMATAÇÃO E UTILITÁRIOS
    // ========================================================================

    /**
     * @brief Imprime linha separadora
     *
     * @param length Comprimento da linha
     * @param character Caractere a usar
     */
    void printSeparator(int length = 80, char character = '=');

    /**
     * @brief Imprime cabeçalho formatado
     *
     * @param title Título do cabeçalho
     * @param length Largura total
     */
    void printHeader(const std::string& title, int length = 80);

    /**
     * @brief Formata tempo em string legível
     *
     * @param seconds Tempo em segundos
     * @return String formatada (ex: "2h 15min 30.5s")
     */
    std::string formatTime(double seconds);

    /**
     * @brief Formata número com separador de milhares
     *
     * @param value Valor a formatar
     * @param decimals Número de casas decimais
     * @return String formatada
     */
    std::string formatNumber(double value, int decimals = 2);

    /**
     * @brief Formata porcentagem
     *
     * @param value Valor (0.0 a 1.0)
     * @param decimals Casas decimais
     * @return String formatada (ex: "85.3%")
     */
    std::string formatPercentage(double value, int decimals = 1);

    // ========================================================================
    // EXPORTAÇÃO
    // ========================================================================

    /**
     * @brief Exporta solução para formato CSV
     *
     * @param solution Ponteiro para solução
     * @param fileName Nome do arquivo CSV
     * @return true se exportado com sucesso
     */
    bool exportToCSV(const Solution* solution, const std::string& fileName);

    /**
     * @brief Exporta rotas para arquivo de texto simples
     *
     * @param solution Ponteiro para solução
     * @param fileName Nome do arquivo
     * @return true se exportado com sucesso
     */
    bool exportRoutesToText(const Solution* solution, const std::string& fileName);

    /**
     * @brief Exporta estatísticas para JSON
     *
     * @param solution Ponteiro para solução
     * @param cvrp Ponteiro para instância
     * @param fileName Nome do arquivo JSON
     * @return true se exportado com sucesso
     */
    bool exportStatisticsToJSON(const Solution* solution,
                               const CVRP* cvrp,
                               const std::string& fileName);

    // ========================================================================
    // VISUALIZAÇÃO (Dados para Plotagem)
    // ========================================================================

    /**
     * @brief Gera arquivo de dados para visualização gráfica
     *
     * Formato compatível com gnuplot ou matplotlib.
     *
     * @param solution Ponteiro para solução
     * @param cvrp Ponteiro para instância
     * @param fileName Nome do arquivo de saída
     * @return true se gerado com sucesso
     */
    bool generatePlotData(const Solution* solution,
                         const CVRP* cvrp,
                         const std::string& fileName);

    /**
     * @brief Gera script gnuplot para visualização
     *
     * @param dataFileName Nome do arquivo de dados
     * @param outputImageFile Nome da imagem de saída
     * @param scriptFileName Nome do script gnuplot
     * @return true se gerado com sucesso
     */
    bool generateGnuplotScript(const std::string& dataFileName,
                              const std::string& outputImageFile,
                              const std::string& scriptFileName = "plot.gp");

} // namespace CVRPUtils

// Aliases para compatibilidade com código original
using CVRPUtils::printInstance;
using CVRPUtils::printSolution;
