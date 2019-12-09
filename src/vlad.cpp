#include <opencv2/core.hpp>  // kmeans
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <fstream>
#include <vector>
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

	
	// Todo: Use any descriptor


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
	// Todo: change inputArray from sift file to actual image, add which descriptor to use, SIFT/SURF/etc.
	void computeVLAD(const Mat &codebook)
	{
		// Use Opencv BOWImgDescriptorExtractor to match detected points to vocab clusters
		// Ptr<Feature2D> detector = xfeatures2d::SIFT::create(); // use int minHess = 400 for SURF
		Ptr<DescriptorExtractor> extractor = detector;
		Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
		BOWImgDescriptorExtractor bowide(extractor, matcher);
		bowide.setVocabulary(codebook);
		
		// Compute SIFT/SURF descriptors
        Mat img, desc;
        vector<KeyPoint> keypoints;
        img = imread(filename);
        // cvtColor(img, img, COLOR_BGR2GRAY);  // For SURF?
        detector->detectAndCompute(img, Mat(), keypoints, desc);

        // Convert desc to desc as float
        // Mat desc_32f;
        // desc.convertTo(desc_32f, CV_32FC1);

        // Compute VLAD descriptors
        vector<DMatch> matches;
        matcher->match(desc, matches); //desc contains descriptors for each image
        
        Mat fisherVec(clusters, dimensions, CV_32FC1, Scalar::all(0.0));
        int sampleCount = matches.size();
        for(int i = 0; i < sampleCount; i++)
        {
            int queryIdx = matches[i].queryIdx;
            int trainIdx = matches[i].trainIdx;
            for (int d = 0; d < dimensions; d++)
                fisherVec.at<float>(trainIdx, d) += (codebook.at<float>(trainIdx, d) - desc.at<float>(queryIdx, d));
        }

		// Normalize vector using L2-norm
		normalize(fisherVec, vladDesc);
	}

	// Helper function for drawVLAD
	Scalar rOrB(float value)
	{
		// Red if positive, else blue
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


	// Todo: parameterize the line thickness, and center dot radius
	void drawVLAD(int resolution = 32)
	{
		int sqSize = resolution;
		double rad = sqSize * 6;
		double slantedRad = rad * cos(CV_PI / 4);
		int thickness = 1;
		Mat img(sqSize * 4, sqSize * 64, CV_8UC3, Scalar::all(255));


		// 
		for (int i = 0; i < clusters; i++) {
			int counter = 0;
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					Point sqCtr(sqSize / 2 + (i * 4 + r) * sqSize, sqSize / 2 + c * sqSize);
					{
						line(img, sqCtr, Point(sqCtr.x + slantedRad * abs(vladDesc.at<float>(i, counter)), sqCtr.y + slantedRad * abs(vladDesc.at<float>(i, counter))), rOrB(vladDesc.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - slantedRad * abs(vladDesc.at<float>(i, counter)), sqCtr.y + slantedRad * abs(vladDesc.at<float>(i, counter))), rOrB(vladDesc.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x + slantedRad * abs(vladDesc.at<float>(i, counter)), sqCtr.y - slantedRad * abs(vladDesc.at<float>(i, counter))), rOrB(vladDesc.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - slantedRad * vladDesc.at<float>(i, counter), sqCtr.y - slantedRad * vladDesc.at<float>(i, counter)), rOrB(vladDesc.at<float>(i, counter)), thickness);
						counter++;

						line(img, sqCtr, Point(sqCtr.x, sqCtr.y + rad * abs(vladDesc.at<float>(i, counter))), rOrB(vladDesc.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x, sqCtr.y - rad * abs(vladDesc.at<float>(i, counter))), rOrB(vladDesc.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - abs(vladDesc.at<float>(i, counter)) * rad, sqCtr.y), rOrB(vladDesc.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x + abs(vladDesc.at<float>(i, counter)) * rad, sqCtr.y), rOrB(vladDesc.at<float>(i, counter)), thickness);
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
		return vladDesc;
	}

	
	// Return the requested cluster as a Mat
	Mat getVLAD(const int k)
	{
		if (k >= 0 && k < clusters) {
			Mat subVLAD;
			for (int d = 0; d < dimensions; d++)
				subVLAD.push_back(vladDesc.at<float>(k, d));
			return subVLAD;
		}
		return Mat();
	}


	// Store .vlad to disk
	void write() {
		FileStorage fs(filename + ".vlad", FileStorage::WRITE);
		fs << "VLAD-SURF" << vladDesc;
		fs.release();
	}

	void compareVLAD(VLAD &comp)
	{
		// Todo: use knn
	}
};
