# ==============================================================================
# Makefile para EAFITos
# ==============================================================================
#
# Este archivo define las reglas para compilar el código fuente C de manera automática.
# 'make' lee estas instrucciones para saber qué archivos recompilar y cómo enlazarlos.
#
# Variables clave:
# - CC: El compilador a usar (gcc).
# - CFLAGS: Opciones (flags) pasadas al compilador.
#   -Wall -Wextra: Activa la mayoría de advertencias para detectar errores.
#   -Iinclude: Indica que busque archivos .h en la carpeta 'include/'.
# ==============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Directorios de trabajo
SRC_DIR = src
BUILD_DIR = build

# Nombre del ejecutable final
TARGET = $(BUILD_DIR)/sistema_os

# ------------------------------------------------------------------------------
# Búsqueda de Archivos Fuente
# ------------------------------------------------------------------------------
# $(wildcard patron): Busca archivos que coincidan con el patrón.
# Aquí listamos explícitamente las subcarpetas de 'src' para encontrar todos los .c.
# Si añades una nueva carpeta en src, debes agregarla aquí.
SRCS = $(wildcard $(SRC_DIR)/core/*.c) \
       $(wildcard $(SRC_DIR)/commands/*.c) \
       $(wildcard $(SRC_DIR)/utils/*.c)

# ------------------------------------------------------------------------------
# Generación de la Lista de Objetos (.o)
# ------------------------------------------------------------------------------
# Transformamos la lista de fuentes (src/main.c) en una lista de objetos (build/main.o).
# Sintaxis: $(VARIABLE:patron_viejo=patron_nuevo)
# Esto nos permite saber qué archivos .o esperamos generar.
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# ------------------------------------------------------------------------------
# Reglas Especiales (.PHONY)
# ------------------------------------------------------------------------------
# .PHONY indica que estos objetivos no son archivos reales.
# 'all', 'clean' y 'run' son acciones, no archivos a crear.
.PHONY: all clean run

# Regla por defecto (la primera que ve make). Construye el ejecutable.
all: $(TARGET)

# ------------------------------------------------------------------------------
# Regla de Enlazado (Linking)
# ------------------------------------------------------------------------------
# Crea el ejecutable final $(TARGET) a partir de todos los objetos $(OBJS).
# $@ : Se expande al nombre del objetivo (TARGET).
# $^ : Se expande a todas las dependencias (OBJS).
# @mkdir -p: Asegura que el directorio 'build' exista.
$(TARGET): $(OBJS)
	@echo "🔗 Enlazando ejecutable: $@"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

# ------------------------------------------------------------------------------
# Regla de Compilación (Pattern Rule)
# ------------------------------------------------------------------------------
# Define cómo crear un archivo .o (en build) a partir de un .c (en src).
# % : Comodín que coincide con el nombre del archivo (ej: 'core/main').
# $< : Se expande a la primera dependencia (el archivo .c fuente).
# -c : Solo compila, no enlaza.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "🔨 Compilando: $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# ------------------------------------------------------------------------------
# Utilidades
# ------------------------------------------------------------------------------

# Limpia los archivos generados (borra la carpeta build).
# Útil para asegurar una compilación limpia desde cero.
clean:
	@echo "🧹 Limpiando archivos compilados..."
	rm -rf $(BUILD_DIR)

# Compila (si es necesario) y ejecuta el programa.
run: all
	@echo "🚀 Ejecutando EAFITos..."
	./$(TARGET)
