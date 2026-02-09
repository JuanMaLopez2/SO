# EAFITos 🎓

**EAFITos** es un sistema operativo educativo diseñado para aprender los fundamentos de la programación de sistemas y la operación interna de una shell utilizando el lenguaje C.

## 🚀 Objetivos
- Entender la **gestión de procesos** y memoria.
- Aprender sobre **hilos (threads)** y concurrencia.
- Explorar el **sistema de archivos** y llamadas al sistema (syscalls).
- Implementar **estructuras de datos básicas** usadas en kernels.

## 🐚 La Shell de EAFITos
El sistema inicia con una interfaz de línea de comandos (Shell) básica que permite interactuar con el sistema.

### ¿Qué son los Argumentos (`args`)?
En una shell, cuando escribes un comando, a menudo necesitas enviarle información adicional. Esta información se divide en "argumentos".

Internamente en C, esto se maneja mediante un arreglo de cadenas (`char **args`):
- **`args[0]`**: Es siempre el nombre del comando (ej. `calc`).
- **`args[1]`, `args[2]`, etc**: Son los parámetros que le pasas al comando.

**Ejemplo en el comando `calc 10 + 5`:**
- `args[0]` -> `"calc"`
- `args[1]` -> `"10"`
- `args[2]` -> `"+"`
- `args[3]` -> `"5"`

### Comandos Disponibles

| Comando | Argumentos | Descripción | Ejemplo |
| :--- | :--- | :--- | :--- |
| `listar` | Ninguno | Muestra los archivos del directorio actual. | `listar` |
| `leer` | `<archivo>` | Muestra el contenido de un archivo de texto. | `leer README.md` |
| `tiempo` | Ninguno | Muestra la fecha y hora actual del sistema. | `tiempo` |
| `calc` | `<n1> <op> <n2>` | Realiza operaciones aritméticas (+, -, *, /). | `calc 10 * 2.5` |
| `ayuda` | Ninguno | Muestra la lista de comandos disponibles. | `ayuda` |
| `salir` | Ninguno | Termina la sesión de EAFITos. | `salir` |

## 🛠️ Estructura del Proyecto
- `/src`: Código fuente del proyecto (`main.c` contiene el loop principal).
- `Makefile`: Script para automatizar la compilación.

## ⚡ Cómo compilar y ejecutar
1. **Compilar**: Ejecuta `make` en la terminal.
   ```bash
   make
   ```
2. **Ejecutar**: Inicia el sistema.
   ```bash
   make run
   # O directamente:
   ./build/sistema_os
   ```

## 📚 Documentación

El proyecto incluye un sistema de autodocumentación basado en **Doxygen**. Esto permite generar un sitio web técnico a partir de los comentarios explicativos en el código fuente.

### ¿Qué es Doxyfile?
El archivo `Doxyfile` contiene la configuración necesaria para que Doxygen entienda cómo analizar nuestro código (C), dónde buscar los archivos y en qué formato generar la salida (HTML).

### Cómo generar la documentación
Si tienes Doxygen instalado en tu sistema:
1. Asegúrate de estar en la raíz del proyecto.
2. Ejecuta el comando:
   ```bash
   doxygen Doxyfile
   ```
3. Esto creará una carpeta `docs/html`. Abre el archivo `docs/html/index.html` en tu navegador para navegar por la documentación interactiva de funciones y estructuras.

