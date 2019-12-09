#include "vlad_trainer.cpp"
// #include "vlad.cpp"


int main(int argc, char* argv[]) {
    cv::Ptr<Feature2D> detector = cv::xfeatures2d::SIFT::create();

	// Training data
	VLAD_trainer jason;
    jason.compute("list.txt", 16);
    jason.write();
    cout << "Training complete.." << endl;

    // Testing VLAD-SIFT
    Mat codebook = jason.getBook();
	VLAD kittens1("../test_images/kittens1.jpg", codebook, detector);
	VLAD kittens2("../test_images/kittens2.jpg", codebook, detector);
	kittens1.drawVLAD();
	kittens2.drawVLAD();
	waitKey(0);
	
	return 0;
}


// ifstream f1;
// f1.open("../test_images/list.txt");


// while(!f1.eof())
// {        
//     FileStorage fs(filename + ".vlad", FileStorage::WRITE);  // Store to disk
//     fs << "VLAD-SIFT" << vladDes;
//     fs.release();
// }
// f1.close();
