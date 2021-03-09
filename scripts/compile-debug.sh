gcc -c src/main.c src/table.c lib/table.h -g
mv main.o out/main.o
mv table.o out/table.o
gcc -o out/tp2virtuald out/main.o out/table.o
