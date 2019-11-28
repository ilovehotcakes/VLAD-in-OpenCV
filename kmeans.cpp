#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
	ifstream file("../sift/pippy.jpg.hesaff.sift");
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
		for (int j = 0; j < d; j++)
			fisherV.at<float>(labels.at<int>(i), j) += points.at<float>(i, j);
	}
	cout << fisherV.at<float>(1, 0) << endl;

	cout << "done" << endl;
	getchar();

	return 0;
}