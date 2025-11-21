/**
 * @file Solver.h
 * @brief Classe principal do solver CVRP usando metaheurística ILS
 * @author Isaac (Original), Melhorado em 2025
 * @date 03/02/2025 (Original), Melhorado em Outubro/2025
 */

#pragma once

#include "CVRP.h"
#include "Solution.h"
#include <random>
#include <memory>
#include <vector>

/**
 * @class Solver
 * @brief Implementa algoritmos de otimização para o CVRP usando ILS
 *
 * Esta classe contém a implementação completa do algoritmo Iterated Local Search (ILS)
 * para resolver o problema CVRP. Inclui:
 * - Métodos de geração de solução inicial (Greedy, Random)
 * - Operadores de busca local (2-Opt, 3-Opt, Or-Opt, SwapStar)
 * - Mecanismos de perturbação (Double Bridge, Random Remove-Insert)
 * - Critérios de aceitação (Best Improvement, Simulated Annealing)
 *
 * @note A classe usa Mersenne Twister para geração de números aleatórios
 *       de alta qualidade, garantindo reprodutibilidade com seeds.
 */
class Solver
{
private:
    // ========================================================================
    // ATRIBUTOS PRIVADOS
    // ========================================================================

    std::mt19937 rng;                                    ///< Gerador Mersenne Twister
    std::uniform_real_distribution<double> dist;         ///< Distribuição uniforme [0,1]

    static constexpr double EPSILON = 1e-9;              ///< Tolerância para comparações

    // ========================================================================
    // MÉTODOS AUXILIARES PRIVADOS
    // ========================================================================

    /**
     * @brief Valida se uma rota respeita a capacidade do veículo
     *
     * @param route Rota a ser validada (sequência de IDs)
     * @param instance Instância do problema
     * @return true se a rota é válida
     */
    bool validateRouteCapacity(const std::vector<int>& route, CVRP* instance) const;

    /**
     * @brief Calcula a carga total de uma rota
     *
     * @param route Rota a ser avaliada
     * @param instance Instância do problema
     * @return Carga total da rota
     */
    int calculateRouteLoad(const std::vector<int>& route, CVRP* instance) const;

    /**
     * @brief Calcula delta de custo para movimento SwapStar
     *
     * Calcula a mudança no custo se uma cadeia de nós for movida de uma
     * posição para outra (mesmo na mesma rota ou entre rotas diferentes).
     *
     * @param instance Instância do problema
     * @param route1 Primeira rota (origem)
     * @param route2 Segunda rota (destino)
     * @param i Posição inicial da cadeia na route1
     * @param j Posição de inserção na route2
     * @param k Tamanho da cadeia
     * @return Delta de custo (negativo = melhoria)
     */
    double calculateSwapStarDelta(CVRP* instance,
                                  const std::vector<int>& route1,
                                  const std::vector<int>& route2,
                                  int i, int j, int k) const;

public:
    // ========================================================================
    // CONSTRUTORES E DESTRUTOR
    // ========================================================================

    /**
     * @brief Construtor do Solver
     *
     * Inicializa o gerador de números aleatórios com a seed fornecida.
     * Se nenhuma seed for fornecida, usa std::random_device para gerar uma.
     *
     * @param seed Semente para o gerador aleatório (padrão: aleatória)
     *
     * @note Use a mesma seed para reproduzir resultados
     */
    explicit Solver(unsigned int seed = std::random_device{}());

    /**
     * @brief Destrutor padrão
     */
    ~Solver() = default;

    /**
     * @brief Construtor de cópia (deletado - evita cópias acidentais)
     */
    Solver(const Solver& other) = delete;

    /**
     * @brief Operador de atribuição (deletado)
     */
    Solver& operator=(const Solver& other) = delete;

    /**
     * @brief Construtor de movimento
     */
    Solver(Solver&& other) noexcept = default;

    /**
     * @brief Operador de atribuição por movimento
     */
    Solver& operator=(Solver&& other) noexcept = default;

    // ========================================================================
    // MÉTODOS DE SOLUÇÃO INICIAL
    // ========================================================================

    /**
     * @brief Gera solução inicial usando heurística gulosa do vizinho mais próximo
     *
     * Algoritmo:
     * 1. Inicia no depósito
     * 2. Sempre seleciona o cliente não visitado mais próximo que respeite capacidade
     * 3. Quando não há mais clientes que cabem, fecha a rota e inicia nova
     *
     * Complexidade: O(n²) onde n é o número de clientes
     *
     * @param instance Ponteiro para instância do problema
     * @param solution Ponteiro para solução a ser preenchida
     *
     * @pre instance != nullptr && solution != nullptr
     * @post solution contém uma solução viável
     * @post Todos os clientes estão visitados exatamente uma vez
     */
    void initialSolution_Greedy(CVRP* instance, Solution* solution);

    /**
     * @brief Gera solução inicial aleatória
     *
     * Útil para diversificação. Embaralha os clientes e constrói rotas
     * sequencialmente respeitando a capacidade.
     *
     * @param instance Ponteiro para instância do problema
     * @param solution Ponteiro para solução a ser preenchida
     *
     * @pre instance != nullptr && solution != nullptr
     * @post solution contém uma solução viável
     */
    void initialSolution_Random(CVRP* instance, Solution* solution);

    // ========================================================================
    // OPERADORES DE BUSCA LOCAL (INTRA-ROTA)
    // ========================================================================

    /**
     * @brief Busca local 2-Opt (intra-rota)
     *
     * Para cada rota, testa todas as possíveis trocas 2-opt:
     * - Remove duas arestas (i,i+1) e (j,j+1)
     * - Adiciona arestas (i,j) e (i+1,j+1)
     * - Reverte o segmento entre i e j
     *
     * Complexidade: O(k * n³) onde k = número de rotas, n = nós por rota
     *
     * @param instance Ponteiro para instância
     * @param solution Solução inicial
     * @param bestSolution Melhor solução encontrada
     *
     * @post bestSolution contém solução melhorada ou igual a solution
     */
    void localSearch_TwoOpt(CVRP* instance, Solution* solution, Solution* bestSolution);

    /**
     * @brief Busca local 3-Opt (intra-rota)
     *
     * Similar ao 2-opt mas com três pontos de corte, testando múltiplas
     * reconfigurações do segmento.
     *
     * Complexidade: O(k * n⁴)
     *
     * @param instance Ponteiro para instância
     * @param solution Solução inicial
     * @param bestSolution Melhor solução encontrada
     *
     * @post bestSolution está otimizada localmente
     */
    void localSearch_ThreeOpt(CVRP* instance, Solution* solution, Solution* bestSolution);

    /**
     * @brief Busca local Or-Opt (intra-rota)
     *
     * Move sequências de 1, 2 ou 3 nós consecutivos para outras posições
     * na mesma rota. Mais rápido que 3-opt e complementa bem o 2-opt.
     *
     * Complexidade: O(k * n³)
     *
     * @param instance Ponteiro para instância
     * @param solution Solução a ser melhorada
     *
     * @post solution está otimizada localmente
     */
    void localSearch_OrOpt(CVRP* instance, Solution* solution);

    // ========================================================================
    // OPERADORES DE BUSCA LOCAL (INTER-ROTA)
    // ========================================================================

    /**
     * @brief Busca local SwapStar (inter-rota e intra-rota)
     *
     * Move cadeias de nós entre rotas diferentes ou dentro da mesma rota.
     * Verifica restrições de capacidade antes de aplicar movimentos.
     *
     * Algoritmo:
     * 1. Para cada par de rotas (incluindo mesma rota)
     * 2. Para cada posição i na rota origem
     * 3. Para cada posição j na rota destino
     * 4. Calcula delta de custo
     * 5. Se melhoria, aplica movimento imediatamente (first improvement)
     *
     * Complexidade: O(k² * n² * c) onde c = tamanho da cadeia
     *
     * @param instance Ponteiro para instância
     * @param solution Solução a ser melhorada
     * @param chainLength Tamanho da cadeia a ser movida (tipicamente 1-3)
     *
     * @pre chainLength >= 1
     * @post solution está otimizada considerando movimentos inter-rota
     */
    void localSearch_SwapStar(CVRP* instance, Solution* solution, int chainLength);

    // ========================================================================
    // MECANISMOS DE PERTURBAÇÃO
    // ========================================================================

    /**
     * @brief Perturbação Double Bridge
     *
     * Para cada rota com tamanho suficiente:
     * 1. Divide a rota em 4 segmentos aleatórios
     * 2. Reconecta os segmentos em ordem diferente: [1][3][2][4]
     *
     * Objetivo: Escapar de ótimos locais mantendo estrutura da solução
     *
     * @param instance Ponteiro para instância
     * @param bestSolution Solução base para perturbação
     * @param perturbedSolution Solução perturbada resultante
     *
     * @post perturbedSolution é uma versão perturbada de bestSolution
     * @post perturbedSolution permanece viável
     */
    void perturbation_DoubleBridge(CVRP* instance,
                                   Solution* bestSolution,
                                   Solution* perturbedSolution);

    /**
     * @brief Perturbação por remoção e reinserção aleatória
     *
     * Mecanismo mais agressivo de perturbação:
     * 1. Remove numNodes nós aleatórios de rotas aleatórias
     * 2. Reinsere em posições aleatórias válidas (respeitando capacidade)
     * 3. Cria novas rotas se necessário
     *
     * Útil quando detectada estagnação do algoritmo.
     *
     * @param instance Ponteiro para instância
     * @param bestSolution Solução base
     * @param perturbedSolution Solução perturbada resultante
     * @param numNodes Número de nós a remover/reinserir
     *
     * @pre numNodes >= 1 && numNodes < número de clientes
     * @post perturbedSolution é significativamente diferente de bestSolution
     */
    void perturbation_RandomRemoveInsert(CVRP* instance,
                                        Solution* bestSolution,
                                        Solution* perturbedSolution,
                                        int numNodes);

    // ========================================================================
    // CRITÉRIOS DE ACEITAÇÃO
    // ========================================================================

    /**
     * @brief Critério de aceitação: apenas melhorias (hill climbing)
     *
     * Aceita a nova solução se e somente se ela for estritamente melhor
     * que a solução atual (menor custo).
     *
     * @param bestSolution Melhor solução atual
     * @param newSolution Nova solução candidata
     * @return true se newSolution foi aceita (é melhor)
     *
     * @post Se retornou true, bestSolution = newSolution
     */
    bool acceptanceCriterion_BestImprovement(Solution* bestSolution,
                                            Solution* newSolution);

    /**
     * @brief Critério de aceitação: Simulated Annealing
     *
     * Aceita melhorias sempre, e aceita pioras com probabilidade
     * decrescente baseada na temperatura:
     *
     * P(accept) = exp(-delta / temperature)
     *
     * onde delta = newCost - currentCost
     *
     * @param bestSolution Melhor solução atual
     * @param newSolution Nova solução candidata
     * @param temperature Temperatura atual (deve ser > 0)
     * @return true se newSolution foi aceita
     *
     * @pre temperature > 0
     * @post Se retornou true, bestSolution = newSolution
     *
     * @note A temperatura deve ser reduzida externamente a cada iteração
     */
    bool acceptanceCriterion_SimulatedAnnealing(Solution* bestSolution,
                                               Solution* newSolution,
                                               double temperature);

    // ========================================================================
    // MÉTODOS AUXILIARES PÚBLICOS
    // ========================================================================

    /**
     * @brief Obtém seed atual do gerador aleatório
     *
     * Útil para debugging e reprodução de resultados.
     *
     * @return Seed usada na inicialização
     */
    unsigned int getCurrentSeed() const;

    /**
     * @brief Define nova seed para o gerador aleatório
     *
     * @param seed Nova seed
     */
    void setSeed(unsigned int seed);

private:
    unsigned int currentSeed;  ///< Seed atual (para referência)
};
