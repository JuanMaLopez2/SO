/**
 * @file shell_loop.c
 * @brief Corazón operativo de la shell (Lógica de control).
 *
 * Contiene el bucle principal (Read-Eval-Print Loop) y la lógica para
 * despachar (dispatch) la ejecución de comandos utilizando una tabla de búsqueda.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "commands.h"

/*
 * --- Registro de Comandos ---
 * Para evitar una larga cadena de 'if-else if-else', usamos dos arreglos paralelos:
 * 1. nombres_comandos: Lista de cadenas con los nombres reconocidos.
 * 2. func_comandos: Lista de punteros a funciones correspondientes.
 *
 * Esto facilita la extensibilidad: para añadir un comando, solo se agregan entradas aquí.
 */

char *nombres_comandos[] = {
    "listar",
    "leer",
    "tiempo",
    "calc",
    "ayuda",
    "salir"
};

/*
 * Puntero a función: void (*func)(char **)
 * Significa: "La dirección de una función que no retorna nada (void)
 * y recibe como parámetro un arreglo de cadenas (char **)".
 */
void (*func_comandos[]) (char **) = {
    &cmd_listar,
    &cmd_leer,
    &cmd_tiempo,
    &cmd_calc,
    &cmd_ayuda,
    &cmd_salir
};

/**
 * @brief Calcula dinámicamente cuántos comandos hay registrados.
 * 
 * @return int El número de comandos (tamaño del array / tamaño de un elemento ptr).
 */
int num_comandos() {
    return sizeof(nombres_comandos) / sizeof(char *);
}

/**
 * @brief Busca y ejecuta el comando solicitado por el usuario.
 * 
 * @param args Lista de argumentos parseados. args[0] es el nombre del comando.
 */
void ejecutar(char **args) {
    if (args[0] == NULL) {
        // El usuario presionó Enter sin escribir nada.
        return; 
    }

    // Recorremos el registro de comandos buscando una coincidencia.
    for (int i = 0; i < num_comandos(); i++) {
        // strcmp: Retorna 0 si dos cadenas son idénticas.
        if (strcmp(args[0], nombres_comandos[i]) == 0) {
            // ¡Coincidencia encontrada! Llamamos a la función a través del puntero.
            (*func_comandos[i])(args);
            return;
        }
    }

    // Si salimos del ciclo, el comando no existe.
    printf("Comando desconocido: %s\nEscribe 'ayuda' para ver los comandos.\n", args[0]);
}

/**
 * @brief Bucle principal Read-Eval-Print Loop (REPL).
 * 
 * Ciclo de vida:
 * 1. Read: Muestra el prompt "EAFITos>" y lee la entrada.
 * 2. Parse (Eval implícito): Divide la entrada en argumentos.
 * 3. Execute: Busca y corre el comando.
 * 4. Loop: Repite o libera memoria.
 */
void loop_shell() {
    char *linea;   // Almacenará la línea cruda
    char **args;   // Almacenará los tokens
    int status = 1; // Variable de control del bucle

    do {
        printf("EAFITos> ");
        
        // 1. Lectura
        linea = leer_linea();
        
        // 2. Parseo
        args = parsear_linea(linea);
        
        // 3. Ejecución
        ejecutar(args);
        
        // 4. Limpieza de memoria (Gestión manual requerida en C)
        free(linea); // Libera el buffer de getline
        free(args);  // Libera el arreglo de punteros (nota: no los strings individuales si son punteros a 'linea')
        
    } while (status); // Por ahora el loop es infinito hasta que cmd_salir hace exit(0)
}
