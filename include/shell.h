/**
 * @file shell.h
 * @brief Definiciones y prototipos del núcleo de la Shell.
 *
 * Este archivo cabecera expone las funciones públicas del sistema central
 * para que puedan ser utilizadas por el punto de entrada (main) u otros módulos.
 */

#ifndef SHELL_H
#define SHELL_H

// Tamaño máximo del buffer de entrada (aunque getline maneja dinámicamente)
#define MAX_CMD_INPUT 1024

// Caracteres que separan los argumentos de un comando (espacio, tab, etc.)
#define DELIM " \t\r\n\a"

/**
 * @brief Inicia el bucle principal de la shell.
 * No retorna hasta que el comando 'salir' sea invocado.
 */
void loop_shell();

/**
 * @brief Lee una línea de la entrada estándar.
 * @return char* Puntero a la cadena leída (debe ser liberada con free).
 */
char *leer_linea(void);

/**
 * @brief Parsea una línea cruda en un arreglo de tokens.
 * @param linea Cadena de entrada.
 * @return char** Arreglo de cadenas terminado en NULL.
 */
char **parsear_linea(char *linea);

/**
 * @brief Orquesta la ejecución de un comando dado sus argumentos.
 * @param args Lista de argumentos.
 */
void ejecutar(char **args);

#endif
