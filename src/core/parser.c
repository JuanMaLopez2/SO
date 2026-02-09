/**
 * @file parser.c
 * @brief Módulo encargado de interpretar la entrada del usuario.
 *
 * Contiene funciones para leer texto desde la consola y convertirlo
 * en una estructura de datos (lista de argumentos) que el sistema pueda procesar.
 */

#include <stdio.h>  // Para getline, perror, fprintf, stdin
#include <stdlib.h> // Para malloc, realloc, free, exit
#include <string.h> // Para strtok
#include "shell.h"  // Definiciones globales como DELIM

/**
 * @brief Lee una línea completa de texto desde la entrada estándar (teclado).
 * 
 * Utiliza la función getline(), que es una extensión segura de GNU C library.
 * getline() maneja automáticamente la asignación de memoria: si la línea es
 * demasiado larga para el buffer, getline() lo redimensiona usando realloc().
 * 
 * @return char* Puntero a la cadena de caracteres (string) leída.
 *         IMPORTANTE: El llamador es responsable de liberar (free) esta memoria.
 */
char *leer_linea(void) {
    char *linea = NULL;
    size_t bufsize = 0; // getline asignará el tamaño necesario

    // getline(&buffer, &tamaño, stream)
    // Lee hasta encontrar un salto de línea o EOF (End Of File)
    if (getline(&linea, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            // Se encontró EOF (Ctrl+D), salimos elegantemente
            exit(EXIT_SUCCESS);
        } else {
            // Ocurrió un error real
            perror("Error al leer línea");
            exit(EXIT_FAILURE);
        }
    }
    return linea;
}

/**
 * @brief Divide una cadena de texto en tokens individuales (argumentos).
 * 
 * Esta función implementa el análisis léxico básico. Transforma:
 * "calc 10 + 20"  --->  ["calc", "10", "+", "20", NULL]
 * 
 * Utiliza memoria dinámica porque no sabemos a priori cuántos argumentos
 * introducirá el usuario.
 * 
 * @param linea La cadena de texto cruda leída anteriormente.
 * @return char** Un arreglo de cadenas (doble puntero) terminado en NULL.
 */
char **parsear_linea(char *linea) {
    int bufsize = 64; // Tamaño inicial arbitrario del buffer de tokens
    int posicion = 0;
    
    // malloc: Asigna un bloque de memoria de (64 * tamaño_puntero) bytes.
    // char** tokens es, en esencia, una lista de punteros a string.
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Error de asignación de memoria (malloc falló)\n");
        exit(EXIT_FAILURE);
    }

    // strtok: Divide el string 'linea' usando los delimitadores (espacio, tab, etc.)
    // La primera llamada toma la cadena; las siguientes con NULL continúan parseando la misma cadena.
    token = strtok(linea, DELIM);
    while (token != NULL) {
        tokens[posicion] = token;
        posicion++;

        // Si tenemos más argumentos que el tamaño de nuestro buffer...
        if (posicion >= bufsize) {
            bufsize += 64; // Aumentamos el tamaño
            // realloc: Intenta redimensionar el bloque de memoria existente,
            // preservando el contenido anterior.
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Error de reasignación de memoria (realloc falló)\n");
                exit(EXIT_FAILURE);
            }
        }

        // Obtener el siguiente token
        token = strtok(NULL, DELIM);
    }
    
    // Lista terminada en NULL: Convención estándar en C para indicar el fin de un arreglo de punteros.
    // Esencial para que las funciones que usen 'args' sepan dónde parar.
    tokens[posicion] = NULL;
    return tokens;
}
