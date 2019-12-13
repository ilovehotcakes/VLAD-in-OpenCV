// Only able to use codebook that has kVisualWords = clusters

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <fstream>
#include <vector>
using namespace cv;  // Todo: add scope for cv namespace
using namespace std;


// Todo: add namespace for cv and VLAD
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
        matcher->match(desc, matches); //desc contains descriptors for each image
        
		// Compute VLAD descriptors
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

	
	// Helper function for draw
	Scalar rOrB(float value)
	{
		// Blue if positive, else red
		return (value > 0)? Scalar(255, 0, 0) : Scalar(0, 0, 255);
	}


	// Helper function for draw
	int howManySq(int value)
	{
		return (value % 128 == 0)? (value / 128) : (value / 128 + 1);
	}


public:
	// VLAD constructor that takes a path or file and load the codebook
	VLAD(const string f, const string dic,
		Ptr<Feature2D> detector, const int k = 16, const int d = 128)
		: filename(f), clusters(k), dimensions(d), detector(detector)
	{
		Mat input = readBook(dic);
		computeVLAD(input);
	}


	// VLAD constructor that takes a codebook of Mat
	VLAD(const string f, Mat &dic,
		Ptr<Feature2D> detector, const int k = 16, const int d = 128)
		: filename(f), clusters(k), dimensions(d), detector(detector)
	{
		computeVLAD(dic);
	}


	~VLAD() {}


	// Todo: redo this
	Mat draw(int sqSize = 32, int thickness = 1)
	{
		double rad = sqSize * 5;
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
						line(img, dotCtr, Point(dotCtr.x + abs(value) * rad * cos(angle * CV_PI / 180.0),
							dotCtr.y + abs(value) * rad * sin(angle * CV_PI / 180.0)), rOrB(value), thickness);
					}
					circle(img, dotCtr, sqSize / 16, Scalar(0), thickness * -(sqSize / 32));
				}
			}
			// Drawing borders between each cluster square
			line(img, Point(4 * k * sqSize, 0), Point(4 * k * sqSize, 4 * sqSize), Scalar(0), thickness);
		}
		// Drawing the rest of the border
		line(img, Point(hms * 4 * sqSize - thickness, 0), Point(hms * 4 * sqSize - thickness, 4 * sqSize), Scalar(0), thickness); // Right
		line(img, Point(0, 4 * sqSize - thickness), Point(hms * 4 * sqSize, 4 * sqSize - thickness), Scalar(0), thickness);      // Bottom
		line(img, Point(0, 0), Point(hms * 4 * sqSize, 0), Scalar(0), thickness);  // Top

		imshow(filename, img);
		return img;
	}


	Mat getVLAD()
	{
		return vladDesc;
	}


	// Store VLAD to disk
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
