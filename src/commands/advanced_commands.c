/**
 * @file advanced_commands.c
 * @brief Implementación de comandos avanzados: historial, limpiar, buscar, estadisticas
 */

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "commands.h"
#include "shell.h"

enum { LINE_BUF = 1024 };

static const char *CLR_RESET = "\033[0m";
static const char *CLR_YELLOW = "\033[1;33m";
static const char *CLR_GREEN = "\033[1;32m";
static const char *CLR_RED = "\033[1;31m";
static const char *CLR_CYAN = "\033[1;36m";

static void usage_buscar(void) {
  puts("Uso: buscar <texto> <archivo>");
  puts("Ejemplo: buscar main main.c");
  puts("Ejemplo: buscar \"main\" main.c");
}

static void usage_estadisticas(void) {
  puts("Uso: estadisticas <archivo>");
  puts("Ejemplo: estadisticas README.md");
}

static char *strip_quotes(char *s) {
  if (!s) return s;
  size_t n = strlen(s);
  if (n >= 2) {
    char a = s[0];
    char b = s[n - 1];
    if ((a == '"' && b == '"') || (a == '\'' && b == '\'')) {
      s[n - 1] = '\0';
      return s + 1;
    }
  }
  return s;
}

static void perm_to_rwx(mode_t mode, char out[10]) {
  const mode_t bits[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                          S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
  const char chars[3] = {'r', 'w', 'x'};

  for (int i = 0; i < 9; ++i) out[i] = (mode & bits[i]) ? chars[i % 3] : '-';
  out[9] = '\0';
}

static void fmt_time(time_t t, char *buf, size_t cap) {
  struct tm tm_info;
#if defined(_POSIX_THREAD_SAFE_FUNCTIONS)
  localtime_r(&t, &tm_info);
  strftime(buf, cap, "%d/%m/%Y %H:%M:%S", &tm_info);
#else
  struct tm *p = localtime(&t);
  if (!p) {
    snprintf(buf, cap, "N/A");
    return;
  }
  strftime(buf, cap, "%d/%m/%Y %H:%M:%S", p);
#endif
}

static void print_human_size(off_t bytes) {
  if (bytes >= (off_t)1024 * 1024) {
    printf("%lld bytes (%.2f MB)\n", (long long)bytes,
           (double)bytes / (1024.0 * 1024.0));
  } else if (bytes >= (off_t)1024) {
    printf("%lld bytes (%.2f KB)\n", (long long)bytes, (double)bytes / 1024.0);
  } else {
    printf("%lld bytes\n", (long long)bytes);
  }
}

static void print_open_error(const char *filename) {
  fprintf(stderr, "Error: No se pudo abrir '%s' (%s)\n", filename,
          strerror(errno));
}

// ============================================
// COMANDO: HISTORIAL
// ============================================

void cmd_historial(char **args) {
  (void)args;

  printf("\n=== HISTORIAL DE COMANDOS (últimos %d) ===\n", history_count);
  if (history_count <= 0) {
    puts("  El historial está vacío.\n");
    return;
  }

  for (int i = 0; i < history_count; ++i) {
    printf("  %2d: %s\n", i + 1, history[i]);
  }
  putchar('\n');
}

// ============================================
// COMANDO: LIMPIAR
// ============================================

void cmd_limpiar(char **args) {
  (void)args;

  // ANSI: limpia pantalla y mueve cursor al inicio
  fputs("\033[2J\033[H", stdout);

  fputs("╔══════════════════════════════════════╗\n"
        "║           EAFITos v1.0               ║\n"
        "║       Shell Educativa - SO           ║\n"
        "╚══════════════════════════════════════╝\n"
        "Escribe 'ayuda' para ver los comandos.\n\n",
        stdout);
}

// ============================================
// COMANDO: BUSCAR
// ============================================

void cmd_buscar(char **args) {
  if (!args || !args[1] || !args[2]) {
    usage_buscar();
    return;
  }

  char *search_text = strip_quotes(args[1]);
  const char *filename = args[2];

  FILE *file = fopen(filename, "r");
  if (!file) {
    print_open_error(filename);
    return;
  }

  char line[LINE_BUF];
  int line_num = 1;
  int matches = 0;

  printf("\nBuscando '%s' en %s:\n", search_text, filename);
  puts("────────────────────────────────────");

  while (fgets(line, sizeof(line), file)) {
    // Quitar salto final para imprimir bonito
    line[strcspn(line, "\n")] = '\0';

    if (strstr(line, search_text)) {
      printf("%s%4d:%s %s\n", CLR_YELLOW, line_num, CLR_RESET, line);
      ++matches;
    }
    ++line_num;
  }

  fclose(file);

  if (matches == 0) {
    printf("%s✗ Texto '%s' no encontrado.%s\n\n", CLR_RED, search_text,
           CLR_RESET);
  } else {
    printf("%s✓ Encontrado %d ocurrencia(s).%s\n\n", CLR_GREEN, matches,
           CLR_RESET);
  }
}

// ============================================
// COMANDO: ESTADISTICAS
// ============================================

void cmd_estadisticas(char **args) {
  if (!args || !args[1]) {
    usage_estadisticas();
    return;
  }

  const char *filename = args[1];
  struct stat st;

  if (stat(filename, &st) == -1) {
    fprintf(stderr, "Error: No se pudo acceder a '%s' (%s)\n", filename,
            strerror(errno));
    return;
  }

  printf("\n%s=== ESTADÍSTICAS DE: %s ===%s\n", CLR_CYAN, filename, CLR_RESET);
  puts("────────────────────────────────────");

  // 1) Tamaño
  printf("%sTamaño:%s        ", CLR_YELLOW, CLR_RESET);
  print_human_size(st.st_size);

  // 2) Tipo
  printf("%sTipo:%s          ", CLR_YELLOW, CLR_RESET);
  if (S_ISREG(st.st_mode))
    puts("Archivo regular");
  else if (S_ISDIR(st.st_mode))
    puts("Directorio");
  else if (S_ISLNK(st.st_mode))
    puts("Enlace simbólico");
  else
    puts("Otro");

  // 3) Permisos
  char rwx[10];
  perm_to_rwx(st.st_mode, rwx);
  printf("%sPermisos:%s      %o (%s)\n", CLR_YELLOW, CLR_RESET,
         (unsigned)(st.st_mode & 0777), rwx);

  // 4) Fechas
  char tb[64];
  fmt_time(st.st_mtime, tb, sizeof(tb));
  printf("%sModificado:%s    %s\n", CLR_YELLOW, CLR_RESET, tb);

  fmt_time(st.st_atime, tb, sizeof(tb));
  printf("%sAccedido:%s      %s\n", CLR_YELLOW, CLR_RESET, tb);

  // 5) Contenido (solo si es archivo regular y se puede leer)
  if (S_ISREG(st.st_mode)) {
    FILE *f = fopen(filename, "r");
    if (!f) {
      printf("⚠️  No se pudo leer el contenido para análisis (%s).\n",
             strerror(errno));
    } else {
      int lines = 0, words = 0;
      long long chars = 0;
      int in_word = 0;
      int c = 0, last = 0;

      while ((c = fgetc(f)) != EOF) {
        ++chars;
        if (c == '\n') ++lines;

        if (isspace((unsigned char)c)) {
          in_word = 0;
        } else if (!in_word) {
          in_word = 1;
          ++words;
        }
        last = c;
      }
      fclose(f);

      if (chars > 0 && last != '\n') ++lines; // cuenta última línea sin '\n'

      printf("%sLíneas:%s        %d\n", CLR_YELLOW, CLR_RESET, lines);
      printf("%sPalabras:%s      %d\n", CLR_YELLOW, CLR_RESET, words);
      printf("%sCaracteres:%s    %lld\n", CLR_YELLOW, CLR_RESET, chars);
    }
  }

  puts("────────────────────────────────────\n");
}