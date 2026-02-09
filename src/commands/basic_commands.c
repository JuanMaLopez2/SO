/**
 * @file basic_commands.c
 * @brief Implementación de comandos básicos del sistema.
 */

#include <stdio.h>  // printf
#include <stdlib.h> // exit, malloc
#include <time.h>   // time, localtime, strftime
#include "commands.h"

/**
 * @brief Comando AYUDA
 * 
 * Muestra al usuario la lista de acciones que puede realizar.
 * Es fundamental para la usabilidad de la shell.
 * 
 * @param args Argumentos del comando (no se usan aquí, por eso (void)args).
 */
void cmd_ayuda(char **args) {
    printf("--- Ayuda de Shell Educativa ---\n");
    printf("Comandos disponibles:\n");
    printf("  - listar: Muestra archivos del directorio.\n");
    printf("  - leer <archivo>: Muestra el contenido de un archivo.\n");
    printf("  - tiempo: Muestra la fecha y hora actual.\n");
    printf("  - calc <n1> <op> <n2>: Realiza cálculos simples.\n");
    printf("  - ayuda: Muestra este mensaje.\n");
    printf("  - salir: Termina la sesión.\n");
    
    // Silenciar advertencia de compilador sobre variable no usada
    (void)args;
}

/**
 * @brief Comando SALIR
 * 
 * Finaliza la ejecución del programa de forma controlada.
 * Utiliza la llamada al sistema exit().
 * 
 * @param args Argumentos del comando (ignorados).
 */
void cmd_salir(char **args) {
    printf("Saliendo de la shell...\n");
    // exit(0) devuelve el código 0 al sistema operativo padre,
    // indicando que el proceso terminó sin errores.
    exit(0);
    (void)args;
}

/**
 * @brief Comando TIEMPO (date)
 * 
 * Obtiene y formatea la fecha y hora del sistema.
 * Demuestra el uso de la librería estándar <time.h>.
 * 
 * @param args Argumentos del comando (ignorados).
 */
void cmd_tiempo(char **args) {
    // 1. Obtener tiempo "crudo" (segundos desde 1970 - Epoch Unix)
    time_t t = time(NULL);
    
    // 2. Convertir a una estructura estructurada local (struct tm)
    // localtime devuelve un puntero, así que desreferenciamos con *
    struct tm tm = *localtime(&t);
    
    // 3. Imprimir con formato
    // tm_year cuenta desde 1900, por eso sumamos 1900.
    // tm_mon es 0-11, por eso sumamos 1.
    printf("Fecha y Hora del Sistema: %02d-%02d-%04d %02d:%02d:%02d\n",
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
           tm.tm_hour, tm.tm_min, tm.tm_sec);
           
    (void)args;
}
