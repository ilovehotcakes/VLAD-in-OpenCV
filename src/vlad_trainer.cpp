#include "vlad.cpp"
#include <opencv2/features2d.hpp>
#include <fstream>
#include <iostream>
using namespace cv;  // Todo: add scope for cv namespace
using namespace std;


// Todo: 
class VLAD_trainer
{
private:
	const string dir;
	const int kVisualWords;
	Mat codebook;
	const int clusters = 16;
	const int dimensions = 128;

	void train() {
		ifstream file(dir);
		string name;
		int lines;
		file >> lines;

		Mat words(lines * clusters, dimensions, CV_32FC1);  // Vector that temporarily holds all the VLAD descriptors

		// Compute a VLAD descriptor for each images and store in vector
		for (int w = 0; w < lines; w++) {
			file >> name;
			string path = "../ukbench_sift/" + name;

			// Compute VLAD descriptor
			VLAD vlad(path);
			
			// Store VLAD in the words
			for (int k = 0; k < clusters; k++) {
				for (int d = 0; d < dimensions; d++)
					words.at<float>(w * clusters + k, d) = vlad.getVLAD().at<float>(k, d);
			}
		}
		
		// Compute k-means. there needs to be at least 16 .sift files to train
		//kmeans(words, kVisualWords, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1), 10, KMEANS_PP_CENTERS, centers);
		BOWKMeansTrainer bow(kVisualWords);
		codebook = bow.cluster(words);
		
				
		cout << "start" << endl;
		FileStorage fs("dictionary.yml", FileStorage::WRITE);
		fs << "vocabulary" << codebook;
		fs.release();
		cout << "done" << endl;
	}


public:
	VLAD_trainer(const string dir, const int k) : dir(dir), kVisualWords(kVisualWords) {
		train();
	}

	//write codebook to disk
};