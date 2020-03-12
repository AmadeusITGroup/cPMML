#! /bin/bash

clang-format -style="{BasedOnStyle: Google, ColumnLimit: 120}" -i benchmark/*.cc test/*.cc src/*.h src/*/*.h src/*/*.cc include/*.h
