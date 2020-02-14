//--------------------------------- main.cpp ----------------------------------
// Author: Jason Chen
//-----------------------------------------------------------------------------
// This is the driver to test VLAD class and VLADTrainer class that are based
// on Jégou et al.'s paper "Aggregating local descriptors into a compact image
// presentation"(2010). It tests the codebook generation with both OpenCV SIFT
// and SURF descriptors. It will draw the VLAD on screen.
//
// Inputs: kittens1.jpg, kittens2.jpg, pippy.jpg, ukbench00000-00001.jpg,
//         ukbench_16.yml, list.txt
// Ouputs: SIFT-like VLAD display, codebook.yml
//-----------------------------------------------------------------------------
#include "vlad_trainer.cpp"
#include <iostream>


int main(int argc, char* argv[]) {
	// Using OpenCV SIFT descriptor
    cv::Ptr<Feature2D> detSIFT = cv::xfeatures2d::SIFT::create();

	// Training kittens1, kittens2, pippy, ukbench00000-00003
	VLADTrainer yoda("../test_images/", "list.txt");
    yoda.train(detSIFT, 16);  // Train codebook of 16 visual words
    yoda.write("codebook");   // Write codebook to disk
    cout << "SIFT-VLAD training complete.." << endl;

    // Testing SIFT-VLAD + codebook
    Mat codebook = yoda.getBook();
	VLAD kittens1_SIFT("../test_images/kittens1.jpg", codebook, detSIFT);
	kittens1_SIFT.draw(32, 2);
	VLAD kittens2_SIFT("../test_images/kittens2.jpg", codebook, detSIFT);
	kittens2_SIFT.draw(32, 2);
	VLAD pippy_SIFT("../test_images/pippy.jpg", codebook, detSIFT);
	pippy_SIFT.draw(32, 2);
	waitKey(0);

	// Testing SIFT-VLAD + pre-trained codebook "ukbench_16"
	VLAD kittens1("../test_images/kittens1.jpg", "ukbench_16.yml", detSIFT);
	kittens1.draw(32, 2);
	VLAD kittens2("../test_images/kittens2.jpg", "ukbench_16.yml", detSIFT);
	kittens2.draw(32, 2);
	VLAD pippy("../test_images/pippy.jpg", "ukbench_16.yml", detSIFT);
	pippy.draw(32, 2);
	waitKey(0);

	

	// Using OpenCV SURF descriptor
	cv::Ptr<Feature2D> detSURF = cv::xfeatures2d::SURF::create(400);  // minHash = 400

	VLADTrainer obiwan("../test_images/", "list.txt");
	obiwan.train(detSURF, 16);
	cout << "SURF-VLAD training complete.." << endl;

	// Testing SURF-VLAD
	Mat codebook_SURF = obiwan.getBook();
	VLAD kittens1_SURF("../test_images/kittens1.jpg", codebook_SURF, detSURF, 16, 64);
	kittens1_SURF.draw();
	VLAD kittens2_SURF("../test_images/kittens2.jpg", codebook_SURF, detSURF, 16, 64);
	kittens2_SURF.draw();
	VLAD pippy_SURF("../test_images/pippy.jpg", codebook_SURF, detSURF, 16, 64);
	pippy_SURF.draw();



    /* Training INRIA Holiday dataset with 16 visual words
	VLADTrainer anakin("../ukbench/", "list_ukbench.txt");
    anakin.train(detSIFT, 16);
    anakin.write("holiday_16");
    cout << "Training complete(k = 16).." << endl;

    // Testing VLAD-SIFT
	VLAD vlad("../holiday/100000.jpg", "holiday_16.yml", detSIFT);
	vlad.draw();*/

	waitKey(0);	
	return 0;
}
