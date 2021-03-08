#include <stdio.h>
#include <stdlib.h>
#include "../lib/table.h"

Page* newPage() {
    Page* newPage = malloc(sizeof(Page));
    newPage->address = -1;
    newPage->nxtVictim = 0;
    newPage->timeSinceAcc = 0;
    newPage->isDirty = 0;
    return newPage;
};

Table* initializeTable(int size) { 
  Table* newTable = malloc(sizeof(Table));
  newTable->pages = (Page*) calloc(size, sizeof(Page));
  newTable->mrvIndex = (int*) calloc(size, sizeof(int));
  newTable->luIndex = 0;
  newTable->size = size;
  newTable->currOccupancy = 0;
  newTable->clock = 0;
  newTable->pagefaults = 0;
  newTable->dirtyPages = 0;
  return newTable;
}

int getLRU(Page* pages, int clock, int tableSize) {
    int lru;
    int max = 0;
    for(int i=0; i<tableSize; i++) {
        if(max < (pages[i].timeSinceAcc - clock)) {
            max = pages[i].timeSinceAcc;
            lru = i;
        }
    }
    return lru;
}

// updateMemory reads/writes a page with a given algorithm on a table
void updateMemory(int op, int alg, Page* page, Table* table) {
    printf("here");
    int* pageInTable;
    int* index;

    table->clock++;
    page->timeSinceAcc = table->clock;


    alreadyExists(page->address, table, pageInTable, index, alg);

    printf("%d", *pageInTable);
    if (!pageInTable && table->currOccupancy < table->size) {
      // Table has space available for new pages: insert page in table
      if (table->currOccupancy == 0) {
        table->luIndex = *(index);
      }

      table->pagefaults++;

      table->pages[table->currOccupancy] = *page;
      table->currOccupancy++;
    }
    else if (!pageInTable && table->currOccupancy == table->size) {
      // Table is full: insert page in table according to algorithm
      // Paginas lidas == pagefault?
      table->pagefaults++;

      if (table->pages[*index].isDirty){
        table->dirtyPages++;
      }

      table->pages[*index] = *page;
    }

    // Already in table: just set to dirty in case of write
    if (op == WRITE) {
      table->pages[*index].isDirty = 1;
    }

}

// alreadyExists checks if a page is in a table
// pageInTable is 1 if the page is in the table, and 0 otherwise
// index is the int where the new page should be inserted
void alreadyExists(int pageAddr, Table* table, int* pageInTable, int* index, int alg) {
    int t = 1; int f = 0;

    for(int i=0; i<table->currOccupancy; i++) {
        if (pageAddr == table->pages[i].address) {
            pageInTable = &t;
            index = &i;
            break;
        }
    }

    int auxIndex;
    pageInTable = &f;

    // table NOT FULL
    if (table->currOccupancy < table->size) {
        auxIndex = table->currOccupancy-1;
        index = &auxIndex;
    } 
    // table FULL
    else {
        switch (alg) {
          case 1: // lru
              auxIndex = getLRU(table->pages, table->clock, table->size);
              index = &auxIndex;
              break;

          case 2: // 2a
              for(int i=table->luIndex; i<table->size; i++) {
                  if(table->pages[i].nxtVictim) {
                      auxIndex = i;
                      index = &auxIndex;
                      table->luIndex = (table->luIndex+1) % table->size;

                      break;
                  } else {
                      table->pages[i].nxtVictim = 1;
                  }

                  if (i == table->size - 1) {
                      i = 0;
                  }
              }
              break;

          case 3: // fifo
              auxIndex = table->luIndex;
              table->luIndex = (table->luIndex+1) % table->size;
              index = &auxIndex;
              break;

          default: // other
              printf("Other algorithm\n");
              break;
        }
    }
};
