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
run_solver ../Benchmarks/CMT/CMT1.vrp CMT1 CMT
run_solver ../Benchmarks/CMT/CMT10.vrp CMT10 CMT
run_solver ../Benchmarks/CMT/CMT11.vrp CMT11 CMT
run_solver ../Benchmarks/CMT/CMT12.vrp CMT12 CMT
run_solver ../Benchmarks/CMT/CMT13.vrp CMT13 CMT
run_solver ../Benchmarks/CMT/CMT14.vrp CMT14 CMT
run_solver ../Benchmarks/CMT/CMT2.vrp CMT2 CMT
run_solver ../Benchmarks/CMT/CMT3.vrp CMT3 CMT
run_solver ../Benchmarks/CMT/CMT4.vrp CMT4 CMT
run_solver ../Benchmarks/CMT/CMT5.vrp CMT5 CMT
run_solver ../Benchmarks/CMT/CMT6.vrp CMT6 CMT
run_solver ../Benchmarks/CMT/CMT7.vrp CMT7 CMT
run_solver ../Benchmarks/CMT/CMT8.vrp CMT8 CMT
run_solver ../Benchmarks/CMT/CMT9.vrp CMT9 CMT
