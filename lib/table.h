#include <stdbool.h>

#if !defined(TABLE_H)
#define TABLE_H

typedef struct Page {
    int address;
    int value;
    int nxtVictim; // can be 0 or 1, used in 2a implementation
    int timeSinceAcc; // time since last access. defines by a counter in the main function
} Page;

typedef struct Table {
    Page *pages;
    int *mrvIndex; // keeps the index of the Most Recent Visited page
    int luIndex; // luIndex indicates the least used page. the secont to least is luIndex + 1 and so on
    int size;
    int currOccupancy;
} Table;


Table* initializeTable(int size);
Page* newPage();
void updateMemory(int op, int alg, Page* page, Table* table);
void alreadyExists(int pageAddr, Table* table, bool* pageInTable, int* index, int alg);

#endif // TABLE_H