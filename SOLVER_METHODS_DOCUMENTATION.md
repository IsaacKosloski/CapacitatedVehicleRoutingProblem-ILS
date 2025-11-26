# 📚 Documentação dos Métodos - Solver.cpp

> **Autor:** Isaac Kosloski
> **Data de Criação:** 03/02/2025
> **Última Atualização:** 26/11/2025
> **Versão:** 1.0

---

## 📋 Índice

1. [Visão Geral](#visão-geral)
2. [Construtor](#construtor)
3. [Métodos de Solução Inicial](#métodos-de-solução-inicial)
4. [Métodos de Busca Local](#métodos-de-busca-local)
5. [Métodos de Perturbação](#métodos-de-perturbação)
6. [Métodos de Aceitação](#métodos-de-aceitação)
7. [Métodos Auxiliares](#métodos-auxiliares)

---

## 🔍 Visão Geral

A classe **Solver** é o núcleo do algoritmo ILS (Iterated Local Search) para o CVRP. Ela contém todos os métodos necessários para:
- Construir soluções iniciais factíveis
- Realizar buscas locais para otimização
- Perturbar soluções para escapar de ótimos locais
- Avaliar e aceitar novas soluções

**Localização:** `src/Solver.cpp`
**Dependências:** `Solution.h`, `CVRP.h`, `<limits>`

---

## 🏗️ Construtor

### `Solver::Solver()`

**Assinatura:**
```cpp
Solver::Solver()
```

**Descrição de Alto Nível:**
Construtor padrão da classe Solver. Inicializa uma instância do resolvedor sem realizar nenhuma operação específica. A configuração real do solver ocorre quando os métodos são invocados com instâncias de CVRP e Solution.

**Parâmetros:** Nenhum

**Retorno:** Instância do objeto Solver

**Complexidade:** O(1)

**Uso:**
```cpp
auto *solver = new Solver();
```

---

## 🎯 Métodos de Solução Inicial

### `initialSolution_Greedy()`

**Assinatura:**
```cpp
void initialSolution_Greedy(CVRP *instance, Solution *initialSolution)
```

**Descrição de Alto Nível:**
Constrói uma solução inicial factível usando uma heurística gulosa (greedy). O algoritmo inicia no depósito e, iterativamente, adiciona o cliente não visitado mais próximo que ainda cabe na capacidade do veículo atual. Quando a capacidade é excedida, um novo veículo é iniciado. Este método garante uma solução de boa qualidade inicial que serve como ponto de partida para o ILS.

**Estratégia:**
1. Marca todos os clientes como não visitados
2. Inicia uma rota no depósito
3. Seleciona iterativamente o cliente não visitado mais próximo do último cliente na rota
4. Verifica se a capacidade permite adicionar o cliente
5. Se sim, adiciona à rota atual
6. Se não, fecha a rota atual (retorna ao depósito) e inicia nova rota
7. Repete até que todos os clientes sejam visitados

**Parâmetros:**
- `instance`: Ponteiro para a instância do problema CVRP contendo todos os dados
- `initialSolution`: Ponteiro para o objeto Solution que será preenchido com a solução inicial

**Efeitos Colaterais:**
- Modifica `initialSolution->routes` com as rotas geradas
- Atualiza `initialSolution->totalCost` com o custo total
- Define `initialSolution->fleetSize` com o número de veículos utilizados
- Preenche `initialSolution->routeLoads` com as cargas de cada rota

**Complexidade:** O(n²) onde n é o número de clientes

**Qualidade da Solução:** ⭐⭐⭐ (Boa para médias instâncias)

**Quando Usar:**
- Como solução inicial padrão para o ILS
- Para instâncias de pequeno a médio porte (até 200 clientes)
- Quando se deseja uma solução rápida e razoável

**Exemplo de Uso:**
```cpp
auto *cvrp = new CVRP("Benchmarks/A/A-n32-k5.vrp");
auto *solution = new Solution();
solver->initialSolution_Greedy(cvrp, solution);
// solution agora contém uma solução inicial factível
```

---

## 🔍 Métodos de Busca Local

### `localSearch_TwoOpt()`

**Assinatura:**
```cpp
void localSearch_TwoOpt(CVRP *instance, Solution *initialSolution, Solution *bestSolution)
```

**Descrição de Alto Nível:**
Implementa o algoritmo de busca local 2-Opt, um dos métodos mais eficientes para otimização de rotas. O algoritmo tenta melhorar cada rota removendo dois arcos e reconectando-os de forma diferente (reversão de segmento). O processo continua iterativamente até que nenhuma melhoria seja possível, alcançando um ótimo local.

**Estratégia:**
1. Copia a solução inicial como melhor solução atual
2. Para cada rota na solução:
   - Considera todos os pares de arcos (i, j) onde j > i
   - Calcula o custo atual dos arcos A→B e C→D
   - Calcula o custo novo se reverter o segmento: A→C e B→D
   - Se o novo custo for menor, realiza a reversão
3. Repete o processo até que nenhuma melhoria seja encontrada

**Parâmetros:**
- `instance`: Ponteiro para a instância CVRP
- `initialSolution`: Ponteiro para a solução de entrada
- `bestSolution`: Ponteiro para armazenar a solução otimizada

**Efeitos Colaterais:**
- Modifica `bestSolution` com a solução otimizada
- Atualiza o custo total da solução

**Complexidade:** O(n² × k) onde n é o tamanho médio das rotas e k é o número de iterações até convergência

**Melhoria Típica:** 10-30% de redução no custo total

**Características:**
- ✅ Rápido e eficiente
- ✅ Garante ótimo local
- ✅ Funciona bem em rotas individuais
- ⚠️ Não realiza trocas entre rotas diferentes

**Quando Usar:**
- Após construir solução inicial
- Após cada perturbação no loop ILS
- Como principal operador de intensificação

**Exemplo de Uso:**
```cpp
solver->initialSolution_Greedy(cvrp, solution);
solver->localSearch_TwoOpt(cvrp, solution, bestSolution);
// bestSolution agora contém uma versão otimizada de solution
```

---

### `localSearch_ThreeOpt()`

**Assinatura:**
```cpp
void localSearch_ThreeOpt(CVRP *instance, Solution *initialSolution, Solution *bestSolution)
```

**Descrição de Alto Nível:**
Extensão mais poderosa do 2-Opt que considera três pontos de quebra simultâneos em uma rota. Remove três arcos e testa diferentes formas de reconexão. É mais lento que o 2-Opt, mas pode encontrar melhores soluções ao explorar vizinhanças maiores. Particularmente útil para rotas complexas onde o 2-Opt fica preso em ótimos locais ruins.

**Estratégia:**
1. Para cada tripla de posições (i, j, k) em uma rota
2. Gera 5 possíveis reconfigurações:
   - Rota original (baseline)
   - Reverter segmento (i, j)
   - Reverter segmento (j, k)
   - Reverter ambos os segmentos (i, j) e (j, k)
   - Reverter segmento completo (i, k)
3. Avalia o custo de cada configuração
4. Aceita a melhor configuração se houver melhoria
5. Repete até convergência

**Parâmetros:**
- `instance`: Ponteiro para a instância CVRP
- `initialSolution`: Ponteiro para a solução de entrada
- `bestSolution`: Ponteiro para armazenar a solução otimizada

**Efeitos Colaterais:**
- Modifica `bestSolution` com a solução otimizada
- Recalcula custos múltiplas vezes durante o processo

**Complexidade:** O(n³ × k) onde n é o tamanho da rota e k é o número de iterações

**Melhoria Típica:** 5-15% adicional sobre 2-Opt (mas lento)

**Trade-offs:**
- ✅ Explora vizinhança maior que 2-Opt
- ✅ Pode escapar de ótimos locais onde 2-Opt fica preso
- ❌ Significativamente mais lento (n³ vs n²)
- ❌ Alto custo computacional para rotas longas

**Quando Usar:**
- Em instâncias pequenas (< 50 clientes)
- Quando 2-Opt não está dando bons resultados
- Como refinamento final de soluções promissoras
- Quando o tempo de execução não é crítico

**Recomendação:**
⚠️ Use apenas ocasionalmente, pois a complexidade cúbica pode tornar o algoritmo muito lento. Considere usar 2-Opt como padrão e reservar 3-Opt para refinamento final.

---

### `localSearch_SwapStar()`

**Assinatura:**
```cpp
void localSearch_SwapStar(CVRP *instance, Solution *solution, int chain_length)
```

**Descrição de Alto Nível:**
Operador sofisticado de busca local que move sequências (cadeias) de clientes entre rotas diferentes ou dentro da mesma rota. Ao contrário do 2-Opt que trabalha em rotas individuais, o Swap Star permite colaboração entre rotas, potencialmente redistribuindo cargas de forma mais eficiente. O tamanho da cadeia é configurável, permitindo ajustar o equilíbrio entre exploração e tempo de execução.

**Estratégia:**
1. Para cada par de rotas (r1, r2):
   - Considera todas as cadeias de tamanho `chain_length` em r1
   - Para cada posição de inserção em r2:
     - Verifica viabilidade de capacidade
     - Calcula delta de custo (remoção vs. inserção)
     - Se delta < 0, executa o movimento imediatamente
2. Suporta movimentos intra-rota (r1 == r2) e inter-rota (r1 != r2)
3. Atualiza cargas das rotas após movimentos inter-rota
4. Repete até que nenhuma melhoria seja possível

**Parâmetros:**
- `instance`: Ponteiro para a instância CVRP
- `solution`: Ponteiro para a solução a ser otimizada (modificada in-place)
- `chain_length`: Tamanho da sequência de clientes a ser movida (geralmente 1-3)

**Efeitos Colaterais:**
- Modifica `solution->routes` diretamente
- Atualiza `solution->routeLoads` após movimentos
- Atualiza `solution->totalCost` incrementalmente

**Complexidade:** O(n² × m² × k) onde n é o tamanho médio das rotas, m é o número de rotas, e k é o tamanho da cadeia

**Características Especiais:**
- ✅ Único operador que realiza trocas entre rotas diferentes
- ✅ Pode balancear cargas entre veículos
- ✅ Aceita movimentos imediatamente (first improvement)
- ✅ Verifica viabilidade de capacidade antes de calcular custo
- ⚠️ Complexidade aumenta rapidamente com chain_length

**Quando Usar:**
- Após 2-Opt ter convergido
- Quando se deseja balanceamento de carga entre rotas
- Em instâncias com múltiplos veículos (fleet > 3)
- Com chain_length = 1 para maior rapidez, ou 2-3 para maior exploração

**Recomendações:**
- **chain_length = 1**: Movimentos rápidos e frequentes (padrão)
- **chain_length = 2**: Equilíbrio entre qualidade e velocidade
- **chain_length = 3**: Máxima exploração (lento)

**Exemplo de Uso:**
```cpp
// Movimento de clientes individuais (rápido)
solver->localSearch_SwapStar(cvrp, solution, 1);

// Movimento de pares de clientes (médio)
solver->localSearch_SwapStar(cvrp, solution, 2);
```

---

## 🔄 Métodos de Perturbação

### `pertubation_DoubleBridge()`

**Assinatura:**
```cpp
void pertubation_DoubleBridge(CVRP *instance, Solution *bestSolution, Solution *perturbedSolution)
```

**Descrição de Alto Nível:**
Implementa a perturbação Double-Bridge, uma técnica elegante que divide uma rota em quatro segmentos e os reconecta em ordem diferente. Esta perturbação é especialmente eficaz porque gera uma modificação estrutural significativa (escapa de ótimos locais) mas mantém muitas características da boa solução original. É a perturbação padrão no algoritmo Lin-Kernighan e é fundamental para o sucesso do ILS.

**Estratégia:**
1. Seleciona uma rota (atualmente a primeira rota por simplicidade)
2. Verifica se a rota tem pelo menos 8 nós (necessário para 4 segmentos)
3. Divide a rota em 4 segmentos aproximadamente iguais:
   - Segmento A: [0, split1)
   - Segmento B: [split1, split2)
   - Segmento C: [split2, split3)
   - Segmento D: [split3, split4)
   - Segmento E: [split4, fim)
4. Reconecta na ordem: A → C → B → D → E
5. Recalcula o custo da solução perturbada

**Parâmetros:**
- `instance`: Ponteiro para a instância CVRP
- `bestSolution`: Ponteiro para a melhor solução atual (entrada)
- `perturbedSolution`: Ponteiro para armazenar a solução perturbada (saída)

**Efeitos Colaterais:**
- Copia `bestSolution` para `perturbedSolution`
- Modifica a primeira rota de `perturbedSolution`
- Recalcula o custo total

**Complexidade:** O(n) onde n é o tamanho da rota

**Características:**
- ✅ Perturbação controlada (não aleatória demais)
- ✅ Muito rápida (linear)
- ✅ Mantém estrutura parcial da solução original
- ✅ Escapa eficientemente de ótimos locais
- ⚠️ Atualmente apenas perturba a primeira rota

**Intensidade da Perturbação:** Média-Alta

**Quando Usar:**
- Como perturbação padrão no loop ILS
- Após busca local ter convergido
- A cada iteração do algoritmo

**Limitação Atual:**
⚠️ O código atual apenas perturba `routes[0]`. Para melhor desempenho, considere:
- Perturbar rota escolhida aleatoriamente
- Perturbar múltiplas rotas
- Perturbar a rota mais longa

**Sugestão de Melhoria:**
```cpp
// Em vez de:
vector<int> &route = perturbedSolution->routes[0];

// Considere:
int randomRoute = rand() % perturbedSolution->routes.size();
vector<int> &route = perturbedSolution->routes[randomRoute];
```

**Exemplo de Uso:**
```cpp
// No loop ILS
for(int i = 0; i < MAX_ITERATIONS; i++)
{
    solver->pertubation_DoubleBridge(cvrp, bestSolution, solution);
    solver->localSearch_TwoOpt(cvrp, solution, solution);
    solver->acceptanceCriterion_BestSolution(bestSolution, solution);
}
```

---

## ✅ Métodos de Aceitação

### `acceptanceCriterion_BestSolution()`

**Assinatura:**
```cpp
void acceptanceCriterion_BestSolution(Solution *bestSolution, Solution *newSolution)
```

**Descrição de Alto Nível:**
Implementa o critério de aceitação mais rigoroso: aceita apenas soluções estritamente melhores. Esta é uma estratégia elitista que garante que a melhor solução só melhore ao longo das iterações. É simples, eficiente e funciona bem quando combinado com perturbações fortes (como Double-Bridge) que garantem diversificação suficiente.

**Estratégia:**
1. Compara `newSolution->totalCost` com `bestSolution->totalCost`
2. Se `newSolution` tem custo menor:
   - Copia completamente `newSolution` para `bestSolution`
   - (Opcionalmente imprime mensagem de melhoria)
3. Caso contrário:
   - Não faz nada (mantém `bestSolution` inalterada)

**Parâmetros:**
- `bestSolution`: Ponteiro para a melhor solução conhecida (atualizada se nova for melhor)
- `newSolution`: Ponteiro para a nova solução candidata

**Efeitos Colaterais:**
- Pode modificar `bestSolution` se `newSolution` for melhor
- Nenhum efeito se `newSolution` não for melhor

**Complexidade:** O(1) para comparação + O(n × m) para cópia (onde n é o número de rotas e m é o tamanho médio)

**Características:**
- ✅ Garantia de não-degradação (best solution nunca piora)
- ✅ Simplicidade e clareza
- ✅ Convergência monotônica
- ❌ Pode ser muito restritivo em paisagens multimodais
- ❌ Depende fortemente da perturbação para diversificação

**Critérios de Aceitação Alternativos:**

| Critério | Quando Aceita | Características |
|----------|---------------|-----------------|
| **Best Only** (atual) | Custo < Melhor | Elitista, convergência garantida |
| **Better or Equal** | Custo ≤ Melhor | Permite plateaus |
| **Threshold** | Custo < Melhor + δ | Aceita soluções próximas |
| **Simulated Annealing** | Probabilístico | Aceita pioras com probabilidade decrescente |
| **Late Acceptance** | Comparação com histórico | Memória de soluções passadas |

**Quando Usar:**
- Como critério padrão em ILS clássico
- Quando se tem perturbação forte
- Em problemas onde qualquer aceitação de piora é indesejada

**Possíveis Extensões:**
```cpp
// Aceitação com threshold
if (newSolution->totalCost < bestSolution->totalCost + threshold) {
    *bestSolution = *newSolution;
}

// Simulated Annealing
double delta = newSolution->totalCost - bestSolution->totalCost;
if (delta < 0 || exp(-delta / temperature) > random(0,1)) {
    *bestSolution = *newSolution;
}
```

**Exemplo de Uso:**
```cpp
Solution *candidate = new Solution();
solver->pertubation_DoubleBridge(cvrp, bestSolution, candidate);
solver->localSearch_TwoOpt(cvrp, candidate, candidate);
solver->acceptanceCriterion_BestSolution(bestSolution, candidate);
// bestSolution agora contém a melhor entre ela mesma e candidate
```

---

## 🛠️ Métodos Auxiliares

### `calculate_swap_star_delta()`

**Assinatura:**
```cpp
double calculate_swap_star_delta(CVRP *instance, const vector<int>& route1, const vector<int>& route2, int i, int j, int k)
```

**Descrição de Alto Nível:**
Função auxiliar crítica para o Swap Star que calcula a variação de custo (delta) ao mover uma cadeia de k clientes da posição i na route1 para a posição j na route2. Esta função é otimizada para evitar recalcular toda a solução, computando apenas a diferença nos arcos afetados. Distingue entre movimentos intra-rota (mesma rota) e inter-rota (rotas diferentes), cada um com lógica de delta específica.

**Estratégia:**

**Caso 1: Inter-Rota (route1 != route2)**
```
Antes:  A → [B...C] → D  (route1)
        E → F            (route2)

Depois: A → D            (route1)
        E → [B...C] → F  (route2)

Delta = (A→D + E→B + C→F) - (A→B + C→D + E→F)
```

**Caso 2: Intra-Rota (route1 == route2)**
- **Se j < i:** Inserção acontece antes da remoção
- **Se j > i:** Inserção acontece depois da remoção
- Cálculo é mais complexo devido a mudanças de índices

**Parâmetros:**
- `instance`: Ponteiro para instância CVRP (para acessar matriz de distâncias)
- `route1`: Referência constante para a rota de origem
- `route2`: Referência constante para a rota de destino
- `i`: Índice inicial da cadeia a ser movida (em route1)
- `j`: Índice de inserção (em route2)
- `k`: Tamanho da cadeia (chain_length)

**Retorno:**
- `double`: Delta de custo (positivo = piora, negativo = melhoria)

**Nós Relevantes:**
```
route1: ... → A → B → ... → C → D → ...
               ↑     cadeia    ↑
              i-1    de k      i+k
                    clientes

route2: ... → E → F → ...
               ↑
              j-1   j
```

**Complexidade:** O(1) - apenas acessa matriz de distâncias algumas vezes

**Características:**
- ✅ Extremamente eficiente (evita recalcular solução inteira)
- ✅ Distingue corretamente casos intra e inter-rota
- ✅ Usa referências constantes (sem cópias)
- ✅ Retorna apenas a diferença de custo

**Detalhes de Implementação:**

**Arcos Removidos (Inter-Rota):**
1. A → B (conexão antes da cadeia em route1)
2. C → D (conexão depois da cadeia em route1)
3. E → F (onde a cadeia será inserida em route2)

**Arcos Adicionados (Inter-Rota):**
1. A → D (nova conexão em route1 após remoção)
2. E → B (início da cadeia inserida em route2)
3. C → F (fim da cadeia inserida em route2)

**Validação:**
⚠️ Esta função assume que:
- Os índices i, j, k são válidos
- A cadeia está completamente dentro de route1
- A posição j é válida em route2
- Não verifica viabilidade de capacidade (feito em localSearch_SwapStar)

**Exemplo de Cálculo:**
```
route1: 0 → 1 → 2 → 3 → 0  (i=1, k=2, cadeia=[1,2])
route2: 0 → 4 → 5 → 0      (j=1)

Nós: A=0, B=1, C=2, D=3, E=0, F=4

Custo Removido:
  dist[0→1] + dist[2→3] + dist[0→4]

Custo Adicionado:
  dist[0→3] + dist[0→1] + dist[2→4]

Delta = Adicionado - Removido
```

**Quando Usar:**
- Internamente chamado por `localSearch_SwapStar()`
- Não deve ser chamado diretamente por código cliente

**Otimização Possível:**
Para cadeias longas (k > 1), também considere o custo interno da cadeia:
```cpp
// Custo interno da cadeia (B→...→C)
double chainInternalCost = 0;
for(int idx = i; idx < i + k - 1; idx++) {
    chainInternalCost += instance->distanceMatrix[
        route1[idx] * instance->nodesDimension + route1[idx+1]
    ];
}
```

---

## 📊 Resumo Comparativo dos Métodos

### Métodos de Construção

| Método | Complexidade | Qualidade | Uso Recomendado |
|--------|--------------|-----------|-----------------|
| **Greedy** | O(n²) | ⭐⭐⭐ | Padrão (implementado) |
| Random Insertion | O(n²) | ⭐⭐ | Diversidade |
| Nearest Neighbor | O(n²) | ⭐⭐⭐ | Alternativa ao Greedy |
| Clarke-Wright | O(n² log n) | ⭐⭐⭐⭐ | Instâncias grandes |

### Métodos de Busca Local

| Método | Complexidade | Melhoria | Escopo | Uso Recomendado |
|--------|--------------|----------|--------|-----------------|
| **2-Opt** | O(n²) | ⭐⭐⭐ | Intra-rota | Sempre (padrão) |
| **3-Opt** | O(n³) | ⭐⭐⭐⭐ | Intra-rota | Instâncias pequenas |
| **Swap Star** | O(n²m²) | ⭐⭐⭐⭐ | Inter-rota | Após 2-Opt |
| Or-Opt | O(n²) | ⭐⭐⭐ | Intra-rota | Complemento ao 2-Opt |
| Relocation | O(nm) | ⭐⭐ | Inter-rota | Balanceamento |

### Métodos de Perturbação

| Método | Intensidade | Rapidez | Eficácia | Uso Recomendado |
|--------|-------------|---------|----------|-----------------|
| **Double-Bridge** | Média-Alta | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | Padrão ILS |
| Large Neighborhood | Alta | ⭐⭐ | ⭐⭐⭐⭐ | Instâncias difíceis |
| Random Shake | Baixa | ⭐⭐⭐⭐⭐ | ⭐⭐ | Diversificação rápida |

---

## 🎯 Estratégias de Uso Combinado

### Estratégia Básica (Implementada)
```cpp
// 1. Construir solução inicial
solver->initialSolution_Greedy(cvrp, solution);

// 2. Otimização inicial
solver->localSearch_TwoOpt(cvrp, solution, bestSolution);

// 3. Loop ILS
for(int i = 0; i < MAX_ITERATIONS; i++) {
    solver->pertubation_DoubleBridge(cvrp, bestSolution, solution);
    solver->localSearch_TwoOpt(cvrp, solution, solution);
    solver->acceptanceCriterion_BestSolution(bestSolution, solution);
}
```

### Estratégia Avançada (Recomendada)
```cpp
// 1. Construir solução inicial
solver->initialSolution_Greedy(cvrp, solution);

// 2. Otimização inicial intensiva
solver->localSearch_TwoOpt(cvrp, solution, bestSolution);
solver->localSearch_SwapStar(cvrp, bestSolution, 1); // Inter-rota

// 3. Loop ILS com busca multi-nível
for(int i = 0; i < MAX_ITERATIONS; i++) {
    solver->pertubation_DoubleBridge(cvrp, bestSolution, solution);
    solver->localSearch_TwoOpt(cvrp, solution, solution);

    // A cada 10 iterações, busca mais profunda
    if(i % 10 == 0) {
        solver->localSearch_SwapStar(cvrp, solution, 2);
    }

    solver->acceptanceCriterion_BestSolution(bestSolution, solution);
}

// 4. Refinamento final
solver->localSearch_SwapStar(cvrp, bestSolution, 1);
solver->localSearch_TwoOpt(cvrp, bestSolution, bestSolution);
```

### Estratégia para Instâncias Pequenas (< 50 clientes)
```cpp
solver->initialSolution_Greedy(cvrp, solution);
solver->localSearch_ThreeOpt(cvrp, solution, bestSolution); // Usa 3-Opt

for(int i = 0; i < MAX_ITERATIONS; i++) {
    solver->pertubation_DoubleBridge(cvrp, bestSolution, solution);
    solver->localSearch_ThreeOpt(cvrp, solution, solution);
    solver->acceptanceCriterion_BestSolution(bestSolution, solution);
}
```

---

## 🔧 Parâmetros Recomendados

### Por Tamanho de Instância

| Clientes | MAX_ITERATIONS | Busca Local | Perturbação | Tempo Esperado |
|----------|----------------|-------------|-------------|----------------|
| < 20 | 500 | 3-Opt | Double-Bridge | < 1 min |
| 20-50 | 1000 | 2-Opt + Swap* | Double-Bridge | 1-5 min |
| 50-100 | 2000 | 2-Opt | Double-Bridge | 5-15 min |
| 100-200 | 3000 | 2-Opt | Double-Bridge | 15-45 min |
| > 200 | 5000 | 2-Opt | Double-Bridge | > 45 min |

### Chain Length para Swap Star

| Instância | chain_length | Explicação |
|-----------|--------------|------------|
| Qualquer | 1 | Padrão: rápido e efetivo |
| Clusters densos | 2 | Move pares relacionados |
| Rotas longas | 2-3 | Maior reestruturação |
| Tempo limitado | 1 | Máxima velocidade |

---

## 📚 Referências Técnicas

1. **2-Opt**: Croes, G. A. (1958). A Method for Solving Traveling-Salesman Problems. Operations Research.

2. **3-Opt**: Lin, S. (1965). Computer solutions of the traveling salesman problem. Bell System Technical Journal.

3. **Double-Bridge**: Martin, O., Otto, S. W., & Felten, E. W. (1991). Large-step Markov chains for the traveling salesman problem.

4. **ILS Framework**: Lourenço, H. R., Martin, O. C., & Stützle, T. (2003). Iterated local search. Handbook of metaheuristics.

5. **Swap Star**: Taillard, É. D. (1993). Parallel iterative search methods for vehicle routing problems. Networks.

---

## 📝 Notas de Implementação

### Código Comentado
O arquivo contém duas versões comentadas de métodos:
- `initialSolution_Greedy()` (linhas 11-62): Versão antiga com debug verboso
- `initialSolutionNNH()` (linhas 64-96): Implementação incompleta do Nearest Neighbor

Esses códigos foram mantidos para referência histórica mas não são funcionais.

### Melhorias Futuras Sugeridas

1. **Pertubation_DoubleBridge**
   - Perturbar rota aleatória em vez de sempre a primeira
   - Suportar perturbação de múltiplas rotas
   - Adicionar parâmetro de intensidade

2. **AcceptanceCriterion**
   - Implementar Simulated Annealing
   - Adicionar critério Late Acceptance
   - Suportar threshold dinâmico

3. **LocalSearch_SwapStar**
   - Implementar versão Best Improvement
   - Adicionar poda de movimentos inviáveis
   - Cachear cálculos de delta

4. **Métodos Faltantes**
   - Implementar Or-Opt
   - Implementar Relocation
   - Implementar Lin-Kernighan completo
   - Implementar Clarke-Wright

---

## 🤝 Contribuindo

Se você implementar novos métodos ou melhorias, por favor:
1. Documente seguindo este formato
2. Adicione complexidade computacional
3. Inclua exemplo de uso
4. Compare com métodos existentes
5. Atualize as tabelas comparativas

---

## 📄 Licença

Este documento faz parte do projeto CVRP-ILS e está sob a mesma licença MIT.

---

<div align="center">

**Documentação Técnica - Solver Methods**

Última atualização: 26/11/2025

</div>
