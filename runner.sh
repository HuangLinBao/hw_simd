#!/bin/bash
echo "=================for size = 128================="

echo "==================Vector * Vector=============================="

gcc -O0 ./size128/vec_vec.cpp -o out
./out

echo "==================Matrix * Vector=============================="

gcc -O0 ./size128/mat_vec.cpp -o out
./out

echo "==================Matrix * Matrix=============================="

gcc -O0 ./size128/mat_mat.cpp -o out
./out


echo "=================for size = 256================="

echo "==================Vector * Vector=============================="

gcc -O0 ./size256/vec_vec.cpp -o out
./out

echo "==================Matrix * Vector=============================="

gcc -O0 ./size256/mat_vec.cpp -o out
./out

echo "==================Matrix * Matrix=============================="

gcc -O0 ./size256/mat_mat.cpp -o out
./out


echo "=================for size = 512================="

echo "==================Vector * Vector=============================="

gcc -O0 ./size512/vec_vec.cpp -o out
./out

echo "==================Matrix * Vector=============================="

gcc -O0 ./size512/mat_vec.cpp -o out
./out

echo "==================Matrix * Matrix=============================="

gcc -O0 ./size512/mat_mat.cpp -o out
./out