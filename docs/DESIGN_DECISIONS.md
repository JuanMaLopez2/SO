# 🎯 Decisiones de Diseño - Comandos Básicos y Avanzados

Este documento detalla las decisiones de diseño específicas implementadas en los **comandos básicos** y **comandos avanzados** de EAFITos, ubicados en `src/commands/basic_commands.c` y `src/commands/advanced_commands.c`.

---

## Comandos Básicos (`basic_commands.c`)

### 1. 🆘 Comando AYUDA (`cmd_ayuda`)

**Decisión de Diseño**: Mostrar información categorizada con colorización ANSI

**Características**:
- Divide los comandos en dos categorías: **BÁSICOS** y **AVANZADOS**
- Usa colores ANSI para mejorar legibilidad:
  - Cyan (`\033[1;36m`) para títulos
  - Amarillo (`\033[1;33m`) para categorías
  - Verde (`\033[1;32m`) para ejemplos

```c
printf("\033[1;33mCOMANDOS BÁSICOS:\033[0m\n");
printf("  - listar: Muestra archivos del directorio.\n");
printf("  - leer <archivo>: Muestra el contenido de un archivo.\n");
```

**Justificación**:
- Proporciona un punto de entrada claro para nuevos usuarios
- La colorización mejora la experiencia educativa sin dependencias externas
- Incluye ejemplos prácticos para fomentar exploración

---

### 2. 🚪 Comando SALIR (`cmd_salir`)

**Decisión de Diseño**: Terminación controlada del programa

**Implementación**:
```c
void cmd_salir(char **args) {
    printf("Saliendo de la shell...\n");
    exit(0);  // Código 0 = sin errores
    (void)args;
}
```

**Justificación**:
- Usa `exit(0)` para terminar correctamente, señalando al SO que no hubo error
- Suprime warnings del compilador con `(void)args` (parámetro no usado)
- Proporciona feedback visual al usuario

---

### 3. ⏰ Comando TIEMPO (`cmd_tiempo`)

**Decisión de Diseño**: Usar la librería estándar `time.h` con formato personalizado

**Implementación**:
```c
time_t t = time(NULL);  // Segundos desde Epoch (1970)
struct tm tm = *localtime(&t);  // Convertir a struct local

printf("Fecha y Hora del Sistema: %02d-%02d-%04d %02d:%02d:%02d\n",
       tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
       tm.tm_hour, tm.tm_min, tm.tm_sec);
```

**Decisiones Clave**:
- **Conversión de Epoch**: `time(NULL)` obtiene segundos desde 1970, necesario para convertir a fecha legible
- **Ajustes de `struct tm`**:
  - `tm_year` es relativo a 1900 → suma 1900
  - `tm_mon` es 0-11 → suma 1
  - `tm_mday` está en 1-31 (no requiere ajuste)
- **Formato %02d**: Asegura que números se muestren con dos dígitos (ej: 01, 02)

**Justificación**:
- Educativo: Demuestra cómo trabajar con la API de tiempo en C
- Estándar: Sigue las convenciones POSIX
- Preciso: Obtiene tiempo del kernel del sistema operativo

---

## Comandos Avanzados (`advanced_commands.c`)

### 4. 📜 Comando HISTORIAL (`cmd_historial`)

**Decisión de Diseño**: Almacenar historial en array global estático

**Implementación**:
```c
printf("\n=== HISTORIAL DE COMANDOS (últimos %d) ===\n", history_count);

for (int i = 0; i < history_count; i++) {
    printf("  %2d: %s\n", i + 1, history[i]);
}
```

**Variables Globales Utilizadas** (en `utils/history_manager.c`):
```c
#define MAX_HISTORY 100
char *history[MAX_HISTORY];  // Array de punteros
int history_count = 0;       // Contador
```

**Decisiones Clave**:
- **Array Globalizado**: Accesible desde cualquier módulo (`extern` en `commands.h`)
- **Límite Fijo (100)**: Predecible, educativo, evita complejidad dinámica
- **Numeración Legible**: `%2d` formato para alineación

**Justificación**:
- **Simplicidad**: Fácil de entender para estudiantes
- **Performance**: O(1) acceso, O(1) inserción
- **Educativo**: Demuestra variables globales y su visibilidad

---

### 5. 🎨 Comando LIMPIAR (`cmd_limpiar`)

**Decisión de Diseño**: Usar secuencias ANSI escape para compatibilidad multiplataforma

**Implementación**:
```c
printf("\033[2J");  // Limpiar toda la pantalla
printf("\033[H");   // Mover cursor a origen (1,1)

// Banner con bordes Unicode
printf("╔══════════════════════════════════════╗\n");
printf("║           EAFITos v1.0               ║\n");
printf("║       Shell Educativa - SO           ║\n");
printf("╚══════════════════════════════════════╝\n");
```

**Códigos ANSI Utilizados**:
| Código | Efecto |
|--------|--------|
| `\033[2J` | Limpiar pantalla |
| `\033[H` | Cursor a posición (1,1) |
| `\033[1;36m` | Cyan brillante |
| `\033[0m` | Reset atributos |

**Justificación**:
- **Portabilidad**: Funciona en Linux, macOS, WSL, terminales POSIX
- **Sin Dependencias**: No requiere librerías externas como ncurses
- **Educativo**: Enseña escape codes y control de terminal

---

### 6. 🔍 Comando BUSCAR (`cmd_buscar`)

**Decisión de Diseño**: Implementar versión simplificada y educativa de `grep`

**Implementación**:
```c
FILE *file = fopen(filename, "r");
if (file == NULL) {
    printf("Error: No se pudo abrir '%s'\n", filename);
    return;
}

char line[1024];
int line_num = 1;
int matches = 0;

while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';  // Eliminar salto de línea
    
    if (strstr(line, search_text) != NULL) {
        printf("\033[1;33m%4d:\033[0m %s\n", line_num, line);
        matches++;
    }
    line_num++;
}
fclose(file);
```

**Decisiones Clave**:
- **`strcspn()`**: Encuentra y elimina saltos de línea sin usar `strtok()` (destructivo)
- **`strstr()`**: Búsqueda case-sensitive (simple pero adecuada para educación)
- **Numeración de Líneas**: Formato `%4d` para alineación visual
- **Colorización**: Líneas encontradas en amarillo para destacarlas
- **Conteo de Coincidencias**: Reporta total al final

**Justificación**:
- **Funcionalidad Básica**: Cubre casos comunes sin complejidad de regex
- **Educativo**: Demuestra I/O de archivos, búsqueda de strings, contadores
- **UX Clara**: Números de línea facilitan localización

---

### 7. 📊 Comando ESTADÍSTICAS (`cmd_estadisticas`)

**Decisión de Diseño**: Combinar syscall `stat()` con análisis de contenido

**Implementación Modular**:

#### Parte 1: Información del Sistema de Archivos
```c
struct stat file_stat;
stat(filename, &file_stat);

// Tamaño
printf("\033[1;33mTamaño:\033[0m %ld bytes", file_stat.st_size);
if (file_stat.st_size > 1024) {
    printf(" (%.2f KB)", file_stat.st_size / 1024.0);
}
```

#### Parte 2: Análisis de Contenido
```c
FILE *file = fopen(filename, "r");
int lines = 0, words = 0, chars = 0;
int in_word = 0;
char c;

while ((c = fgetc(file)) != EOF) {
    chars++;
    if (c == '\n') lines++;
    
    if (isspace(c)) {
        in_word = 0;
    } else if (!in_word) {
        in_word = 1;
        words++;
    }
}
fclose(file);
```

#### Parte 3: Permisos en Formato Octal y rwx
```c
printf("\033[1;33mPermisos:\033[0m %o (", file_stat.st_mode & 0777);

// Formato rwx estilo ls -l
printf("%c", (file_stat.st_mode & S_IRUSR) ? 'r' : '-');
printf("%c", (file_stat.st_mode & S_IWUSR) ? 'w' : '-');
printf("%c", (file_stat.st_mode & S_IXUSR) ? 'x' : '-');
// ... grupos y otros
printf(")\n");
```

#### Parte 4: Fechas Formateadas
```c
char time_buffer[80];
struct tm *tm_info;

tm_info = localtime(&file_stat.st_mtime);
strftime(time_buffer, sizeof(time_buffer), "%d/%m/%Y %H:%M:%S", tm_info);
printf("\033[1;33mModificado:\033[0m %s\n", time_buffer);
```

**Decisiones Clave**:
- **Separación de Responsabilidades**: Stat para metadatos, lectura para contenido
- **Múltiples Unidades**: Bytes, KB, MB automáticamente
- **Máscara de Bits**: `file_stat.st_mode & 0777` para extraer permisos
- **Macros POSIX**: `S_IRUSR`, `S_IWUSR`, etc. para legibilidad
- **Buffer Fijo**: `char line[1024]` apropiado para la mayoría de casos

**Justificación**:
- **Educativo**: Demuestra struct stat, I/O de archivos, bitwise operations
- **Completo**: Información tanto de metadatos como de contenido
- **Robusto**: Manejo de errores explícito
- **Interfaz Clara**: Colorización por categoría

---

## Patrones de Diseño Aplicados

### Manejo de Errores Consistente
```c
if (args[1] == NULL) {
    printf("Uso: comando <argumento>\n");
    printf("Ejemplo: comando valor\n");
    return;
}
```
**Decisión**: Validar argumentos primero, mensajes de uso claros

### Colorización ANSI Selectiva
- **Títulos**: Cyan brillante (`\033[1;36m`)
- **Categorías**: Amarillo (`\033[1;33m`)
- **Éxito**: Verde (`\033[1;32m`)
- **Error**: Rojo (`\033[1;31m`)
- **Reset**: `\033[0m` siempre al final

### Supresión de Warnings
```c
(void)args;  // Parámetro requerido por interfaz pero no utilizado
```
**Decisión**: Explícito vs. comentarios vacíos o ignorar warnings

---

## Conclusión

Los comandos básicos y avanzados de EAFITos demuestran decisiones de diseño que priorizan:

✅ **Claridad**: Código fácil de leer y entender  
✅ **Educación**: Enseña conceptos de C y sistemas operativos  
✅ **Robustez**: Manejo de errores y validación de entrada  
✅ **UX**: Colorización, formato, mensajes útiles  
✅ **Portabilidad**: ANSI compatible, sin dependencias pesadas  

Estas decisiones las hacen ideales como herramienta de aprendizaje para estudiantes que exploran la implementación de shells y sistemas Unix.