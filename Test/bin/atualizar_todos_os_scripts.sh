#!/bin/bash

echo "🔧 Atualizando todos os arquivos run_*.sh (exceto run_all.sh)..."

for script in run_*.sh; do
    [[ "$script" == "run_all.sh" ]] && continue

    echo "➡️ Atualizando $script..."

    # Backup do original
    cp "$script" "backup_$script"

    # Cria novo conteúdo com função de medição e substituição das chamadas
    awk '
    BEGIN {
        inserted = 0
        print "#!/bin/bash"
    }
    /cd bin/ {
        print
        print ""
        print "# Função que mede tempo e memória sem alterar a execução do binário"
        print "run_solver () {"
        print "    local instance_path=$1"
        print "    local instance_name=$2"
        print "    local method=$3"
        print ""
        print "    local log_dir=\"logs\""
        print "    mkdir -p \"$log_dir\""
        print ""
        print "    local stats_file=\"${log_dir}/${method}_${instance_name}_stats.txt\""
        print ""
        print "    echo \"[INFO] Rodando $instance_name com método $method...\""
        print ""
        print "    start_time=$(date +%s.%N)"
        print "    ./cvrp_solver \"$instance_path\" \"$instance_name\" \"$method\" &"
        print "    pid=$!"
        print ""
        print "    peak_mem=0"
        print "    while kill -0 \"$pid\" 2> /dev/null; do"
        print "        mem=$(ps -o rss= -p \"$pid\")"
        print "        [[ \"$mem\" =~ ^[0-9]+$ ]] && (( mem > peak_mem )) && peak_mem=$mem"
        print "        sleep 0.1"
        print "    done"
        print ""
        print "    end_time=$(date +%s.%N)"
        print "    elapsed=$(echo \"$end_time - $start_time\" | bc)"
        print ""
        print "    {"
        print "        echo \"Instância: $instance_name\""
        print "        echo \"Método: $method\""
        print "        echo \"Tempo total (s): $elapsed\""
        print "        echo \"Memória máxima (KB): $peak_mem\""
        print "    } > \"$stats_file\""
        print "}"
        inserted = 1
        next
    }
    /\.\/cvrp_solver/ {
        split($0, a, " ")
        if (length(a) >= 4) {
            print "run_solver " a[2] " " a[3] " " a[4]
            next
        }
    }
    {
        print
    }
    ' "$script" > "updated_$script"

    mv "updated_$script" "$script"
    chmod +x "$script"
done

echo "✅ Todos os scripts foram atualizados com sucesso."
