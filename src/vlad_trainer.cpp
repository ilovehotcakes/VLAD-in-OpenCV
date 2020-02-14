//----------------------------- vlad_trainer.cpp ------------------------------
// Author: Jason Chen
//-----------------------------------------------------------------------------
// This is the VLADTrainer class that is based on Jégou et al.'s paper
// "Aggregating local descriptors into a compact image presentation"(2010). It
// takes a set of images and compute the k-means using BOWKMeansTrainer and
// then generates a codebook of k visual words.
//
// Requirements: OpenCV with contrib module
// Inputs: Directory of images to be trained, a list of the names of images
// Ouputs: codebook, SIFT/SURF descriptors (if specified)
// Limits: (1) The total amount of descriptors must be great than the specified
//             k amount of visual words to train
//         (2) Currently only support SIFT and SURF descriptors
//-----------------------------------------------------------------------------
#include "vlad.cpp"
using namespace cv;
using namespace std;


class VLADTrainer
{
private:
	string dir;      // Location of the images to be trained
	string list;     // List of files to be trained
	string descDir;  // Loc of SIFT/SURF/etc. desc file to be saved to
	Mat codebook;    // Codebook of kVisualWords

public:
	// dir is the directory that the images are located and list is the list of
	// image files to be trained
	VLADTrainer(const string dir, const string list) : dir(dir), list(list) {}
	~VLADTrainer() {}


	// Save computed SIFT/SURF/etc. descriptor disk
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


	// Train codebook; if flag is set to 0, it will save the computed descriptors to disk
	void train(Ptr<Feature2D> detector, const int kVisualWords = 16, const int flag = 0) {
		ifstream file(list);
		string filename;
		Mat allWords;  // Mat that temporarily holds all the SIFT/SURF descriptors
		
		// Compute SIFT/SURF/etc. descriptors for each image
		while (!file.eof()) {
			file >> filename;
			string path = dir + filename;

			// Compute descriptors
			Mat img, desc;
			vector<KeyPoint> keypoints;
			img = imread(path);
			detector->detectAndCompute(img, Mat(), keypoints, desc);

			// Save SIFT/SURF/etc. descriptor to disk located at descDir if flag == 1
			if (flag == 1 && descDir != "") saveDesc(descDir + filename, desc);

			// Store desc in allWords
			allWords.push_back(desc);
		}

		// Compute k-means. allWords.size() must be greater than kVisualWords to train
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


	// Return codebook as Mat
	Mat getBook()
	{
		return codebook;
	}

	
	// Change image director
	void chdir(const string directory)
	{
		dir = directory;
	}


	// Change list of images used to train the codebook
	void chlist(const string newList)
	{
		list = newList;
	}


	// Set the directory for the SIFT/SURF/etc. files to be saved to
	void setDescDir(const string directory)
	{
		descDir = directory;
	}
};