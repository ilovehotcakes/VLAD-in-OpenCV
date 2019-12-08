#! /bin/bash

cd src

g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.1.0_2/lib/pkgconfig/opencv4.pc) -std=c++11 -O3 -Wall -Werror -pedantic main_trainer.cpp vlad_trainer.cpp -o ../prog/driver

cd ..