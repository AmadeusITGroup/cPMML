mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build . --target cPMML
copy libcPMML.dll* test
cmake --build . --target unit_tests
cd ..
mkdir cpmml_win
copy build\libcPMML.dll* cpmml_win
copy include\cPMML.h cpmml_win
rmdir /Q/S build