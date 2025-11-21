#!/bin/bash

# Script principal a ser executado
PROGRAM="./run_all.sh"

# Núcleos de CPU que deseja usar (ex: 2,3)
CORES="2,3"

# Nome dos arquivos de log
LOG_FILE="exec_all_log.txt"
STATS_FILE="exec_all_stats.txt"

# 1. Parar serviços desnecessários
echo "[INFO] Parando serviços desnecessários..."
sudo service cron stop 2>/dev/null
sudo service cups stop 2>/dev/null
sudo service snapd stop 2>/dev/null
sudo service rsyslog stop 2>/dev/null

# 2. Mostrar serviços restantes
echo "[INFO] Serviços restantes:"
service --status-all | grep '+'

# 3. Limpar logs antigos
rm -f "$LOG_FILE" "$STATS_FILE"

# 4. Executar script e medir tempo/memória
echo "[INFO] Executando '$PROGRAM' com prioridade máxima nas CPUs: $CORES"
echo "[INFO] Log será salvo em: $LOG_FILE"
echo "[INFO] Estatísticas serão salvas em: $STATS_FILE"

start_time=$(date +%s.%N)

# Executa o script principal normalmente em background
taskset -c "$CORES" nice -n -20 "$PROGRAM" > "$LOG_FILE" &
pid=$!

# Medir uso de memória durante a execução
peak_mem=0
while kill -0 "$pid" 2> /dev/null; do
    mem=$(ps -o rss= -p "$pid")
    [[ "$mem" =~ ^[0-9]+$ ]] && (( mem > peak_mem )) && peak_mem=$mem
    sleep 0.1
done

end_time=$(date +%s.%N)
elapsed=$(echo "$end_time - $start_time" | bc)

# Salvar estatísticas
{
    echo "Programa: $PROGRAM"
    echo "Tempo total (s): $elapsed"
    echo "Memória máxima (KB): $peak_mem"
} > "$STATS_FILE"
