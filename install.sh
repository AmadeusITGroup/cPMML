#! /bin/bash

mkdir build
cd build
cmake -j 4 ..
sudo env "PATH=$PATH" make install
cd ..
sudo rm -r build
sudo chown -R $USER docs
sudo ldconfig
