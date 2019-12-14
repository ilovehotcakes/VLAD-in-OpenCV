#! /bin/bash

g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.1.0_2/lib/pkgconfig/opencv4.pc) -std=c++11 -O3 hesaff.cpp affine.cpp helpers.cpp pyramid.cpp siftdesc.cpp -o driver

./driver