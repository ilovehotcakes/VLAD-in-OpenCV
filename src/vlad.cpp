//--------------------------------- vlad.cpp ----------------------------------
// Author: Jason Chen
//-----------------------------------------------------------------------------
// This is the VLAD class that is based on Jégou et al.'s paper "Aggregating 
// local descriptors into a compact image presentation"(2010). It is used to 
// generate VLAD descriptors using OpenCV Ptr and BOWImgDescriptorExtractor.
//
// Requirements: OpenCV with contrib module
// Inputs: codebook, image filename
// Ouputs: VLAD descriptor, VLAD visualization
// Limits: (1) Only able to use codebook with kVisualWords = VLAD clusters
//         (2) Each cell for the visualization will represent 128 values regard
//             -less of how many dimensions each cluster contains
//-----------------------------------------------------------------------------
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <fstream>
#include <vector>
using namespace cv;
using namespace std;


class VLAD 
{
private:
	const string filename;    // Path or filename
	const int clusters;       // Cluster k
	const int dimensions;     // Dimesion d
	Ptr<Feature2D> detector;  // Type of detector to be used
	Mat vladDesc;             // Final VLAD descriptor


	// Loading cookbook into memory(Mat codebook)
	Mat readBook(const string dictionary)
	{
		FileStorage fs(dictionary, FileStorage::READ);
		Mat codebook;
		fs["codebook"] >> codebook;
		fs.release();
		return codebook;
	}


	// Compute the fisher vectors and normalized to VLAD
	void computeVLAD(const Mat &codebook)
	{
		// Use Opencv BOWImgDescriptorExtractor to match detected
		// points to codebook vocabulary
		Ptr<DescriptorExtractor> extractor = detector;
		Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
		BOWImgDescriptorExtractor bowide(extractor, matcher);
		bowide.setVocabulary(codebook);
		
		// Compute SIFT/SURF/etc. descriptors
        Mat img, desc;
        vector<KeyPoint> keypoints;
        img = imread(filename);
        detector->detectAndCompute(img, Mat(), keypoints, desc);

        // Match each SIFT/SURF/etc. desc to a vocab from codebook
        vector<DMatch> matches;
        matcher->match(desc, matches); //desc Mat contains SIFT desc for each image
        
		// Compute Fisher vector
        Mat fisherVec(clusters, dimensions, CV_32FC1, Scalar::all(0.0));
        int sampleCount = matches.size();
        for(int i = 0; i < sampleCount; i++)
        {
            int queryIdx = matches[i].queryIdx;
            int trainIdx = matches[i].trainIdx;
            for (int d = 0; d < dimensions; d++)
                fisherVec.at<float>(trainIdx, d) += 
					(codebook.at<float>(trainIdx, d) - desc.at<float>(queryIdx, d));
        }

		// Normalize vector using L2-norm
		normalize(fisherVec, vladDesc);
	}

	
	// Helper function for draw
	// Returns the color of the line, blue if positive, else red
	Scalar rOrB(float value)
	{
		return (value > 0)? Scalar(255, 0, 0) : Scalar(0, 0, 255);
	}


	// Helper function for draw
	// Determines how many square is need to draw the representation
	int howManySq(int value)
	{
		return (value % 128 == 0)? (value / 128) : (value / 128 + 1);
	}


public:
	// VLAD constructor that takes a filename and codebook name/path
	VLAD(const string filename, const string dic,
		Ptr<Feature2D> detector, const int k = 16, const int d = 128)
		: filename(filename), clusters(k), dimensions(d), detector(detector)
	{
		Mat input = readBook(dic);
		computeVLAD(input);
	}


	// VLAD constructor that takes a filename and codebook of Mat
	VLAD(const string filename, Mat &dic,
		Ptr<Feature2D> detector, const int k = 16, const int d = 128)
		: filename(filename), clusters(k), dimensions(d), detector(detector)
	{
		computeVLAD(dic);
	}


	// VLAD destructor
	~VLAD() {}


	// Draws VLAD on screen and also returns the visualization
	Mat draw(int sqSize = 32, int thickness = 1)
	{
		double rad = sqSize * 5;  // Length of line
		int hms = howManySq(clusters * dimensions);
		Mat img(sqSize * 4, sqSize * 4 * hms, CV_8UC3, Scalar::all(255));

		// Each Square will represent 128 dimensions
		for (int k = 0; k < hms; k++) {
			int counter = 0;
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					// Center of each dot, 16 dots per square
					Point dotCtr(sqSize / 2 + sqSize * (k * 4 + r), sqSize / 2 + sqSize * c);

					// Drawing the lines, 8 lines per dot
					for (int p = 0; p < 8; p++) {
						double angle = p * 45.0;
						float value = vladDesc.at<float>(k, counter++);
							line(img, dotCtr, Point(dotCtr.x + abs(value) * rad
							* cos(angle * CV_PI / 180.0), dotCtr.y + abs(value) * rad 
							* sin(angle * CV_PI / 180.0)), rOrB(value), thickness);
					}
					// Drawing the center dot
					circle(img, dotCtr, sqSize / 16, Scalar(0), thickness * -(sqSize / 32));
				}
			}
			// Drawing borders between each cluster square
			line(img, Point(4 * k * sqSize, 0), Point(4 * k * sqSize, 4 * sqSize), Scalar(0), thickness);
		}
		// Drawing the rest of the border
		line(img, Point(hms * 4 * sqSize - thickness, 0), Point(hms * 4 * sqSize
			- thickness, 4 * sqSize), Scalar(0), thickness); // Right
		line(img, Point(0, 4 * sqSize - thickness), Point(hms * 4 * sqSize, 4 * sqSize
			- thickness), Scalar(0), thickness);                                   // Bottom
		line(img, Point(0, 0), Point(hms * 4 * sqSize, 0), Scalar(0), thickness);  // Top

		// Show visualization
		imshow(filename, img);
		return img;
	}


	// Returns VLAD as a Mat
	Mat getVLAD()
	{
		return vladDesc;
	}


	// Write VLAD to disk
	void write(const string path = "") {
		ofstream f(path + filename + ".vlad");
		for (int k = 0; k < clusters; k++) {
			for (int d = 0; d < dimensions; d++)
				f << vladDesc.at<float>(k, d) << " ";
			f << endl;
		}
		f.close();
	}
};
