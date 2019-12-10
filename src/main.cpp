#include "vlad_trainer.cpp"
// #include "vlad.cpp"


int main(int argc, char* argv[]) {
	// Training kittens1, kittens2, pippy, ukbench00000-00003
	// Using opencv SIFT
    cv::Ptr<Feature2D> detSIFT = cv::xfeatures2d::SIFT::create();  // SURF use minHash = 400

	/*VLAD_trainer jason;
    jason.compute("list.txt", detSIFT);
    jason.train(16);
    jason.write("codebook");
    cout << "SIFT-VLAD training complete.." << endl;*/

    // Testing SIFT-VLAD
    //Mat codebook = jason.getBook();
	VLAD kittens1_SIFT("../test_images/kittens1.jpg", "codebook.yml", detSIFT);
	kittens1_SIFT.draw();
	VLAD kittens2_SIFT("../test_images/kittens2.jpg", "codebook.yml", detSIFT);
	kittens2_SIFT.draw();
	VLAD pippy_SIFT("../test_images/pippy.jpg", "codebook.yml", detSIFT);
	pippy_SIFT.draw();
	waitKey(0);



	/* Using SURF
	cv::Ptr<Feature2D> detSURF = cv::xfeatures2d::SURF::create(400);  // minHash = 400

	VLAD_trainer harry;
	harry.compute("list.txt", detSURF);
	harry.train(16);
	cout << "SURF-VLAD training complete.." << endl;

	// Testing SURF-VLAD
	Mat codebook_SURF = harry.getBook();
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
