gcc -c src/main.c src/table.c lib/table.h
mv main.o out/main.o
mv table.o out/table.o
gcc -o out/tp2virtual out/main.o out/table.o
