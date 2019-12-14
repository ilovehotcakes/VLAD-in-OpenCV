#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <iostream>
#include "vlad.cpp"
using namespace std;
using namespace cv;


int main(int argc, char* argv[]) {
	VLAD kittens1("kittens1.jpg.hesaff.sift");
	VLAD kittens2("kittens2.jpg.hesaff.sift");

	Mat img1 = kittens1.drawVLAD(64, 2);
	imwrite("kittens1.png", img1);
	Mat img2 = kittens2.drawVLAD(64, 2);
	imwrite("kittens2.png", img2);
	waitKey(0);

	return 0;
}