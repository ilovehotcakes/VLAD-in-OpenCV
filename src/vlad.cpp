#include <opencv2/core.hpp>  // For K-means
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
// #include <opencv2/xfeatures2d/sift.hpp>
using namespace cv;
using namespace std;


// Todo: add namespace for cv and VLAD
class VLAD 
{
private:
	// const string filename;
	const int k;
	const int d;
	const int iterations;
	int sampleCount;
	Mat finalV;


	Mat readFile(const string filename) {
		ifstream file(filename);
		float x;
		file >> x;
		file >> x;

		sampleCount = x;  // Number of samples
		Mat points = Mat::zeros(sampleCount, d, CV_32FC1);

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

		// Todo: close file
		return points;
	}


	// Todo: Hessian affine-region detector

	// Todo: use opencv SIFT descriptor

	// Compute the fisher vectors
	// Todo: change inputArray from sift file to actual image
	void computeVLAD(const Mat &points)
	{
		Mat centers; // Center of the clusters k
		Mat labels;  // Mapping each point in points to a k

		// Compute k-means for each cluster k and label each points
		kmeans(points, k, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 300, 0.001), iterations, KMEANS_PP_CENTERS, centers);
		
		// For each cluster, compute fisher vector
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

		// Normalize vector using L2 norm
		normalize(fisherV, finalV);
	}

	// Helper function for drawVLAD
	Scalar rOrB(float value) {
		return (value > 0)? Scalar(0, 0, 255) : Scalar(255, 0, 0);
	}


public:
	VLAD(const string filename, const int k = 16, const int dimensions = 128, const int iterations = 10)
		: k(k), d(dimensions), iterations(iterations)
	{
		Mat inputArray = readFile(filename);
		computeVLAD(inputArray);
	}

	// Todo: parameterize the line thickness, and center dot radius
	void drawVLAD(int resolution = 32)
	{
		int sqSize = resolution;
		double rad = sqSize * 10;
		double slantedRad = rad * cos(CV_PI / 4);
		Mat img(sqSize * 4, sqSize * 64, CV_8UC3, Scalar::all(255));


		// Translate the range of the dimensions to -0.5 to 0.5
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < d; j++) {
				// cout << finalV.at<float>(i, j) << "  ";
				if (finalV.at<float>(i, j) == 0)
					;
				else {
					finalV.at<float>(i, j) -= 0.02;  // Todo: parameterize this
					// cout << finalV.at<float>(i, j) << endl;
				}
			}
		}


		for (int i = 0; i < k; i++) {
			int counter = 0;
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					Point sqCtr(sqSize / 2 + (i * 4 + r) * sqSize, sqSize / 2 + c * sqSize);
					{
						line(img, sqCtr, Point(sqCtr.x + slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y + slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)));
						counter++;
						line(img, sqCtr, Point(sqCtr.x - slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y + slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)));
						counter++;
						line(img, sqCtr, Point(sqCtr.x + slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y - slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)));
						counter++;
						line(img, sqCtr, Point(sqCtr.x - slantedRad * finalV.at<float>(i, counter), sqCtr.y - slantedRad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter)));
						counter++;

						line(img, sqCtr, Point(sqCtr.x, sqCtr.y + rad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)));
						counter++;
						line(img, sqCtr, Point(sqCtr.x, sqCtr.y - rad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)));
						counter++;
						line(img, sqCtr, Point(sqCtr.x - abs(finalV.at<float>(i, counter)) * rad, sqCtr.y), rOrB(finalV.at<float>(i, counter)));
						counter++;
						line(img, sqCtr, Point(sqCtr.x + abs(finalV.at<float>(i, counter)) * rad, sqCtr.y), rOrB(finalV.at<float>(i, counter)));
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
		//imwrite("output.jpg", img);

		//cout << "done" << endl;
		waitKey(0);
	}

	// Todo: void outputVLAD()	
};

