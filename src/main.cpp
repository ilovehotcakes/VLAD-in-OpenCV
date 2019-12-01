#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <iostream>
#include "vlad.cpp"
using namespace std;
using namespace cv;


int main(int argc, char* argv[]) {
	VLAD kittens1("../sift/kittens1.hesaff.sift");
	VLAD kittens2("../sift/kittens2.hesaff.sift");

	//kittens1.drawVLAD();
	//waitKey(0);
	//kittens2.drawVLAD();

	
	kittens1.compareVLAD(kittens2);


	getchar();

	return 0;
}