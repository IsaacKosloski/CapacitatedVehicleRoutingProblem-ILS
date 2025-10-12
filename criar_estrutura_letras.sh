#!/bin/bash

# Caminho para o arquivo zip
ZIP_PATH="Benchmarks.zip"
EXTRACT_DIR="benchmarks_tmp"

# Diretório final
OUTPUT_DIR="Output"

# Extrair os arquivos
unzip -q "$ZIP_PATH" -d "$EXTRACT_DIR"

# Criar estrutura de diretórios
find "$EXTRACT_DIR" -type f -name "*.vrp" | while read -r filepath; do
    filename=$(basename "$filepath" .vrp)  # A-n32-k5
    first_letter=${filename:0:1}           # A

    mkdir -p "$OUTPUT_DIR/$first_letter/$filename"
done

echo "✅ Estrutura criada com sucesso em '$OUTPUT_DIR/'"
