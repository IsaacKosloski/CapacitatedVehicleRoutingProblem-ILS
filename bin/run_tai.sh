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
run_solver ../Benchmarks/tai/tai100a.vrp tai100a tai
run_solver ../Benchmarks/tai/tai100b.vrp tai100b tai
run_solver ../Benchmarks/tai/tai100c.vrp tai100c tai
run_solver ../Benchmarks/tai/tai100d.vrp tai100d tai
run_solver ../Benchmarks/tai/tai150a.vrp tai150a tai
run_solver ../Benchmarks/tai/tai150b.vrp tai150b tai
run_solver ../Benchmarks/tai/tai150c.vrp tai150c tai
run_solver ../Benchmarks/tai/tai150d.vrp tai150d tai
run_solver ../Benchmarks/tai/tai385.vrp tai385 tai
run_solver ../Benchmarks/tai/tai75a.vrp tai75a tai
run_solver ../Benchmarks/tai/tai75b.vrp tai75b tai
run_solver ../Benchmarks/tai/tai75c.vrp tai75c tai
run_solver ../Benchmarks/tai/tai75d.vrp tai75d tai
