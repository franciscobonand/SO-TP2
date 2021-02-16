#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void main(int argc, char* argv[]){
  clock_t start, end;
  start = clock();

  if (argc != 5) {
    printf("Número incorreto de parâmetros.\n");
    printf("tp2virtual [algoritmo] arquivo.log 4 128\n");
    exit(1);
  }

  char subs_algorithm[10];
  strcpy(subs_algorithm, argv[1]);

  char file_path[50]; // 50 because it can be the complete path to a file
  strcpy(file_path, argv[2]);

  int page_size = strtol(argv[3], NULL, 10); // page size in kilobytes

  int mem_size = strtol(argv[4], NULL, 10); // memory size in kilobytes (available to the process)

  // Implement factory for substitution algorithm?
  if (strcmp(subs_algorithm, "lru") == 0) {
    printf("lru!\n");
  } else if (strcmp(subs_algorithm, "2a") == 0) {
    printf("2a!\n");
  } else if (strcmp(subs_algorithm, "fifo") == 0) {
    printf("fifo!\n");
  } else if (strcmp(subs_algorithm, "other") == 0) {
    printf("some other algorithm!\n");
  } else {
    printf("Algoritmo de substituição não encontrado.\n");
  }

  printf("Arquivo de entrada: %s\n", file_path);
  printf("Tamanho das páginas: %d\n", page_size);
  printf("Tamanho da memória: %d\n", mem_size);

  FILE *log_file;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  log_file = fopen(file_path, "r");

  if (log_file == NULL){
    printf("Erro ao abrir o arquivo %s", file_path);
    exit(1);
  }

  printf("Reading file %s\n", file_path);
  while ((read = getline(&line, &len, log_file)) != -1) {
    printf("%s", line);
  }

  fclose(log_file);

  end = clock();
  printf("Tempo gasto: %f\n", ((double) (end-start) / CLOCKS_PER_SEC));
}
