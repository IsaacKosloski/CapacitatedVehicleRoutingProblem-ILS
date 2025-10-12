#!/bin/bash

BENCHMARK_DIR="Benchmarks"
OUTPUT_DIR="bin"

# Garante que o diretório de saída existe
mkdir -p "$OUTPUT_DIR"

for letter_dir in "$BENCHMARK_DIR"/*/; do
    letter=$(basename "$letter_dir")
    script_path="$OUTPUT_DIR/run_${letter}.sh"

    {
        echo "#!/bin/bash"
        echo "cd .."
        echo "make"
        echo "cd bin"
    } > "$script_path"

    for vrp_file in "$letter_dir"/*.vrp; do
        filename=$(basename "$vrp_file")
        name_without_ext="${filename%.vrp}"
        echo "./cvrp_solver ../$BENCHMARK_DIR/$letter/$filename $name_without_ext $letter" >> "$script_path"
    done

    chmod +x "$script_path"
    echo "✅ Script gerado: $script_path"
done

# Criar o script run_all.sh para executar todos os run_X.sh
RUN_ALL="$OUTPUT_DIR/run_all.sh"
echo "#!/bin/bash" > "$RUN_ALL"

# Adiciona cada run_X.sh encontrado no bin
for run_script in "$OUTPUT_DIR"/run_*.sh; do
    script_name=$(basename "$run_script")
    echo "echo '▶️ Rodando $script_name'" >> "$RUN_ALL"
    echo "./$script_name" >> "$RUN_ALL"
    echo "" >> "$RUN_ALL"
done

chmod +x "$RUN_ALL"
echo "🚀 Script global gerado: $RUN_ALL"