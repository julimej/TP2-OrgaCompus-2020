#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "cache.h"

/**
 * Imprime la ayuda del programa por stderr.
 */
void print_help()
{
  fprintf(stderr,
          "Simulador de caché\n"
          "Uso: \n"
          "./tp2 [-v|--verbose] <file.mem>\n"
          "\n"
          "-v --verbose: Muestra el detalle de cada lectura/escritura\n");
}

/**
 * Compara un argumento contra su forma corta y su forma larga. 
 * 
 * Devuelve true si el argumento coincide con alguna de ellas.
 */
bool arg_compare(const char *arg, const char *shortstr, const char *longstr)
{
  return (strcmp(arg, shortstr) == 0) || (strcmp(arg, longstr) == 0);
}

/**
 * Procesa los argumentos de línea de comando.
 * 
 * Si no se recibe un nombre de archivo o si los argumentos son inválidos 
 * devuelve falso.
 * 
 * En caso contrario devuelve verdadero y almacena los argumentos recibidos
 * en los parámetros verbose y filename.
 */
bool process_argv(int argc, char *argv[], bool *verbose, char **filename)
{
  if (argc == 2)
  {
    *filename = argv[1];
    *verbose = false;
    return true;
  }
  else if (argc == 3)
  {
    if (!arg_compare(argv[1], "-v", "--verbose"))
      return false;

    *verbose = true;
    *filename = argv[2];
    return true;
  }

  return false;
}

/**
 * Simulador de caché.
 * 
 * Uso:
 * ./tp2 [-v|--verbose] <file.mem>
 * 
 * -v --verbose: Muestra el detalle de cada lectura/escritura.
 */
int main(int argc, char *argv[]) {
  bool verbose = false;
  char *filename = NULL;
  FILE *input;
  memory_t *memory = calloc(1, sizeof(memory_t));
  cache = calloc(1, sizeof(cache_t));
  memory_create(memory);
  create_cache(memory);
  if (!process_argv(argc, argv, &verbose, &filename)) {
    print_help();
    return EXIT_FAILURE;
  }

  if (strcmp(filename, "-") == 0)
    input = stdin;
  else
    input = fopen(filename, "r");

  if (!input) {
    fprintf(stderr,
            "ERROR: No se pudo abrir %s\n", filename);
    return EXIT_FAILURE;
  }

  char buffer[8192];
  while (fgets(buffer, sizeof(buffer), input)) {
    unsigned int addr;
    unsigned char value;
    command_t cmd = input_process_line(buffer, &addr, &value);
    switch (cmd)
    {
    case CMD_FLUSH:
      if (verbose)
        printf("flush\n");
      init();
      break;
    case CMD_R:
      if (verbose)
        printf("read addr %d\n", addr);
      unsigned char read = (unsigned char)read_byte(addr);
      printf("read %d\n",read);
      break;
    case CMD_W:
      if (verbose)
        printf("write value %d to addr %d\n", value, addr);
      write_byte(addr, value);
      break;
    case CMD_MR:
      if (verbose)
        printf("miss rate: ");
      printf("%.2f\n", get_miss_rate());
      break;
    default:
      fprintf(stderr, "ERROR: Comando no reconocido. Abortando\n");
      if (input != stdin)
        fclose(input);
      return EXIT_FAILURE;
    }
  }

  if (input != stdin)
    fclose(input);
  memory_destroy(memory);
  destroy_cache(cache);
  free(memory);
  free(cache);
  return EXIT_SUCCESS;
}
