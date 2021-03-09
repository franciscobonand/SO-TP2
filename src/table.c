#include <stdio.h>
#include <stdlib.h>
#include<time.h>
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
    int elapsedTimeSinceAcc;

    for(int i=0; i < tableSize; i++) {
      elapsedTimeSinceAcc = clock - pages[i].timeSinceAcc;
      if(max < elapsedTimeSinceAcc) {
        max = elapsedTimeSinceAcc;
        lru = i;
      }
    }
    return lru;
}

int returnRandom(int luIndx, int tableSize) {
    clock_t start;
    start = clock();
    srand(start);
    int numb = rand() % 3;
    numb = (luIndx + numb) % tableSize;
    return numb;
}

// updateMemory reads/writes a page with a given algorithm on a table
void updateMemory(int op, int alg, Page* page, Table* table) {
    int pageInTable;
    int index;

    table->clock++;
    page->timeSinceAcc = table->clock;


    alreadyExists(page->address, table, &pageInTable, &index, alg);

    if (!pageInTable && table->currOccupancy < table->size) {
      // Table has space available for new pages: insert page in table

      table->pagefaults++;

      table->pages[table->currOccupancy] = *page;
      table->currOccupancy++;
    }
    else if (!pageInTable && table->currOccupancy == table->size) {
      // Table is full: insert page in table according to algorithm
      table->pagefaults++;

      if (table->pages[index].isDirty){
        table->dirtyPages++;
      }

      table->pages[index] = *page;
    }
    else {
        // Page is in table and it's bein accessed again
        table->pages[index].nxtVictim = 1;
    }
    // Already in table: just set to dirty in case of write
    if (op == WRITE) {
      table->pages[index].isDirty = 1;
    }

}

// alreadyExists checks if a page is in a table
// pageInTable is 1 if the page is in the table, and 0 otherwise
// index is the int where the new page should be inserted
void alreadyExists(int pageAddr, Table* table, int* pageInTable, int* index, int alg) {
    for(int i=0; i < table->currOccupancy; i++) {
        if (pageAddr == table->pages[i].address) {
            *pageInTable = 1;
            *index = i;
            return;
        }
    }

    *pageInTable = 0;

    // table NOT FULL
    if (table->currOccupancy < table->size) {
        *index = table->currOccupancy-1;
    } 
    // table FULL
    else {
        switch (alg) {
          case 1: // lru
              *index = getLRU(table->pages, table->clock, table->size);
              break;

          case 2: // 2a
              for(int i = table->luIndex; i < table->size; i++) {
                  if(!table->pages[i].nxtVictim) {
                      *index = i;
                      table->luIndex++;
                      table->luIndex = table->luIndex % table->size;

                      break;
                  } else {
                      table->pages[i].nxtVictim = 0;
                  }

                  if (i == table->size - 1) {
                      i = 0;
                  }
              }
              break;

          case 3: // fifo
              *index = table->luIndex;
              table->luIndex++;
              table->luIndex = table->luIndex % table->size;
              break;

          default: // rand
              *index = returnRandom(table->luIndex, table->size);
              table->luIndex++;
              table->luIndex = table->luIndex % table->size;
              break;
        }
    }
};
