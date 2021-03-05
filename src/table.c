#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../lib/table.h"

Page* newPage() {
    Page* newPage = malloc(sizeof(Page));
    newPage->address = -1;
    newPage->value = -1;
    newPage->nxtVictim = 0;
    newPage->timeSinceAcc = 0;
    return newPage;
};

Table* initializeTable(int size) { 
  Table* newTable = malloc(sizeof(Table));
  newTable->pages = (Page*) calloc(size, sizeof(Page));
  newTable->mrvIndex = (int*) calloc(size, sizeof(int));
  newTable->luIndex = 0;
  newTable->size = size;
  newTable->currOccupancy = 0;
  return newTable;
}

int getLRU(Page* pages, int tableSize) {
    int lru;
    int max = 0;
    for(int i=0; i<tableSize; i++) {
        if(max < pages[i].timeSinceAcc) {
            max = pages[i].timeSinceAcc;
            lru = i;
        }
    }
    return lru;
}

// updateMemory reads/writes a page with a given algorithm on a table
void updateMemory(int op, int alg, Page* page, Table* table) {
    bool* pageInTable;
    int* index;
    alreadyExists(page->address, table, pageInTable, index, alg);
    // case page needs to be inserted in a not-full table
    if (!pageInTable && table->currOccupancy < table->size) {

    }
    // case page needs to be inserted in a full table
    else if (!pageInTable && table->currOccupancy == table->size) {

    }
    // case page already exists in the table
    else {

    }
}

// alreadyExists checks if a page is in a table
// pageInTable is true if the page is in the table, and false otherwise
// index is the int where the new page should be inserted
void alreadyExists(int pageAddr, Table* table, bool* pageInTable, int* index, int alg) {
    bool t = true; bool f = false;

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
            auxIndex = getLRU(table->pages, table->size);
            index = &auxIndex;
        break;

        case 2: // 2a
            for(int i=table->luIndex; i<table->size; i++) {
                if(table->pages[i].nxtVictim) {
                    auxIndex = i;
                    index = &auxIndex;
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
            index = &auxIndex;
        break;

        default: // other
            printf("Other algorithm\n");
        }
    }
};