# VLAD in OpenCV

## Introduction
This is an OpenCV implementation of Hervé Jégou, Matthijs Douze, Cordelia Schmid, and Patrick Pérez’s work titled "Aggregating local descriptors into a compact representation". They try to solve the problem of retreiving image from a large database while maintaining efficiency, accuracy, and low memory usage. The paper utilizes techniques like Bag-of-words, Fisher Kernel, PCA, kNN search. It is a very well written paper and I would recommend anyone to read it.


## Support
This VLAD implementation supports OpenCV 2.4 and requires the extra modules for SIFT.

+ For Windows users, you can follow the tutorial here to build and install the modules: https://putuyuwono.wordpress.com/2015/04/23/building-and-installing-opencv-3-0-on-windows-7-64-bit/

+ For Mac users, the extra modules are already installed if you installed OpenCV via HomeBrew


## Usage
Use "main.cpp" to run the system. 
