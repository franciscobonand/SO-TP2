gcc -c src/main.c src/table.c lib/table.h
cp main.o out/main.o
cp table.o out/table.o
gcc -o out/tp2virtual out/main.o out/table.o
