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
run_solver ../Benchmarks/Golden/Golden_1.vrp Golden_1 Golden
run_solver ../Benchmarks/Golden/Golden_10.vrp Golden_10 Golden
run_solver ../Benchmarks/Golden/Golden_11.vrp Golden_11 Golden
run_solver ../Benchmarks/Golden/Golden_12.vrp Golden_12 Golden
run_solver ../Benchmarks/Golden/Golden_13.vrp Golden_13 Golden
run_solver ../Benchmarks/Golden/Golden_14.vrp Golden_14 Golden
run_solver ../Benchmarks/Golden/Golden_15.vrp Golden_15 Golden
run_solver ../Benchmarks/Golden/Golden_16.vrp Golden_16 Golden
run_solver ../Benchmarks/Golden/Golden_17.vrp Golden_17 Golden
run_solver ../Benchmarks/Golden/Golden_18.vrp Golden_18 Golden
run_solver ../Benchmarks/Golden/Golden_19.vrp Golden_19 Golden
run_solver ../Benchmarks/Golden/Golden_2.vrp Golden_2 Golden
run_solver ../Benchmarks/Golden/Golden_20.vrp Golden_20 Golden
run_solver ../Benchmarks/Golden/Golden_3.vrp Golden_3 Golden
run_solver ../Benchmarks/Golden/Golden_4.vrp Golden_4 Golden
run_solver ../Benchmarks/Golden/Golden_5.vrp Golden_5 Golden
run_solver ../Benchmarks/Golden/Golden_6.vrp Golden_6 Golden
run_solver ../Benchmarks/Golden/Golden_7.vrp Golden_7 Golden
run_solver ../Benchmarks/Golden/Golden_8.vrp Golden_8 Golden
run_solver ../Benchmarks/Golden/Golden_9.vrp Golden_9 Golden
