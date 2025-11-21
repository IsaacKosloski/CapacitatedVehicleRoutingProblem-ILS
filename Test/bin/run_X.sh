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
run_solver ../Benchmarks/X/X-n1001-k43.vrp X-n1001-k43 X
run_solver ../Benchmarks/X/X-n101-k25.vrp X-n101-k25 X
run_solver ../Benchmarks/X/X-n106-k14.vrp X-n106-k14 X
run_solver ../Benchmarks/X/X-n110-k13.vrp X-n110-k13 X
run_solver ../Benchmarks/X/X-n115-k10.vrp X-n115-k10 X
run_solver ../Benchmarks/X/X-n120-k6.vrp X-n120-k6 X
run_solver ../Benchmarks/X/X-n125-k30.vrp X-n125-k30 X
run_solver ../Benchmarks/X/X-n129-k18.vrp X-n129-k18 X
run_solver ../Benchmarks/X/X-n134-k13.vrp X-n134-k13 X
run_solver ../Benchmarks/X/X-n139-k10.vrp X-n139-k10 X
run_solver ../Benchmarks/X/X-n143-k7.vrp X-n143-k7 X
run_solver ../Benchmarks/X/X-n148-k46.vrp X-n148-k46 X
run_solver ../Benchmarks/X/X-n153-k22.vrp X-n153-k22 X
run_solver ../Benchmarks/X/X-n157-k13.vrp X-n157-k13 X
run_solver ../Benchmarks/X/X-n162-k11.vrp X-n162-k11 X
run_solver ../Benchmarks/X/X-n167-k10.vrp X-n167-k10 X
run_solver ../Benchmarks/X/X-n172-k51.vrp X-n172-k51 X
run_solver ../Benchmarks/X/X-n176-k26.vrp X-n176-k26 X
run_solver ../Benchmarks/X/X-n181-k23.vrp X-n181-k23 X
run_solver ../Benchmarks/X/X-n186-k15.vrp X-n186-k15 X
run_solver ../Benchmarks/X/X-n190-k8.vrp X-n190-k8 X
run_solver ../Benchmarks/X/X-n195-k51.vrp X-n195-k51 X
run_solver ../Benchmarks/X/X-n200-k36.vrp X-n200-k36 X
run_solver ../Benchmarks/X/X-n204-k19.vrp X-n204-k19 X
run_solver ../Benchmarks/X/X-n209-k16.vrp X-n209-k16 X
run_solver ../Benchmarks/X/X-n214-k11.vrp X-n214-k11 X
run_solver ../Benchmarks/X/X-n219-k73.vrp X-n219-k73 X
run_solver ../Benchmarks/X/X-n223-k34.vrp X-n223-k34 X
run_solver ../Benchmarks/X/X-n228-k23.vrp X-n228-k23 X
run_solver ../Benchmarks/X/X-n233-k16.vrp X-n233-k16 X
run_solver ../Benchmarks/X/X-n237-k14.vrp X-n237-k14 X
run_solver ../Benchmarks/X/X-n242-k48.vrp X-n242-k48 X
run_solver ../Benchmarks/X/X-n247-k50.vrp X-n247-k50 X
run_solver ../Benchmarks/X/X-n251-k28.vrp X-n251-k28 X
run_solver ../Benchmarks/X/X-n256-k16.vrp X-n256-k16 X
run_solver ../Benchmarks/X/X-n261-k13.vrp X-n261-k13 X
run_solver ../Benchmarks/X/X-n266-k58.vrp X-n266-k58 X
run_solver ../Benchmarks/X/X-n270-k35.vrp X-n270-k35 X
run_solver ../Benchmarks/X/X-n275-k28.vrp X-n275-k28 X
run_solver ../Benchmarks/X/X-n280-k17.vrp X-n280-k17 X
run_solver ../Benchmarks/X/X-n284-k15.vrp X-n284-k15 X
run_solver ../Benchmarks/X/X-n289-k60.vrp X-n289-k60 X
run_solver ../Benchmarks/X/X-n294-k50.vrp X-n294-k50 X
run_solver ../Benchmarks/X/X-n298-k31.vrp X-n298-k31 X
run_solver ../Benchmarks/X/X-n303-k21.vrp X-n303-k21 X
run_solver ../Benchmarks/X/X-n308-k13.vrp X-n308-k13 X
run_solver ../Benchmarks/X/X-n313-k71.vrp X-n313-k71 X
run_solver ../Benchmarks/X/X-n317-k53.vrp X-n317-k53 X
run_solver ../Benchmarks/X/X-n322-k28.vrp X-n322-k28 X
run_solver ../Benchmarks/X/X-n327-k20.vrp X-n327-k20 X
run_solver ../Benchmarks/X/X-n331-k15.vrp X-n331-k15 X
run_solver ../Benchmarks/X/X-n336-k84.vrp X-n336-k84 X
run_solver ../Benchmarks/X/X-n344-k43.vrp X-n344-k43 X
run_solver ../Benchmarks/X/X-n351-k40.vrp X-n351-k40 X
run_solver ../Benchmarks/X/X-n359-k29.vrp X-n359-k29 X
run_solver ../Benchmarks/X/X-n367-k17.vrp X-n367-k17 X
run_solver ../Benchmarks/X/X-n376-k94.vrp X-n376-k94 X
run_solver ../Benchmarks/X/X-n384-k52.vrp X-n384-k52 X
run_solver ../Benchmarks/X/X-n393-k38.vrp X-n393-k38 X
run_solver ../Benchmarks/X/X-n401-k29.vrp X-n401-k29 X
run_solver ../Benchmarks/X/X-n411-k19.vrp X-n411-k19 X
run_solver ../Benchmarks/X/X-n420-k130.vrp X-n420-k130 X
run_solver ../Benchmarks/X/X-n429-k61.vrp X-n429-k61 X
run_solver ../Benchmarks/X/X-n439-k37.vrp X-n439-k37 X
run_solver ../Benchmarks/X/X-n449-k29.vrp X-n449-k29 X
run_solver ../Benchmarks/X/X-n459-k26.vrp X-n459-k26 X
run_solver ../Benchmarks/X/X-n469-k138.vrp X-n469-k138 X
run_solver ../Benchmarks/X/X-n480-k70.vrp X-n480-k70 X
run_solver ../Benchmarks/X/X-n491-k59.vrp X-n491-k59 X
run_solver ../Benchmarks/X/X-n502-k39.vrp X-n502-k39 X
run_solver ../Benchmarks/X/X-n513-k21.vrp X-n513-k21 X
run_solver ../Benchmarks/X/X-n524-k153.vrp X-n524-k153 X
run_solver ../Benchmarks/X/X-n536-k96.vrp X-n536-k96 X
run_solver ../Benchmarks/X/X-n548-k50.vrp X-n548-k50 X
run_solver ../Benchmarks/X/X-n561-k42.vrp X-n561-k42 X
run_solver ../Benchmarks/X/X-n573-k30.vrp X-n573-k30 X
run_solver ../Benchmarks/X/X-n586-k159.vrp X-n586-k159 X
run_solver ../Benchmarks/X/X-n599-k92.vrp X-n599-k92 X
run_solver ../Benchmarks/X/X-n613-k62.vrp X-n613-k62 X
run_solver ../Benchmarks/X/X-n627-k43.vrp X-n627-k43 X
run_solver ../Benchmarks/X/X-n641-k35.vrp X-n641-k35 X
run_solver ../Benchmarks/X/X-n655-k131.vrp X-n655-k131 X
run_solver ../Benchmarks/X/X-n670-k130.vrp X-n670-k130 X
run_solver ../Benchmarks/X/X-n685-k75.vrp X-n685-k75 X
run_solver ../Benchmarks/X/X-n701-k44.vrp X-n701-k44 X
run_solver ../Benchmarks/X/X-n716-k35.vrp X-n716-k35 X
run_solver ../Benchmarks/X/X-n733-k159.vrp X-n733-k159 X
run_solver ../Benchmarks/X/X-n749-k98.vrp X-n749-k98 X
run_solver ../Benchmarks/X/X-n766-k71.vrp X-n766-k71 X
run_solver ../Benchmarks/X/X-n783-k48.vrp X-n783-k48 X
run_solver ../Benchmarks/X/X-n801-k40.vrp X-n801-k40 X
run_solver ../Benchmarks/X/X-n819-k171.vrp X-n819-k171 X
run_solver ../Benchmarks/X/X-n837-k142.vrp X-n837-k142 X
run_solver ../Benchmarks/X/X-n856-k95.vrp X-n856-k95 X
run_solver ../Benchmarks/X/X-n876-k59.vrp X-n876-k59 X
run_solver ../Benchmarks/X/X-n895-k37.vrp X-n895-k37 X
run_solver ../Benchmarks/X/X-n916-k207.vrp X-n916-k207 X
run_solver ../Benchmarks/X/X-n936-k151.vrp X-n936-k151 X
run_solver ../Benchmarks/X/X-n957-k87.vrp X-n957-k87 X
run_solver ../Benchmarks/X/X-n979-k58.vrp X-n979-k58 X
