/**
 * @file main.c
 * @brief Punto de entrada principal del sistema operativo educativo EAFITos.
 *
 * Este archivo contiene la función main(), que es el lugar donde comienza
 * la ejecución del programa. Su única responsabilidad es inicializar el mensaje
 * de bienvenida y ceder el control al bucle principal de la shell.
 */

/* 
 * --- Librerías Estándar ---
 * <stdio.h>: Standard Input/Output. 
 * Provee funcionalidades para operaciones de entrada y salida..
 * En este caso, se usa para la función printf() que imprime texto en la terminal.
 */
#include <stdio.h>

/*
 * --- Cabeceras Propias ---
 * "shell.h": Incluye los prototipos de las funciones principales del núcleo,
 * como loop_shell(), permitiendo que main() conozca su existencia.
 */
#include "shell.h"

/**
 * @brief Función principal del programa.
 * 
 * En C, la ejecución siempre comienza en la función main.
 * 
 * @return int Retorna 0 al sistema operativo para indicar que el programa
 *         terminó correctamente (EXIT_SUCCESS).
 */
int main() {
    // Imprime el mensaje de bienvenida a la salida estándar (stdout)
    printf("Iniciando EAFITos v1.0...\n");
    printf("Escribe 'ayuda' para comenzar.\n\n");
    
    // Llama al bucle principal de la shell ubicado en src/core/shell_loop.c.
    // Esta función no retornará hasta que el usuario decida salir.
    loop_shell();

    // Retornamos 0 para indicar una finalización exitosa.
    return 0;
}
