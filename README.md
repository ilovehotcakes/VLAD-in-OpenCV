# VLAD in OpenCV

## Introduction
This is an OpenCV implementation of Hervé Jégou, Matthijs Douze, Cordelia Schmid, and Patrick Pérez’s work titled "Aggregating local descriptors into a compact representation". They try to solve the problem of retreiving image from a large database while maintaining efficiency, accuracy, and low memory usage. The paper utilizes techniques like Bag-of-words, Fisher Kernel, PCA, kNN search. It is a very well written paper and I would recommend anyone to read it.


## Support
This VLAD implementation supports OpenCV 4.1.3 and requires the extra modules for SIFT/SURF (xfeatures2d).

+ For Windows users, you can follow the tutorial here to build and install the modules: https://putuyuwono.wordpress.com/2015/04/23/building-and-installing-opencv-3-0-on-windows-7-64-bit/

+ For Mac users, the extra modules are already installed if you installed OpenCV via HomeBrew

+ Currently supports OpenCV 4.1.3 SIFT an SURF


## Files
+ vlad.cpp - Using generated codebook to compute VLAD descriptor

+ vlad_trainer.cpp - Used generate codebook

+ main.cpp - Driver

+ property_sheets folder - Contains the property sheets when working in Visual Studios for OpenCV properties

+ codebooks folder - Contains the codebook I generated

+ results folder - Contains the output images from my tests


## Usage
+ Use the build script to build the program. Depending on the version of your OpenCV and the config, you might need to point to the correct OpenCV file in the build script 

+ Modify the list.txt to decide which files to compute

+ Use main as the driver to generate a new codebook, and use the codebook to generate the VLAD descriptor. 


## To-Do
+ Build comparison function

+ Build search engine

+ Documentation
