#if !defined(TABLE_H)
#define TABLE_H

#define WRITE 1
#define READ 2

typedef struct Page {
    unsigned long int address;
    int nxtVictim; // can be 0 or 1, used in 2a implementation
    int timeSinceAcc; // time since last access. defines by a counter in the main function
    int isDirty;
} Page;

typedef struct Table {
    Page *pages;
    int *mrvIndex; // keeps the index of the Most Recent Visited page
    int luIndex; // luIndex indicates the last used page. the second to last is luIndex + 1 and so on
    int size;
    int currOccupancy;
    int clock; // counts the current access iteration
    int pagefaults;
    int dirtyPages;
} Table;


Table* initializeTable(int size);
Page* newPage();
void updateMemory(int op, int alg, Page* page, Table* table);
void alreadyExists(int pageAddr, Table* table, int* pageInTable, int* index, int alg);

#endif // TABLE_H
