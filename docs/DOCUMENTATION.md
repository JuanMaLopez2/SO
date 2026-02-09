# 📚 Guía de Documentación con Doxygen

Este proyecto utiliza el estándar **Doxygen** para documentar el código fuente. Esto nos permite generar automáticamente manuales en HTML y PDF a partir de los comentarios en el código.

## 🏷️ Etiquetas Principales

Utilizamos bloques de comentarios especiales `/** ... */` antes de funciones, archivos y estructuras.

| Etiqueta | Descripción | Ejemplo |
| :--- | :--- | :--- |
| **@file** | Nombre y propósito del archivo. | `@file main.c` |
| **@brief** | Resumen corto de la funcionalidad. | `@brief Inicializa el sistema.` |
| **@param** | Describe un parámetro de función. | `@param args Lista de argumentos.` |
| **@return** | Describe el valor de retorno. | `@return 0 si fue exitoso.` |
| **@note** | Notas adicionales importantes. | `@note Requiere memoria dinámica.` |

## 📝 Ejemplo de Código Documentado

```c
/**
 * @brief Suma dos números enteros.
 * 
 * Esta función toma dos enteros y devuelve su suma. 
 * Maneja desbordamiento de enteros verificando los límites.
 * 
 * @param a Primer operando.
 * @param b Segundo operando.
 * @return int El resultado de la suma (a + b).
 */
int sumar(int a, int b) {
    return a + b;
}
```

## 🚀 Generar la Documentación

### Prerrequisitos
Debes tener instalado **Doxygen** y **Graphviz** (opcional, para diagramas).

```bash
# Ubuntu/Debian
sudo apt-get install doxygen graphviz
```

### Pasos
1. Ubícate en la raíz del proyecto.
2. Ejecuta el comando:
   ```bash
   doxygen Doxyfile
   ```
3. Abre el archivo generado: `docs/html/index.html` en tu navegador.

## ⚙️ Configuración (Doxyfile)

El archivo `Doxyfile` en la raíz controla cómo se genera la documentación. Algunas opciones clave:
- `PROJECT_NAME`: "EAFITos"
- `INPUT`: Directorios a escanear (`src/` `include/`)
- `RECURSIVE`: YES (para buscar en subcarpetas)
- `OUTPUT_DIRECTORY`: `docs/`
