#!/bin/bash
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
run_solver ../Benchmarks/P/P-n101-k4.vrp P-n101-k4 P
run_solver ../Benchmarks/P/P-n16-k8.vrp P-n16-k8 P
run_solver ../Benchmarks/P/P-n19-k2.vrp P-n19-k2 P
run_solver ../Benchmarks/P/P-n20-k2.vrp P-n20-k2 P
run_solver ../Benchmarks/P/P-n21-k2.vrp P-n21-k2 P
run_solver ../Benchmarks/P/P-n22-k2.vrp P-n22-k2 P
run_solver ../Benchmarks/P/P-n22-k8.vrp P-n22-k8 P
run_solver ../Benchmarks/P/P-n23-k8.vrp P-n23-k8 P
run_solver ../Benchmarks/P/P-n40-k5.vrp P-n40-k5 P
run_solver ../Benchmarks/P/P-n45-k5.vrp P-n45-k5 P
run_solver ../Benchmarks/P/P-n50-k10.vrp P-n50-k10 P
run_solver ../Benchmarks/P/P-n50-k7.vrp P-n50-k7 P
run_solver ../Benchmarks/P/P-n50-k8.vrp P-n50-k8 P
run_solver ../Benchmarks/P/P-n51-k10.vrp P-n51-k10 P
run_solver ../Benchmarks/P/P-n55-k10.vrp P-n55-k10 P
run_solver ../Benchmarks/P/P-n55-k15.vrp P-n55-k15 P
run_solver ../Benchmarks/P/P-n55-k7.vrp P-n55-k7 P
run_solver ../Benchmarks/P/P-n55-k8.vrp P-n55-k8 P
run_solver ../Benchmarks/P/P-n60-k10.vrp P-n60-k10 P
run_solver ../Benchmarks/P/P-n60-k15.vrp P-n60-k15 P
run_solver ../Benchmarks/P/P-n65-k10.vrp P-n65-k10 P
run_solver ../Benchmarks/P/P-n70-k10.vrp P-n70-k10 P
run_solver ../Benchmarks/P/P-n76-k4.vrp P-n76-k4 P
run_solver ../Benchmarks/P/P-n76-k5.vrp P-n76-k5 P
