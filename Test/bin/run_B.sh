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
run_solver ../Benchmarks/B/B-n31-k5.vrp B-n31-k5 B
run_solver ../Benchmarks/B/B-n34-k5.vrp B-n34-k5 B
run_solver ../Benchmarks/B/B-n35-k5.vrp B-n35-k5 B
run_solver ../Benchmarks/B/B-n38-k6.vrp B-n38-k6 B
run_solver ../Benchmarks/B/B-n39-k5.vrp B-n39-k5 B
run_solver ../Benchmarks/B/B-n41-k6.vrp B-n41-k6 B
run_solver ../Benchmarks/B/B-n43-k6.vrp B-n43-k6 B
run_solver ../Benchmarks/B/B-n44-k7.vrp B-n44-k7 B
run_solver ../Benchmarks/B/B-n45-k5.vrp B-n45-k5 B
run_solver ../Benchmarks/B/B-n45-k6.vrp B-n45-k6 B
run_solver ../Benchmarks/B/B-n50-k7.vrp B-n50-k7 B
run_solver ../Benchmarks/B/B-n50-k8.vrp B-n50-k8 B
run_solver ../Benchmarks/B/B-n51-k7.vrp B-n51-k7 B
run_solver ../Benchmarks/B/B-n52-k7.vrp B-n52-k7 B
run_solver ../Benchmarks/B/B-n56-k7.vrp B-n56-k7 B
run_solver ../Benchmarks/B/B-n57-k7.vrp B-n57-k7 B
run_solver ../Benchmarks/B/B-n57-k9.vrp B-n57-k9 B
run_solver ../Benchmarks/B/B-n63-k10.vrp B-n63-k10 B
run_solver ../Benchmarks/B/B-n64-k9.vrp B-n64-k9 B
run_solver ../Benchmarks/B/B-n66-k9.vrp B-n66-k9 B
run_solver ../Benchmarks/B/B-n67-k10.vrp B-n67-k10 B
run_solver ../Benchmarks/B/B-n68-k9.vrp B-n68-k9 B
run_solver ../Benchmarks/B/B-n78-k10.vrp B-n78-k10 B
