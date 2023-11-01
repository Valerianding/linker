#!/bin/bash

mkdir -p out

for file in tests/*.c; do
  filename=$(basename "$file" .c)

  mkdir -p "out/$filename"

  echo "riscv64-linux-gnu-gcc -c" ${file} "-o out/$filename/$filename.o"

  riscv64-linux-gnu-gcc -c "$file" -o "out/$filename/$filename.o"
done

echo "Passed"