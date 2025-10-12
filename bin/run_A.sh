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
run_solver ../Benchmarks/A/A-n32-k5.vrp A-n32-k5 A
run_solver ../Benchmarks/A/A-n33-k5.vrp A-n33-k5 A
run_solver ../Benchmarks/A/A-n33-k6.vrp A-n33-k6 A
run_solver ../Benchmarks/A/A-n34-k5.vrp A-n34-k5 A
run_solver ../Benchmarks/A/A-n36-k5.vrp A-n36-k5 A
run_solver ../Benchmarks/A/A-n37-k5.vrp A-n37-k5 A
run_solver ../Benchmarks/A/A-n37-k6.vrp A-n37-k6 A
run_solver ../Benchmarks/A/A-n38-k5.vrp A-n38-k5 A
run_solver ../Benchmarks/A/A-n39-k5.vrp A-n39-k5 A
run_solver ../Benchmarks/A/A-n39-k6.vrp A-n39-k6 A
run_solver ../Benchmarks/A/A-n44-k6.vrp A-n44-k6 A
run_solver ../Benchmarks/A/A-n45-k6.vrp A-n45-k6 A
run_solver ../Benchmarks/A/A-n45-k7.vrp A-n45-k7 A
run_solver ../Benchmarks/A/A-n46-k7.vrp A-n46-k7 A
run_solver ../Benchmarks/A/A-n48-k7.vrp A-n48-k7 A
run_solver ../Benchmarks/A/A-n53-k7.vrp A-n53-k7 A
run_solver ../Benchmarks/A/A-n54-k7.vrp A-n54-k7 A
run_solver ../Benchmarks/A/A-n55-k9.vrp A-n55-k9 A
run_solver ../Benchmarks/A/A-n60-k9.vrp A-n60-k9 A
run_solver ../Benchmarks/A/A-n61-k9.vrp A-n61-k9 A
run_solver ../Benchmarks/A/A-n62-k8.vrp A-n62-k8 A
run_solver ../Benchmarks/A/A-n63-k10.vrp A-n63-k10 A
run_solver ../Benchmarks/A/A-n63-k9.vrp A-n63-k9 A
run_solver ../Benchmarks/A/A-n64-k9.vrp A-n64-k9 A
run_solver ../Benchmarks/A/A-n65-k9.vrp A-n65-k9 A
run_solver ../Benchmarks/A/A-n69-k9.vrp A-n69-k9 A
run_solver ../Benchmarks/A/A-n80-k10.vrp A-n80-k10 A
