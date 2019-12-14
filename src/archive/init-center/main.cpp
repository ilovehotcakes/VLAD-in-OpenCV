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
	VLAD uk0("ukbench00000.jpg.hesaff.sift");
	VLAD uk4("ukbench00004.jpg.hesaff.sift");
	VLAD uk8("ukbench00008.jpg.hesaff.sift");

	Mat img1 = kittens1.drawVLAD(64, 2);
	imwrite("kittens1.png", img1);
	Mat img2 = uk0.drawVLAD(64, 2);
	imwrite("ukbench00000.png", img2);
	Mat img3 = uk4.drawVLAD(64, 2);
	imwrite("ukbench00004.png", img3);
	Mat img4 = uk8.drawVLAD(64, 2);
	imwrite("ukbench00008.png", img4);
	waitKey(0);

	return 0;
}