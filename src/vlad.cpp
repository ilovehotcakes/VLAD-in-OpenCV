#include <opencv2/core.hpp>  // For K-means
#include "sift.cpp"
using namespace std;
using namespace cv;

class VLAD 
{
private:
	// Hessian affine-region detector

	// SIFT descriptor

	// K-means algorithm
	kmeans();

	// Compute the fisher vectors
	void computeVLAD()
	{
		// For each cluster, compute fisher vector

		// Normalize vector using L1 norm
	}


	// Mat* inputArray

	// Mat histogram (D = k * dimensions)

	// Vector listOfSIFT

	// const int k

	// const int dimensions

	// const int iterations

public:
	VLAD(const Mat &inputArray, const int k, const int dimensions)
	{
		computeVLAD(inputArray, k, dimensions);
	}
	
	// getVLAD?

	// void drawVLAD()

	// void outputVLAD()	
};
