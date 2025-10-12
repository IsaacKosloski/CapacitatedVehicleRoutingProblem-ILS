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
run_solver ../Benchmarks/D/Loggi-n1001-k31.vrp Loggi-n1001-k31 D
run_solver ../Benchmarks/D/Loggi-n401-k23.vrp Loggi-n401-k23 D
run_solver ../Benchmarks/D/Loggi-n501-k24.vrp Loggi-n501-k24 D
run_solver ../Benchmarks/D/Loggi-n601-k19.vrp Loggi-n601-k19 D
run_solver ../Benchmarks/D/Loggi-n601-k42.vrp Loggi-n601-k42 D
run_solver ../Benchmarks/D/Loggi-n901-k42.vrp Loggi-n901-k42 D
run_solver ../Benchmarks/D/ORTEC-n242-k12.vrp ORTEC-n242-k12 D
run_solver ../Benchmarks/D/ORTEC-n323-k21.vrp ORTEC-n323-k21 D
run_solver ../Benchmarks/D/ORTEC-n405-k18.vrp ORTEC-n405-k18 D
run_solver ../Benchmarks/D/ORTEC-n455-k41.vrp ORTEC-n455-k41 D
run_solver ../Benchmarks/D/ORTEC-n510-k23.vrp ORTEC-n510-k23 D
run_solver ../Benchmarks/D/ORTEC-n701-k64.vrp ORTEC-n701-k64 D
