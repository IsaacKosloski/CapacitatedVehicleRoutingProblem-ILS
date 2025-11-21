#!/bin/bash
#!/bin/bash

# Caminho para o binário C++ que resolve o CVRP com ILS
PROGRAM="./run_all.sh"

# CPU cores que deseja usar (ex: 2 e 3)
CORES="2,3"

# Nome do log (opcional)
LOG_FILE="exec_ils_log.txt"

# 1. Finaliza serviços comuns que consomem recursos
echo "[INFO] Parando serviços desnecessários..."
sudo service cron stop 2>/dev/null
sudo service cups stop 2>/dev/null
sudo service snapd stop 2>/dev/null
sudo service rsyslog stop 2>/dev/null

# 2. Mostra os serviços ativos restantes (para conferência)
echo "[INFO] Serviços restantes:"
service --status-all | grep '+'

# 3. Executa o programa com prioridade máxima e afinidade de CPU
echo "[INFO] Executando '$PROGRAM' com prioridade máxima nas CPUs: $CORES"
echo "[INFO] Log será salvo em: $LOG_FILE"

# 4. Usa exec para substituir o shell, evitando qualquer sobrecarga
exec taskset -c $CORES nice -n -20 $PROGRAM | tee "$LOG_FILE"
