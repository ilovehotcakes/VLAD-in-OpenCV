#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace cv;
using namespace std;

Scalar rOrB(float value) {
	return (value > 0.1)? Scalar(0, 0, 255) : Scalar(255, 0, 0);
}

int main() {
	string filename = "../sift/kittens1.hesaff.sift";
	Scalar red = Scalar(0, 0, 255);
	Scalar blue = Scalar(255, 0, 0);

	ifstream file(filename);
	float x;
	file >> x;
	file >> x;

	int k = 16;  // Number of clusters
	int d = 128; // Number of dimension per descriptor
	int sampleCount = x;  // Number of samples


	Mat points = Mat::zeros(sampleCount, d, CV_32FC1);
	Mat labels;  // points
	Mat centers; // Center of the clusters

	// Reading .hesaff.sift file to memory
	for (int i = 0; i < sampleCount; i++) {
		for (int j = 0; j < 5; j++) {
			file >> x;
		}

		for (int j = 0; j < d; j++) {
			file >> x;
			points.at<float>(i, j) = x;
		}
	}

	// k-mean calculation
	kmeans(points, k, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 300, 0.001), 10, KMEANS_PP_CENTERS, centers);
	
	// Accessing label -> points
	// labels.at<int>(i) = which k
	
	// centers.at<float>(k, d) => d value at k


	Mat fisherV;
	centers.copyTo(fisherV);
	for (int i = 0; i < sampleCount; i++) {
		int localCluster = labels.at<int>(i);
		for (int j = 0; j < d; j++) {
			if (centers.at<float>(localCluster, j) < points.at<float>(i, j))
				fisherV.at<float>(localCluster, j) += points.at<float>(i, j);
			else
				fisherV.at<float>(localCluster, j) -= points.at<float>(i, j);
		}
	}

	// L2-norm
	Mat finalV;
	normalize(fisherV, finalV);


	/* Translate the range of the dimensions to -0.5 to 0.5
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < d; j++)
			finalV.at<float>(i, j) -= 0.5;
	}*/
	


	// Draw descriptor
	int sqSize = 16;
	double rad = sqSize * 6;
	double slantedRad = rad * cos(CV_PI / 4);
	Mat img(sqSize * 4, sqSize * 64, CV_8UC3, Scalar::all(255));

	for (int i = 0; i < k; i++) {
		int counter = 0;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 4; c++) {
				Point sqCtr(sqSize / 2 + (i * 4 + r) * sqSize, sqSize / 2 + c * sqSize);
				{
					line(img, sqCtr, Point(sqCtr.x + slantedRad * finalV.at<float>(i, counter), sqCtr.y + slantedRad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter++)));
					line(img, sqCtr, Point(sqCtr.x - slantedRad * finalV.at<float>(i, counter), sqCtr.y + slantedRad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter++)));
					line(img, sqCtr, Point(sqCtr.x + slantedRad * finalV.at<float>(i, counter), sqCtr.y - slantedRad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter++)));
					line(img, sqCtr, Point(sqCtr.x - slantedRad * finalV.at<float>(i, counter), sqCtr.y - slantedRad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter++)));

					line(img, sqCtr, Point(sqCtr.x, sqCtr.y + rad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter++)));
					line(img, sqCtr, Point(sqCtr.x, sqCtr.y - rad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter++)));
					line(img, sqCtr, Point(sqCtr.x - finalV.at<float>(i, counter) * rad, sqCtr.y), rOrB(finalV.at<float>(i, counter++)));
					line(img, sqCtr, Point(sqCtr.x + finalV.at<float>(i, counter) * rad, sqCtr.y), rOrB(finalV.at<float>(i, counter)));
					circle(img, sqCtr, 2, Scalar(0));
				}
			}
		}
		line(img, Point(4 * i * sqSize, 0), Point(4 * i * sqSize, 4 * sqSize), Scalar(0));
	}

	{
		// rest of the square
		line(img, Point(64 * sqSize - 1, 0), Point(64 * sqSize - 1, 4 * sqSize), Scalar(0));
		line(img, Point(0, 4 * sqSize - 1), Point(64 * sqSize, 4 * sqSize - 1), Scalar(0));
		line(img, Point(0, 0), Point(64 * sqSize, 0), Scalar(0));
	}


	imshow("result", img);
	imwrite("kittens2.jpg", img);

	cout << "done" << endl;
	waitKey(0);

	return 0;
}