#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/table.h"

unsigned getPageShitfBit(int p_size) {
  unsigned s, page;
  s = 10;
  while (p_size>1) {
    p_size = p_size>>1;
    s++;
  }
  return s;
}


void printTable(Table* table){
  printf("Endereço lógico  | Dirty bit \n");
  for(int i=0; i < table->currOccupancy; i++) {
    printf("\t %lx \t | \t %d\n", table->pages[i].address, table->pages[i].isDirty);
  }
}

void main(int argc, char* argv[]){
  clock_t start, end;

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
  int table_size = mem_size / page_size; //table size in number of pages

  int algorithm;

  printf("Arquivo de entrada: %s\n", file_path);
  printf("Tamanho das páginas: %d\n", page_size);
  printf("Tamanho da memória: %d\n", mem_size);

  if (strcmp(subs_algorithm, "lru") == 0) {
    printf("Algoritmo: lru\n");
    algorithm = 1;
  } else if (strcmp(subs_algorithm, "2a") == 0) {
    printf("Algoritmo: 2a\n");
    algorithm = 2;
  } else if (strcmp(subs_algorithm, "fifo") == 0) {
    printf("Algoritmo: fifo\n");
    algorithm = 3;
  } else if (strcmp(subs_algorithm, "other") == 0) {
    printf("Algoritmo: outro\n");
    algorithm = 4;
  } else {
    printf("Algoritmo de substituição não encontrado.\n");
  }

  FILE *log_file;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  log_file = fopen(file_path, "r");

  if (log_file == NULL){
    printf("Erro ao abrir o arquivo %s", file_path);
    exit(1);
  }

  Page* auxPage = newPage(); // Creates an auxiliar page structure to receive values in each iteration
  Table* tableMem = initializeTable(table_size); // Creates a table to store pages
  unsigned shift = getPageShitfBit(page_size); // Gets shitf bit to calculate pages address

  printf("Reading file %s\n", file_path);
  unsigned addr;
  char rw;

  start = clock();
  while (fscanf(log_file, "%x %c", &addr, &rw) != EOF) {
    auxPage->address = addr >> shift;

    if(rw == 'W') {
      // printf("Write %ld\n", auxPage->address);
      updateMemory(WRITE, algorithm, auxPage, tableMem);
    }
    else if(rw == 'R') {
      // printf("Read  %ld\n", auxPage->address);
      updateMemory(READ, algorithm, auxPage, tableMem);
    }
    else {
      printf("Invalid command for page %ld\n", auxPage->address);
    }
  }
  end = clock();

  fclose(log_file);

  printf("Paginas lidas: %d\n", tableMem->pagefaults);
  printf("Paginas escritas: %d\n", tableMem->dirtyPages);
  printf("Tempo gasto: %f\n", ((double) (end-start) / CLOCKS_PER_SEC));
  printTable(tableMem);

}
