#include "vlad.cpp"
#include <opencv2/features2d.hpp>
#include <iostream>

using namespace cv;  // Todo: add scope for cv namespace
using namespace std;


class VLAD_trainer
{
private:
	string dir;      // Location of the images to be trained
	string list;     // List of files to be trained
	string descDir;  // Loc of SIFT/SURF/etc. desc file to be saved to
	Mat codebook;    // Codebook of kVisualWords

public:
	VLAD_trainer(const string dir, const string list) : dir(dir), list(list) {}
	~VLAD_trainer() {}


	void saveDesc(const string filename, const Mat &finalMat)
	{
		ofstream f(filename + ".sift");
		int rows = finalMat.rows;
		int dimensions = finalMat.cols;
		for (int i = 0; i < rows; i++) {
			for (int d = 0; d < dimensions; d++)
				f << finalMat.at<float>(i, d) << " ";
			f << endl;
		}
		f.close();
	}


	void train(Ptr<Feature2D> detector, const int kVisualWords = 16, const int flag = 0) {
		ifstream file(list);
		string filename;
		Mat allWords;  // Vector that temporarily holds all the SIFT/SURF descriptors
		
		// Compute SIFT/SURF/etc. descriptors for each image
		while (!file.eof()) {
			file >> filename;
			string path = dir + filename;  // Todo: param path

			// Compute descriptors
			Mat img, desc;
			vector<KeyPoint> keypoints;
			img = imread(path);
			detector->detectAndCompute(img, Mat(), keypoints, desc);

			// Save SIFT/SURF/etc. descriptor to descDir if flag == 1
			if (flag == 1) saveDesc(descDir + filename, desc);

			// Store VLAD in allWords
			allWords.push_back(desc);
			
			// Compute k-means. There needs to be at least kVisualWords of lines to train
			BOWKMeansTrainer bow(kVisualWords);
			codebook = bow.cluster(allWords);
		}
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


	void chdir(const string directory)
	{
		dir = directory;
	}


	void chlist(const string newList)
	{
		list = newList;
	}


	void setDescDir(const string directory)
	{
		descDir = directory;
	}
};