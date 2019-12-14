#! /bin/bash

g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.1.0_2/lib/pkgconfig/opencv4.pc) -std=c++11 -O3 *.cpp -o driver

./driver