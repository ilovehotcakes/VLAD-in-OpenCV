// #include "vlad_trainer.cpp"


// int main(int argc, char* argv[]) {
// 	VLAD kittens1("../sift/kittens1.hesaff.sift");
// 	VLAD kittens2("../sift/kittens2.hesaff.sift");

// 	kittens1.drawVLAD();
// 	kittens2.drawVLAD();
// 	waitKey(0);
	
	
// 	//VLAD_trainer jason("list.txt", 16);
// 	getchar();

// 	return 0;
// }


//OPENCV C++ Tutorial:Feature Detector Using SURF Detector
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgproc.hpp>  // line, circle
#include <unistd.h>
#include <string>
#include <fstream>
#include <vector>
using namespace cv;
using namespace std;

Scalar rOrB(float value) {
    return (value > 0)? Scalar(255, 0, 0) : Scalar(0, 0, 255);
}


int main()
{
    // Loading vocabulary.yml into memory(Mat vocabulary)
	Mat vocabulary;
	FileStorage fs1("vocabulary.yml", FileStorage::READ);
	fs1["vocabulary"] >> vocabulary;
	fs1.release();


    // 
	Ptr<Feature2D> detector = xfeatures2d::SIFT::create(); // use int minHess = 400 for SURF
	Ptr<DescriptorExtractor> extractor = xfeatures2d::SIFT::create();
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	BOWImgDescriptorExtractor bowide(extractor, matcher);
	bowide.setVocabulary(vocabulary);
    

	//
    ifstream f1;
    f1.open("../test_images/list.txt");
    

    while(!f1.eof())
    {
        Mat finalV;

        // Compute image SIFT desc
        Mat img, desc1;
        vector<KeyPoint> keypoints;
        string imgName;
        f1 >> imgName;
        cout << "\n" << imgName << endl;
        img = imread("../test_images/" + imgName);
        if(!img.data)
            continue;
        // cvtColor(img, img, COLOR_BGR2GRAY);  // For SURF?
        
        
        detector->detect(img, keypoints);
        detector->compute(img, keypoints, desc1);


        // Convert desc1 to desc1 as float
        // Mat desc1_32f;
        // desc1.convertTo(desc1_32f, CV_32FC1);


        // Compute VLAD
        vector<DMatch> matches;
        matcher->match(desc1, matches); //desc1 contains descriptors for each image
        
        
        Mat responseHist(16, 128, CV_32FC1, Scalar::all(0.0));  // Todo: rename this to fisherVec
        int sampleCount = matches.size();
        for(int i = 0; i < sampleCount; i++)
        {
            int queryIdx = matches[i].queryIdx;
            int trainIdx = matches[i].trainIdx; // cluster index
            for (int d = 0; d < 128; d++)  // Todo: para this
                responseHist.at<float>(trainIdx, d) += (vocabulary.at<float>(trainIdx, d) - desc1.at<float>(queryIdx, d));
        }

        // L2-norm
        normalize(responseHist, responseHist);

        
        FileStorage fs2(imgName + ".vlad", FileStorage::WRITE);  // Store to disk
        fs2 << "responseHist" << responseHist;
        fs2.release();

        finalV = responseHist;
        {
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

            imshow(imgName, img);
            waitKey(0);
        }
    }
    f1.close();
    
		
    return 0;
}
