/**
 * @file main_exe.cpp
 * @brief Programa principal para execuções múltiplas do solver CVRP
 * @author Isaac (Original), Melhorado em 2025
 * @date 30/01/2025 (Original), Melhorado em Outubro/2025
 *
 * Este programa executa o solver CVRP múltiplas vezes na mesma instância,
 * permitindo análise estatística dos resultados. Cada execução usa uma
 * seed diferente para garantir diversidade nas soluções.
 */

#include "Functions.h"
#include "Solver.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

// Verificação de C++11 para std::filesystem
#if __cplusplus >= 201703L
    #include <filesystem>
    namespace fs = std::filesystem;
    #define HAS_FILESYSTEM
#elif defined(_MSC_VER)
    #include <direct.h>
    #define mkdir_platform(dir) _mkdir(dir)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define mkdir_platform(dir) mkdir(dir, 0755)
#endif

// ============================================================================
// CONSTANTES DE CONFIGURAÇÃO
// ============================================================================

#define MAX_ITERATIONS 10000    ///< Número de iterações do ILS por execução
#define EXECUTIONS 30          ///< Número de execuções independentes

// ============================================================================
// ESTRUTURA PARA ESTATÍSTICAS
// ============================================================================

/**
 * @struct ExecutionStats
 * @brief Armazena estatísticas de uma execução
 */
struct ExecutionStats
{
    int executionNumber;
    double cost;
    double time;
    int routes;
    int iterations;

    ExecutionStats()
        : executionNumber(0), cost(0.0), time(0.0), routes(0), iterations(0)
    {}

    ExecutionStats(int num, double c, double t, int r, int i)
        : executionNumber(num), cost(c), time(t), routes(r), iterations(i)
    {}
};

// ============================================================================
// FUNÇÃO PARA CRIAR DIRETÓRIOS
// ============================================================================

/**
 * @brief Cria diretório (e subdiretórios necessários)
 * @param path Caminho do diretório
 * @return true se criado com sucesso ou já existe
 */
bool createDirectory(const std::string& path)
{
#ifdef HAS_FILESYSTEM
    try
    {
        fs::create_directories(path);
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erro ao criar diretório " << path << ": " << e.what() << std::endl;
        return false;
    }
#else
    // Implementação simples sem filesystem
    // Cria apenas diretório final (não recursivo)
    int result = mkdir_platform(path.c_str());
    return result == 0 || errno == EEXIST;
#endif
}

// ============================================================================
// FUNÇÃO PARA FORMATAR NOME DO ARQUIVO
// ============================================================================

/**
 * @brief Gera nome do arquivo de saída formatado
 * @param set Nome do conjunto (ex: "SetA")
 * @param dimension Dimensão da instância (ex: "32")
 * @param executionNumber Número da execução (0-29)
 * @return Caminho completo do arquivo
 */
std::string generateOutputFileName(const std::string& set,
                                   const std::string& dimension,
                                   int executionNumber)
{
    std::ostringstream oss;
    oss << "../Output/" << set << "/" << dimension << "/"
        << dimension << "-" << std::setfill('0') << std::setw(2)
        << executionNumber << ".sol";
    return oss.str();
}

// ============================================================================
// FUNÇÃO PARA IMPRIMIR ESTATÍSTICAS FINAIS
// ============================================================================

/**
 * @brief Calcula e imprime estatísticas de todas as execuções
 * @param stats Vetor com estatísticas de cada execução
 */
void printFinalStatistics(const std::vector<ExecutionStats>& stats)
{
    if (stats.empty())
    {
        std::cout << "\nNenhuma estatística disponível." << std::endl;
        return;
    }

    CVRPUtils::printHeader("ESTATÍSTICAS FINAIS DAS EXECUÇÕES");

    // Calcula estatísticas
    std::vector<double> costs;
    std::vector<double> times;
    std::vector<int> routeCounts;

    for (const auto& s : stats)
    {
        costs.push_back(s.cost);
        times.push_back(s.time);
        routeCounts.push_back(s.routes);
    }

    // Custos
    double minCost = *std::min_element(costs.begin(), costs.end());
    double maxCost = *std::max_element(costs.begin(), costs.end());
    double avgCost = std::accumulate(costs.begin(), costs.end(), 0.0) / costs.size();

    // Desvio padrão dos custos
    double sumSquares = 0.0;
    for (double cost : costs)
    {
        double diff = cost - avgCost;
        sumSquares += diff * diff;
    }
    double stdDevCost = std::sqrt(sumSquares / (costs.size() - 1));

    // Tempos
    double minTime = *std::min_element(times.begin(), times.end());
    double maxTime = *std::max_element(times.begin(), times.end());
    double avgTime = std::accumulate(times.begin(), times.end(), 0.0) / times.size();

    // Rotas
    int minRoutes = *std::min_element(routeCounts.begin(), routeCounts.end());
    int maxRoutes = *std::max_element(routeCounts.begin(), routeCounts.end());
    double avgRoutes = std::accumulate(routeCounts.begin(), routeCounts.end(), 0.0) / routeCounts.size();

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "\n=== CUSTOS ===" << std::endl;
    std::cout << "Melhor:        " << minCost << std::endl;
    std::cout << "Pior:          " << maxCost << std::endl;
    std::cout << "Média:         " << avgCost << std::endl;
    std::cout << "Desvio padrão: " << stdDevCost << std::endl;
    std::cout << "Amplitude:     " << (maxCost - minCost) << std::endl;

    std::cout << "\n=== TEMPOS ===" << std::endl;
    std::cout << "Mais rápido:   " << std::setprecision(4) << minTime << "s" << std::endl;
    std::cout << "Mais lento:    " << maxTime << "s" << std::endl;
    std::cout << "Média:         " << avgTime << "s" << std::endl;
    std::cout << "Tempo total:   " << std::accumulate(times.begin(), times.end(), 0.0) << "s" << std::endl;

    std::cout << "\n=== ROTAS ===" << std::endl;
    std::cout << "Mínimo:        " << minRoutes << std::endl;
    std::cout << "Máximo:        " << maxRoutes << std::endl;
    std::cout << "Média:         " << std::setprecision(2) << avgRoutes << std::endl;

    std::cout << "\n=== QUALIDADE ===" << std::endl;
    double gap = ((maxCost - minCost) / minCost) * 100.0;
    std::cout << "Gap (pior vs melhor): " << gap << "%" << std::endl;

    CVRPUtils::printSeparator();
}

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================

int main(int argc, char** argv)
{
    // ========================================================================
    // VALIDAÇÃO DE ARGUMENTOS
    // ========================================================================

    if (argc < 4)
    {
        std::cerr << "\n╔═══════════════════════════════════════════════════════╗" << std::endl;
        std::cerr << "║        CVRP Solver - Execuções Múltiplas            ║" << std::endl;
        std::cerr << "╚═══════════════════════════════════════════════════════╝\n" << std::endl;
        std::cerr << "Uso: " << argv[0] << " <arquivo_instancia> <dimensao> <conjunto>\n" << std::endl;
        std::cerr << "Exemplo:" << std::endl;
        std::cerr << "  " << argv[0] << " instances/A-n32-k5.vrp 32 SetA" << std::endl;
        std::cerr << "\nParâmetros:" << std::endl;
        std::cerr << "  <arquivo_instancia> : Caminho do arquivo .vrp" << std::endl;
        std::cerr << "  <dimensao>          : Número de nós (ex: 32)" << std::endl;
        std::cerr << "  <conjunto>          : Nome do conjunto (ex: SetA, SetB)" << std::endl;
        std::cerr << "\nConfiguração:" << std::endl;
        std::cerr << "  - Execuções: " << EXECUTIONS << std::endl;
        std::cerr << "  - Iterações por execução: " << MAX_ITERATIONS << std::endl;
        return 1;
    }

    std::string instanceFile = argv[1];
    std::string dimension = argv[2];
    std::string setName = argv[3];

    // ========================================================================
    // BANNER INICIAL
    // ========================================================================

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         CVRP SOLVER - EXECUÇÕES MÚLTIPLAS                ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\nInstância:  " << instanceFile << std::endl;
    std::cout << "Dimensão:   " << dimension << std::endl;
    std::cout << "Conjunto:   " << setName << std::endl;
    std::cout << "Execuções:  " << EXECUTIONS << std::endl;
    std::cout << "Iterações:  " << MAX_ITERATIONS << " por execução" << std::endl;
    CVRPUtils::printSeparator();

    // ========================================================================
    // CRIA ESTRUTURA DE DIRETÓRIOS
    // ========================================================================

    std::string outputDir = "../Output/" + setName + "/" + dimension;
    std::cout << "\nCriando diretório de saída: " << outputDir << std::endl;

    if (!createDirectory(outputDir))
    {
        std::cerr << "AVISO: Não foi possível criar diretório. Continuando..." << std::endl;
    }

    // ========================================================================
    // CARREGA INSTÂNCIA
    // ========================================================================

    std::cout << "\nCarregando instância..." << std::endl;

    std::unique_ptr<CVRP> cvrp;

    try
    {
        cvrp = std::make_unique<CVRP>(instanceFile);

        std::cout << "✓ Instância carregada com sucesso!" << std::endl;
        CVRPUtils::printInstanceSummary(cvrp.get());
    }
    catch (const std::exception& e)
    {
        std::cerr << "\n✗ ERRO ao carregar instância: " << e.what() << std::endl;
        return 1;
    }

    CVRPUtils::printSeparator();

    // ========================================================================
    // LOOP DE EXECUÇÕES
    // ========================================================================

    std::vector<ExecutionStats> allStats;
    allStats.reserve(EXECUTIONS);

    std::cout << "\nIniciando execuções...\n" << std::endl;

    auto globalStartTime = std::chrono::high_resolution_clock::now();

    for (int execution = 0; execution < EXECUTIONS; ++execution)
    {
        // Cria objetos para esta execução
        unsigned int seed = execution * 1000; // Seed diferente para cada execução
        auto solver = std::make_unique<Solver>(seed);
        auto solution = std::make_unique<Solution>();
        auto bestSolution = std::make_unique<Solution>();

        std::cout << "═══ Execução " << std::setw(2) << (execution + 1)
                  << "/" << EXECUTIONS << " ═══" << std::flush;

        // Inicia cronômetro
        auto startTime = std::chrono::high_resolution_clock::now();

        try
        {
            // Solução inicial
            solver->initialSolution_Greedy(cvrp.get(), solution.get());

            // Busca local inicial
            solver->localSearch_TwoOpt(cvrp.get(), solution.get(), bestSolution.get());

            // Loop ILS
            for (int i = 0; i < MAX_ITERATIONS; ++i)
            {
                solver->perturbation_DoubleBridge(cvrp.get(), bestSolution.get(), solution.get());
                solver->localSearch_TwoOpt(cvrp.get(), solution.get(), solution.get());
                solver->acceptanceCriterion_BestImprovement(bestSolution.get(), solution.get());
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "\n✗ ERRO na execução " << (execution + 1)
                     << ": " << e.what() << std::endl;
            continue;
        }

        // Para cronômetro
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;

        // Gera nome do arquivo
        std::string outputFile = generateOutputFileName(setName, dimension, execution);

        // Salva solução
        bool saved = bestSolution->printSolution(outputFile.c_str(),
                                                 elapsed.count(),
                                                 MAX_ITERATIONS,
                                                 cvrp->nodesDimension);

        if (!saved)
        {
            std::cerr << " - ERRO ao salvar!" << std::endl;
        }

        // Armazena estatísticas
        ExecutionStats stats(execution + 1,
                           bestSolution->totalCost,
                           elapsed.count(),
                           bestSolution->fleetSize,
                           MAX_ITERATIONS);
        allStats.push_back(stats);

        // Imprime progresso
        std::cout << " | Custo: " << std::fixed << std::setprecision(2)
                  << bestSolution->totalCost
                  << " | Rotas: " << bestSolution->fleetSize
                  << " | Tempo: " << std::setprecision(3) << elapsed.count() << "s";

        if (saved)
        {
            std::cout << " ✓" << std::endl;
        }
        else
        {
            std::cout << " ✗" << std::endl;
        }
    }

    auto globalEndTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalElapsed = globalEndTime - globalStartTime;

    // ========================================================================
    // IMPRIME ESTATÍSTICAS FINAIS
    // ========================================================================

    std::cout << "\n";
    printFinalStatistics(allStats);

    std::cout << "\nTempo total de execução: "
              << CVRPUtils::formatTime(totalElapsed.count()) << std::endl;

    std::cout << "\n✓ Todas as execuções concluídas com sucesso!" << std::endl;
    std::cout << "  Arquivos salvos em: " << outputDir << "/" << std::endl;

    CVRPUtils::printSeparator();

    return 0;
}