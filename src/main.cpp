#include "vlad_trainer.cpp"


int main(int argc, char* argv[]) {
	// Using opencv SIFT
    cv::Ptr<Feature2D> detSIFT = cv::xfeatures2d::SIFT::create();

	// Training kittens1, kittens2, pippy, ukbench00000-00003
	VLAD_trainer yoda;
    yoda.train("list.txt", detSIFT, 16);
    yoda.write("codebook");
    cout << "SIFT-VLAD training complete.." << endl;

    // Testing SIFT-VLAD + trained codebook
    Mat codebook = yoda.getBook();
	VLAD kittens1_SIFT("../test_images/kittens1.jpg", codebook, detSIFT);
	kittens1_SIFT.draw();
	VLAD kittens2_SIFT("../test_images/kittens2.jpg", codebook, detSIFT);
	kittens2_SIFT.draw();
	VLAD pippy_SIFT("../test_images/pippy.jpg", codebook, detSIFT);
	pippy_SIFT.draw();
	waitKey(0);

	// Testing SIFT-VLAD + pre-trained codebook ukbench_16
	VLAD kittens1("../test_images/kittens1.jpg", "ukbench_16.yml", detSIFT);
	kittens1.draw();
	VLAD kittens2("../test_images/kittens2.jpg", "ukbench_16.yml", detSIFT);
	kittens2.draw();
	VLAD pippy("../test_images/pippy.jpg", "ukbench_16.yml", detSIFT);
	pippy.draw();
	waitKey(0);


	/* Using SURF
	cv::Ptr<Feature2D> detSURF = cv::xfeatures2d::SURF::create(400);  // minHash = 400

	VLAD_trainer obiwan;
	obiwan.compute("list.txt", detSURF, 16);
	cout << "SURF-VLAD training complete.." << endl;

	// Testing SURF-VLAD
	Mat codebook_SURF = obiwan.getBook();
	VLAD kittens1_SURF("../test_images/kittens1.jpg", codebook_SURF, detSURF, 16, 64);
	kittens1_SURF.draw();
	VLAD kittens2_SURF("../test_images/kittens2.jpg", codebook_SURF, detSURF, 16, 64);
	kittens2_SURF.draw();
	// VLAD pippy_SURF("../test_images/pippy.jpg", codebook, detector);
	// pippy_SURF.draw();
	waitKey(0);*/




    /* Training INRIA Holiday dataset with 16 visual words
	VLAD_trainer nelson;
    nelson.compute("list_ukbench.txt", detector);
    nelson.train(16);
    nelson.write("holiday_16");
    cout << "Training complete(k = 16).." << endl;

    // Training INRIA Holiday dataset with 64 visual words
    nelson.train(64);
    nelson.write("holiday_64");
	cout << "Training complete(k = 64).." << endl;

    // // Testing VLAD-SIFT
	// VLAD vlad("../holiday/100000.jpg", "holiday_16.yml", detector);
	// vlad.draw();
	// waitKey(0);*/

	
	return 0;
}
