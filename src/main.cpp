#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <iostream>
#include "vlad.cpp"
using namespace std;
using namespace cv;


int main(int argc, char* argv[]) {
	VLAD kittens1("../ukbench_sift/ukbench00925.jpg.hesaff.sift");

	kittens1.drawVLAD();
	waitKey(0);
	

	//getchar();

	return 0;
}