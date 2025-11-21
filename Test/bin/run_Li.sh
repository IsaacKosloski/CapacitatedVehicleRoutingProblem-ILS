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
run_solver ../Benchmarks/Li/Li_21.vrp Li_21 Li
run_solver ../Benchmarks/Li/Li_22.vrp Li_22 Li
run_solver ../Benchmarks/Li/Li_23.vrp Li_23 Li
run_solver ../Benchmarks/Li/Li_24.vrp Li_24 Li
run_solver ../Benchmarks/Li/Li_25.vrp Li_25 Li
run_solver ../Benchmarks/Li/Li_26.vrp Li_26 Li
run_solver ../Benchmarks/Li/Li_27.vrp Li_27 Li
run_solver ../Benchmarks/Li/Li_28.vrp Li_28 Li
run_solver ../Benchmarks/Li/Li_29.vrp Li_29 Li
run_solver ../Benchmarks/Li/Li_30.vrp Li_30 Li
run_solver ../Benchmarks/Li/Li_31.vrp Li_31 Li
run_solver ../Benchmarks/Li/Li_32.vrp Li_32 Li
