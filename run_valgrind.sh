#! /bin/bash

mkdir debug
cd debug
cmake -DBUILD_TYPE=Debug -DTHREADS=1 .. 
make
cd benchmark
valgrind --leak-check=full ./model_benchmark.exe data/model/IrisRandomForest.xml data/dataset/Iris.csv
cd ..
rm -r debug
