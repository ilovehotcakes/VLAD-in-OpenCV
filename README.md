# VLAD in OpenCV

## Introduction
This is an OpenCV implementation of VLAD in C++ based on Hervé Jégou, Matthijs Douze, Cordelia Schmid, and Patrick Pérez’s work (2010). VLAD is a CBIR system that retreives image from a large database while maintaining efficiency, accuracy, and low memory usage. The paper utilizes techniques like Bag-of-words, Fisher Kernel, PCA, kNN search. It is a very good paper and I would recommend anyone to read it.

You can either run the program using your dataset to generate your own codebook, or you can use the provided, pre-trained codebook and dataset to get you started.


## Reference
Jegou, H., Perronnin, F., Douze, M., Sanchez, J., Perez, P., & Schmid, C. (2012). Aggregating Local Image Descriptors into Compact Codes. IEEE Transactions on Pattern Analysis and Machine Intelligence, 34(9), 1704-1716.


## Requirements
+ Requires OpenCV 4.1.0 and the contrib modules with NONFREE enabled.

+ For Windows users, you can follow the tutorial here to build and install the contrib modules: https://putuyuwono.wordpress.com/2015/04/23/building-and-installing-opencv-3-0-on-windows-7-64-bit/

+ For Mac users, the contrib modules comes installed if you installed OpenCV via HomeBrew

+ Currently supports OpenCV 4.1.0-4.1.2, might be backwards compatible but haven't been tested

+ Supports SIFT an SURF descriptors


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

+ Add result images

+ Add code samples

+ Add testing dataset


## Contributor
Jason Chen @ kidultimate@gmail.com
