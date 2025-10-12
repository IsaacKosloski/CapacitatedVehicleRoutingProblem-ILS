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
#run_solver ../Benchmarks/XXL/Antwerp1.vrp Antwerp1 XXL
#run_solver ../Benchmarks/XXL/Antwerp2.vrp Antwerp2 XXL
#run_solver ../Benchmarks/XXL/Brussels1.vrp Brussels1 XXL
#run_solver ../Benchmarks/XXL/Brussels2.vrp Brussels2 XXL
#run_solver ../Benchmarks/XXL/Flanders1.vrp Flanders1 XXL
#run_solver ../Benchmarks/XXL/Flanders2.vrp Flanders2 XXL
#run_solver ../Benchmarks/XXL/Ghent1.vrp Ghent1 XXL
run_solver ../Benchmarks/XXL/Ghent2.vrp Ghent2 XXL
run_solver ../Benchmarks/XXL/Leuven1.vrp Leuven1 XXL
run_solver ../Benchmarks/XXL/Leuven2.vrp Leuven2 XXL
