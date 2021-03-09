#!/bin/bash

for method in lru 2a fifo 
do
  for i in 2 4 8 16 32 64
  do
    for j in 128 256 512 1024 2048 4096 8192 16384
    do
      echo "Running $method examples/compilador.log $i $j"
      ./../out/tp2virtual $method examples/compilador.log $i $j | tr -d '\n' | sed -E "s/.*páginas: ([0-9]+).*memória: ([0-9]+).*Algoritmo: (.*)Reading file (.*)Paginas lidas: ([0-9]+)Paginas escritas: ([0-9]+)Tempo gasto: (.*)Endereço(.*)/\1,\2,\3,\4,\5,\6,\7\n/" >> data/experiments.csv
    done
  done
done
