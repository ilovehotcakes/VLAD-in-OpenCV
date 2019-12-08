#include <opencv2/core.hpp>  // kmeans
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <fstream>
using namespace cv;  // Todo: add scope for cv namespace
using namespace std;


// Todo: add namespace for cv and VLAD
// Todo: change the interface to match opencv style
class VLAD 
{
private:
	const string filename;
	const int clusters;
	const int dimensions;
	const int iterations;
	int sampleCount;
	Mat finalV;  // Todo: change this name


	Mat readFile()
	{
		ifstream file(filename);
		float x;
		file >> x;
		file >> x;

		sampleCount = x;  // Number of samples
		Mat points = Mat::zeros(sampleCount, dimensions, CV_32FC1);

		// Reading .hesaff.sift file to memory
		for (int i = 0; i < sampleCount; i++) {
			for (int j = 0; j < 5; j++)
				file >> x;  // Skip first 5 values

			for (int d = 0; d < dimensions; d++) {
				file >> x;
				points.at<float>(i, d) = x;
			}
		}

		// Todo: close file
		return points;
	}

	// Initialize and label cluster centroids
	void initLabel(Mat &labels, const Mat &points)
	{
		Mat centars(clusters, dimensions, CV_32FC1, Scalar(0));
		int max = 81;  // Todo: max seems to be 162 though

		// Initial k cluster centers
		for (int k = 0; k < clusters; k++) {
			for (int d = k * 8; d < (k * 8 + 2); d++)
				centars.at<float>(k, d) = max;
		}

		for (int i = 0; i < sampleCount; i++) {
			vector<float> vec;  // To rank closest centriod => size = 16
			vector<float> vp;   // for point => size = 128

			for (int d = 0; d < dimensions; d++)
				vp.push_back(points.at<float>(i, d));

			for (int k = 0; k < clusters; k++) {
				vector<float> vc;  // for clusters

				for (int d = 0; d < dimensions; d++)
					vc.push_back(centars.at<float>(k, d));

				vec.push_back(norm(vp, vc));
			}

			float min = vec.at(0);
			int cnt = 0;
			for (int k = 1; k < clusters; k++) {
				if (vec.at(k) <= min) {
					min = vec.at(k);
					cnt = k;
				}
			}

			labels.at<int>(i) = cnt;
		}
	}

	
	// Todo: Use any descriptor


	// Compute the fisher vectors
	// Todo: change inputArray from sift file to actual image, add which descriptor to use, SIFT/SURF/etc.
	void computeVLAD(const Mat &points)
	{
		Mat centers; // Center of the clusters k
		Mat labels = Mat(1, sampleCount, CV_32SC1);  // Mapping each point in points to a cluster
		initLabel(labels, points);  // Todo: can use codebook

		// Compute k-means for each cluster k and label each points
		kmeans(points, clusters, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 300, 0.001), iterations, KMEANS_USE_INITIAL_LABELS, centers);
		
		// For each cluster, compute fisher vector
		Mat fisherV;
		centers.copyTo(fisherV);
		for (int i = 0; i < sampleCount; i++) {
			int localCluster = labels.at<int>(i);
			for (int d = 0; d < dimensions; d++) {
				if (centers.at<float>(localCluster, d) < points.at<float>(i, d))
					fisherV.at<float>(localCluster, d) += points.at<float>(i, d);
				else
					fisherV.at<float>(localCluster, d) -= points.at<float>(i, d);
			}
		}

		// Normalize vector using L2-norm, Todo: positive or negative values?
		normalize(fisherV, finalV);
	}

	// Helper function for drawVLAD
	Scalar rOrB(float value)
	{
		// Red if positive, else blue
		return (value > 0)? Scalar(0, 0, 255) : Scalar(255, 0, 0);
	}


public:
	VLAD(const string filename, const int k = 16, const int d = 128, const int it = 10)
		: filename(filename), clusters(k), dimensions(d), iterations(it)
	{
		Mat inputArray = readFile();
		computeVLAD(inputArray);
	}

	// Todo: parameterize the line thickness, and center dot radius
	void drawVLAD(int resolution = 32)
	{
		int sqSize = resolution;
		double rad = sqSize * 6;
		double slantedRad = rad * cos(CV_PI / 4);
		int thickness = 1;
		Mat img(sqSize * 4, sqSize * 64, CV_8UC3, Scalar::all(255));


		// Translate the range of the dimensions to -0.5 to 0.5
		for (int k = 0; k < clusters; k++) {
			for (int d = 0; d < dimensions; d++) {
				if (finalV.at<float>(k, d) == 0)
					;
				else
					finalV.at<float>(k, d) -= 0.02;  // Todo: parameterize this
			}
		}


		for (int i = 0; i < clusters; i++) {
			int counter = 0;
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					Point sqCtr(sqSize / 2 + (i * 4 + r) * sqSize, sqSize / 2 + c * sqSize);
					{
						line(img, sqCtr, Point(sqCtr.x + slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y + slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y + slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x + slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y - slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - slantedRad * finalV.at<float>(i, counter), sqCtr.y - slantedRad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;

						line(img, sqCtr, Point(sqCtr.x, sqCtr.y + rad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x, sqCtr.y - rad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - abs(finalV.at<float>(i, counter)) * rad, sqCtr.y), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x + abs(finalV.at<float>(i, counter)) * rad, sqCtr.y), rOrB(finalV.at<float>(i, counter)), thickness);
						circle(img, sqCtr, 2, Scalar(0), thickness * -1);
					}
				}
			}
			line(img, Point(4 * i * sqSize, 0), Point(4 * i * sqSize, 4 * sqSize), Scalar(0), thickness);
		}

		{
			// rest of the square
			line(img, Point(64 * sqSize - 1, 0), Point(64 * sqSize - 1, 4 * sqSize), Scalar(0));
			line(img, Point(0, 4 * sqSize - 1), Point(64 * sqSize, 4 * sqSize - 1), Scalar(0));
			line(img, Point(0, 0), Point(64 * sqSize, 0), Scalar(0));
		}


		imshow(filename, img);
		// Todo: Option to write image
		//imwrite("output.jpg", img);
	}

	Mat getVLAD()
	{
		return finalV;
	}

	// Return the requested cluster as a Mat
	Mat getVLAD(const int k)
	{
		if (k >= 0 && k < clusters) {
			Mat subVLAD;
			for (int d = 0; d < dimensions; d++)
				subVLAD.push_back(finalV.at<float>(k, d));
			return subVLAD;
		}
	}

	void compareVLAD(VLAD &comp)
	{
		// Todo: use knn
	}
};
