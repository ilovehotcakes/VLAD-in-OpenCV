#! /bin/bash

cd src

g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.1.0_2/lib/pkgconfig/opencv4.pc) -std=c++11 -O3 -Wall -Werror -pedantic hesaff.cpp affine.cpp helpers.cpp pyramid.cpp siftdesc.cpp -o ../prog/hessift

cd ..