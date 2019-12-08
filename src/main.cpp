#include <iostream>
#include <vector>
#include "vlad_trainer.cpp"

Scalar rOrB(float value)
{
	return (value > 0) ? Scalar(0, 0, 255) : Scalar(255, 0, 0);
}

int main(int argc, char* argv[]) {
	/*VLAD kittens1("../sift/kittens1.hesaff.sift");
	VLAD kittens2("../sift/kittens2.hesaff.sift");

	kittens1.drawVLAD();
	kittens2.drawVLAD();
	waitKey(0);*/
	
	
	VLAD_trainer jason("list.txt", 16);

	Mat codebook = jason.getBook();

	for (int i = 0; i < 16; i++) {
		VLAD vlad("../sift/kittens1.hesaff.sift");
		Mat finalV = vlad.getVLAD();
		vector<float> k7;
		for (int d = 0; d < 128; d++) {
			k7.push_back(finalV.at<float>(i, d));
		}

		vector<int> indices;
		vector<float> dists;

		flann::Index flann_index(codebook, flann::KDTreeIndexParams());
		flann_index.knnSearch(k7, indices, dists, 16, flann::SearchParams(64));

		for (int d = 0; d < 3; d++) {
			cout << i << ": " << indices.at(d) << "  ";
		}
		cout << endl;
	}


	for (int i = 0; i < 16; i++) {
		VLAD vlad("../sift/kittens2.hesaff.sift");
		Mat finalV = vlad.getVLAD();
		vector<float> k7;
		for (int d = 0; d < 128; d++) {
			k7.push_back(finalV.at<float>(i, d));
		}

		vector<int> indices;
		vector<float> dists;

		flann::Index flann_index(codebook, flann::KDTreeIndexParams());
		flann_index.knnSearch(k7, indices, dists, 16, flann::SearchParams(64));

		for (int d = 0; d < 3; d++) {
			cout << i << ": " << indices.at(d) << "  ";
		}
		cout << endl;
	}

	{
		Mat finalV = codebook;
		int sqSize = 32;
		double rad = sqSize * 8;
		double slantedRad = rad * cos(CV_PI / 4);
		int thickness = 1;
		Mat img(sqSize * 4, sqSize * 64, CV_8UC3, Scalar::all(255));


		for (int i = 0; i < 16; i++) {
			int counter = 0;
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					Point sqCtr(sqSize / 2 + (i * 4 + r) * sqSize, sqSize / 2 + c * sqSize);
					{
						line(img, sqCtr, Point(sqCtr.x + slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y + slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y + slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x + slantedRad * abs(finalV.at<float>(i, counter)), sqCtr.y - slantedRad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - slantedRad * finalV.at<float>(i, counter), sqCtr.y - slantedRad * finalV.at<float>(i, counter)), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;

						line(img, sqCtr, Point(sqCtr.x, sqCtr.y + rad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x, sqCtr.y - rad * abs(finalV.at<float>(i, counter))), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x - abs(finalV.at<float>(i, counter)) * rad, sqCtr.y), rOrB(finalV.at<float>(i, counter)), thickness);
						counter++;
						line(img, sqCtr, Point(sqCtr.x + abs(finalV.at<float>(i, counter)) * rad, sqCtr.y), rOrB(finalV.at<float>(i, counter)), thickness);
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


		imshow("codebook", img);
		waitKey(0);
	}


	getchar();

	return 0;
}