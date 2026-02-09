#!/bin/bash
# ==============================================================================
# Script de Construcción (Build)
# ==============================================================================
#
# Descripción:
# Este script automatiza el proceso de compilación del proyecto EAFITos.
# Actúa como un "wrapper" o envoltorio sobre la herramienta 'make'.
#
# Uso:
#   ./scripts/build.sh
#
# Detalles Técnicos:
# - Invoca 'make' en el directorio raíz.
# - Si se pasan argumentos al script (ej: "clean" o "run"), estos se pasan
#   directamente a make.
#   Ejemplo: ./scripts/build.sh clean  --> ejecuta 'make clean'
#
# ==============================================================================

# Imprimimos qué está pasando para feedback al usuario
echo "🔨 Iniciando proceso de construcción..."

# Ejecutamos make.
# "$@" expande a todos los argumentos pasados al script (ej: clean, run).
make "$@"

# Verificamos si make tuvo éxito (código de salida 0)
if [ $? -eq 0 ]; then
    echo "✅ Construcción completada exitosamente."
else
    echo "❌ Error durante la construcción."
    exit 1
fi
