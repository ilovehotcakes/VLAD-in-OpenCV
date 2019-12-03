#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>  // substr
#include "vlad.cpp"
using namespace std;
using namespace cv;


int main(int argc, char* argv[]) {
	ifstream file(argv[1]);
	string name;
	int lines;
	file >> lines;

	for (int w = 0; w < lines; w++) {
		file >> name;
		string path = "../ukbench_sift/" + name;

		VLAD vlad(path);
		

		string suffix = ".vlad";
        string filename = name.substr(0, name.size() - 16) + suffix;
             
        ofstream out(filename);
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 128; j++) 
				out << vlad.getVLAD().at<float>(i, j) << " ";
			out << endl;
		}



	
		// Track progress
		if (w % 102 == 0)
			cout << "." << flush;
		else if (w % 1020 == 0)
	 		cout << w / 1020 << "%" << flush;
	}
	cout << "  Done" << endl;
	return 0;
}