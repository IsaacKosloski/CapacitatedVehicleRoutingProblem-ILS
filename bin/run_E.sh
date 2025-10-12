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
run_solver ../Benchmarks/E/E-n101-k14.vrp E-n101-k14 E
run_solver ../Benchmarks/E/E-n101-k8.vrp E-n101-k8 E
run_solver ../Benchmarks/E/E-n22-k4.vrp E-n22-k4 E
run_solver ../Benchmarks/E/E-n23-k3.vrp E-n23-k3 E
run_solver ../Benchmarks/E/E-n30-k3.vrp E-n30-k3 E
run_solver ../Benchmarks/E/E-n33-k4.vrp E-n33-k4 E
run_solver ../Benchmarks/E/E-n51-k5.vrp E-n51-k5 E
run_solver ../Benchmarks/E/E-n76-k10.vrp E-n76-k10 E
run_solver ../Benchmarks/E/E-n76-k14.vrp E-n76-k14 E
run_solver ../Benchmarks/E/E-n76-k7.vrp E-n76-k7 E
run_solver ../Benchmarks/E/E-n76-k8.vrp E-n76-k8 E
