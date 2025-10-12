#!/bin/bash
cd ..
make
cd bin

# Função que mede tempo e memória sem alterar a execução do binário
run_solver () {
    local instance_path=$1
    local instance_name=$2
    local method=$3

    local log_dir="logs"
    mkdir -p "$log_dir"

    local stats_file="${log_dir}/${method}_${instance_name}_stats.txt"

    echo "[INFO] Rodando $instance_name com método $method..."

    start_time=$(date +%s.%N)
    ./cvrp_solver "$instance_path" "$instance_name" "$method" &
    pid=$!

    peak_mem=0
    while kill -0 "$pid" 2> /dev/null; do
        mem=$(ps -o rss= -p "$pid")
        [[ "$mem" =~ ^[0-9]+$ ]] && (( mem > peak_mem )) && peak_mem=$mem
        sleep 0.1
    done

    end_time=$(date +%s.%N)
    elapsed=$(echo "$end_time - $start_time" | bc)

    {
        echo "Instância: $instance_name"
        echo "Método: $method"
        echo "Tempo total (s): $elapsed"
        echo "Memória máxima (KB): $peak_mem"
    } > "$stats_file"
}
run_solver ../Benchmarks/A/A-n32-k5.vrp A-n32-k5 SwapStar
run_solver ../Benchmarks/A/A-n33-k5.vrp A-n33-k5 SwapStar
run_solver ../Benchmarks/A/A-n48-k7.vrp A-n48-k7 SwapStar
run_solver ../Benchmarks/A/A-n53-k7.vrp A-n53-k7 SwapStar
run_solver ../Benchmarks/A/A-n69-k9.vrp A-n69-k9 SwapStar
run_solver ../Benchmarks/A/A-n80-k10.vrp A-n80-k10 SwapStar

run_solver ../Benchmarks/D/ORTEC-n701-k64.vrp ORTEC-n701-k64 SwapStar
run_solver ../Benchmarks/D/Loggi-n1001-k31.vrp Loggi-n1001-k31 SwapStar


run_solver ../Benchmarks/Golden/Golden_15.vrp Golden_15 SwapStar
run_solver ../Benchmarks/Golden/Golden_20.vrp Golden_20 SwapStar

run_solver ../Benchmarks/XXL/Antwerp1.vrp Antwerp1 SwapStar
run_solver ../Benchmarks/XXL/Leuven2.vrp Leuven2 SwapStar

