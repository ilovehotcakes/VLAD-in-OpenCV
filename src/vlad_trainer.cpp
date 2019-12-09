#include "vlad.cpp"
#include <opencv2/features2d.hpp>
#include <iostream>
using namespace cv;  // Todo: add scope for cv namespace
using namespace std;


// Todo: 
class VLAD_trainer
{
private:
	string dir;
	int kVisualWords;
	Mat codebook;


public:
	VLAD_trainer() {}
	~VLAD_trainer() {}


	void compute(const string dir, Ptr<Feature2D> detector, const int k = 16) {
		kVisualWords = k;
		ifstream file(dir);
		string filename;
		Mat allWords;  // Vector that temporarily holds all the SIFT/SURF descriptors
		
		// Compute SIFT/SURF/etc. descriptors for each image
		while (!file.eof()) {
			file >> filename;
			string path = "../test_images/" + filename;  // Todo: param path

			// Compute descriptors
			Mat img, desc;
			vector<KeyPoint> keypoints;
			img = imread(path);
			detector->detectAndCompute(img, Mat(), keypoints, desc);

			// Store VLAD in the words
			allWords.push_back(desc);
			cout << "\t" << filename << endl;  // Todo: debuguse
		}
		
		// Compute k-means. There needs to be at least kVisualWords of lines to train
		BOWKMeansTrainer bow(kVisualWords);
		codebook = bow.cluster(allWords);
	}


	//write codebook to disk
	void write(const string filename)
	{
		FileStorage fs(filename + ".yml", FileStorage::WRITE);
		fs << "codebook" << codebook;
		fs.release();
	}


	Mat getBook()
	{
		return codebook;
	}
};