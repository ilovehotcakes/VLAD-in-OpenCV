#include "vlad_trainer.cpp"
// #include "vlad.cpp"


int main(int argc, char* argv[]) {
    cv::Ptr<Feature2D> detector = cv::xfeatures2d::SIFT::create();  // SURF use minHash = 400

	/* Training kittens1, kittens2, pippy, ukbench00000-00003
	VLAD_trainer jason;
    jason.compute("list.txt", detector);
	//jason.saveDesc("test_sift");
    jason.train(16);
    jason.write("codebook");
    cout << "Training complete.." << endl;

    // Testing VLAD-SIFT
    Mat codebook = jason.getBook();
	VLAD kittens1("../test_images/kittens1.jpg", codebook, detector);
	VLAD kittens2("../test_images/kittens2.jpg", codebook, detector);
	kittens1.draw();
	kittens2.draw();
	// VLAD pippy("../test_images/pippy.jpg", codebook, detector);
	// pippy.draw();
	waitKey(0);*/


    // Training INRIA Holiday dataset with 16 visual words
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
	// waitKey(0);
    

    // Todo: test SURF
	
	
	return 0;
}
