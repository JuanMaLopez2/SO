#!/bin/bash
# ==============================================================================
# Script de Formato de Código
# ==============================================================================
#
# Descripción:
# Este script se encarga de estandarizar el estilo del código fuente (C y Headers).
# Utiliza herramientas como 'clang-format' para ajustar indentación, espacios
# y saltos de línea automáticamente.
#
# Objetivo:
# Mantener la consistencia en el proyecto, facilitando la lectura para todos
# los colaboradores.
#
# Uso:
#   ./scripts/format.sh
#
# Requisitos:
#   - clang-format debe estar instalado (sudo apt install clang-format)
#
# ==============================================================================

echo "🎨 Formateando código fuente..."

# Verificamos si clang-format está instalado
if ! command -v clang-format &> /dev/null; then
    echo "⚠️  ADVERTENCIA: clang-format no encontrado."
    echo "   Por favor instálalo para usar este script: sudo apt install clang-format"
    exit 1
fi

# Buscamos archivos .c y .h en src/ e include/ y aplicamos formato
# -style=Google: Usa el estilo de codificación de Google (común en C/C++).
# -i: Edita los archivos "in-place" (sobreescribe con el formato corregido).

echo "   Procesando archivos en src/..."
find src -name "*.c" -o -name "*.h" -exec clang-format -i -style=Google {} +

echo "   Procesando archivos en include/..."
find include -name "*.h" -exec clang-format -i -style=Google {} +

echo "✅ Formato aplicado correctamente."
