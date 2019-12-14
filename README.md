# VLAD

This is the project based on Jégou et al.'s paper "Aggregating local descriptors
into a compact image presentation"(2010).


This project contains the main (driver file), VLAD class and VLADTrainer class.
Both classes are implemented using OpenCV and works with OpenCV descriptors.
For Mac users, you should be able to build and run the project using the build
script. For Windows users, the directory should be set up for Visual Studios
project. I included the property sheets that I used for my VS project.



+ test_images folder contains all the images needed to train a mini codebook
+ list.txt is the list of files in test_images
+ ukbench_16.yml is a codebook of 16 visual words trained using ukbench00000.jpg-
  ukbench00999.jpg and kittens1.jpg, kittens2.jpg, pippy.jpg



You can either run the program using your dataset to generate your own codebook,
or you can use the provided, pre-trained codebook to get you started.



Requirements: OpenCV with contrib module with NONFREE enabled