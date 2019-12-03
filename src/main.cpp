#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <iostream>
#include "vlad.cpp"
using namespace std;
using namespace cv;


int main(int argc, char* argv[]) {
	VLAD kittens1("../sift/ukbench00000.hesaff.sift");
	VLAD kittens2("../sift/ukbench00001.hesaff.sift");
	VLAD kittens3("../sift/ukbench00002.hesaff.sift");
	VLAD kittens4("../sift/ukbench00003.hesaff.sift");

	kittens1.drawVLAD();
	kittens2.drawVLAD();
	kittens3.drawVLAD();
	kittens4.drawVLAD();
	waitKey(0);
	
	//kittens1.compareVLAD(kittens2);


	//getchar();

	return 0;
}