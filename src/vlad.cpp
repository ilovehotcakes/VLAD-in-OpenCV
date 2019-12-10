#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <fstream>
#include <vector>
#include <math.h>  // sin, cos
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
	Ptr<Feature2D> detector;
	Mat vladDesc;


	// Loading cookbook.yml into memory(Mat codebook)
	Mat loadBook(const string dictionary)
	{
		FileStorage fs(dictionary, FileStorage::READ);
		Mat codebook;
		fs["codebook"] >> codebook;
		fs.release();

		return codebook;
	}


	// Compute the fisher vectors
	void computeVLAD(const Mat &codebook)
	{
		// Use Opencv BOWImgDescriptorExtractor to match detected points to vocab clusters
		Ptr<DescriptorExtractor> extractor = detector;
		Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
		BOWImgDescriptorExtractor bowide(extractor, matcher);
		bowide.setVocabulary(codebook);
		
		// Compute SIFT/SURF descriptors
        Mat img, desc;
        vector<KeyPoint> keypoints;
        img = imread(filename);
        detector->detectAndCompute(img, Mat(), keypoints, desc);

        // Compute VLAD descriptors
        vector<DMatch> matches;
        matcher->match(desc, matches); //desc contains descriptors for each image
        
        Mat fisherVec(clusters, dimensions, CV_32FC1, Scalar::all(0.0));
        int sampleCount = matches.size();
        for(int i = 0; i < sampleCount; i++)
        {
			// Todo: Need to rate top k matches if kvisualwords is > clusters
			// voting vector<int>(clusters)
            int queryIdx = matches[i].queryIdx;
            int trainIdx = matches[i].trainIdx;
            for (int d = 0; d < dimensions; d++)
                fisherVec.at<float>(trainIdx, d) += (codebook.at<float>(trainIdx, d) - desc.at<float>(queryIdx, d));
        }

		// Normalize vector using L2-norm
		normalize(fisherVec, vladDesc);
	}

	// Helper function for draw
	Scalar rOrB(float value)
	{
		// Blue if positive, else red
		return (value > 0)? Scalar(255, 0, 0) : Scalar(0, 0, 255);
	}


public:
	VLAD(const string f, const string dic,
		Ptr<Feature2D> detector, const int k = 16, const int d = 128)
		: filename(f), clusters(k), dimensions(d), detector(detector)
	{
		Mat input = loadBook(dic);
		computeVLAD(input);
	}


	VLAD(const string f, Mat &dic,
		Ptr<Feature2D> detector, const int k = 16, const int d = 128)
		: filename(f), clusters(k), dimensions(d), detector(detector)
	{
		computeVLAD(dic);
	}


	~VLAD() {}


	// Todo: redo this
	Mat draw(int resolution = 32)
	{	
		int sqSize = resolution;
		double rad = resolution * resolution / 6;
		int lpd = dimensions / clusters; // linesPerDot
		int thickness = resolution / 32;
		Mat img(sqSize * 4, sqSize * 4 * clusters, CV_8UC3, Scalar::all(255));

		// 
		for (int k = 0; k < clusters; k++) {
			int counter = 0;
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					Point dotCtr(sqSize / 2 + sqSize * (k * 4 + r), sqSize / 2 + sqSize * c);

					//
					for (int p = 1; p <= lpd; p++) {
						double angle = p * 360.0 / lpd;
						float value = vladDesc.at<float>(k, counter++);
						line(img, dotCtr, Point(dotCtr.x + value * rad * cos(angle * CV_PI / 180.0),
							dotCtr.y + value * rad * sin(angle * CV_PI / 180.0)), rOrB(value), thickness);
					}
					circle(img, dotCtr, resolution / 16, Scalar(0), thickness * -(resolution / 32));
				}
			}
			// Drawing border between each cluster square
			line(img, Point(4 * k * sqSize, 0), Point(4 * k * sqSize, 4 * sqSize), Scalar(0), thickness);
		}
		// Drawing the rest of the border
		line(img, Point(4 * clusters * sqSize - 1, 0), Point(4 * clusters * sqSize - 1, 4 * sqSize), Scalar(0));
		line(img, Point(0, 4 * sqSize - 1), Point(4 * clusters * sqSize, 4 * sqSize - 1), Scalar(0));
		line(img, Point(0, 0), Point(4 * clusters * sqSize, 0), Scalar(0));

		imshow(filename, img);
		return img;
	}


	Mat getVLAD()
	{
		return vladDesc;
	}


	// Store .vlad to disk
	void write(const string path = "") {  // Todo: add path
		FileStorage fs(filename + ".vlad", FileStorage::WRITE);
		fs << "VLAD" << vladDesc;
		fs.release();
	}
};
