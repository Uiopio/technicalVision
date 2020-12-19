#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

int th = 0;
int lowH = 0, lowS = 0, lowV = 0, highH = 0, highS = 255, highV = 255;
int numOfRed = 0, numOfGreen = 0, numOfBlue = 0;
vector<Point> centersOfMass;

int calcCentersOfMass(vector<vector<Point>> contours, vector<Point> &centers)
{
	if (contours.empty())
	{
		return -1;
	}
	for (int i = 0; i < contours.size(); i++)
	{
		Moments mnts = moments(contours[i]);
		double m00 = mnts.m00;
		double m10 = mnts.m10;
		double m01 = mnts.m01;
		Point center;
		center.x = m10 / m00;
		center.y = m01 / m00;
		centers.push_back(center);
	}
	return 0;
}

void findTarget1(int, void* inputImage)
{
	Mat* source = (Mat*)inputImage;
	Mat sourceClone = (*source).clone();
	imshow("Source", sourceClone);
	Mat sourceGray;
	cvtColor(sourceClone, sourceGray, COLOR_BGR2GRAY);
	//imshow("Source gray", sourceGray);
	Mat sourceBinary;
	threshold(sourceGray, sourceBinary, th, 255, THRESH_BINARY);
	imshow("Binary source", sourceBinary);
	Mat afterErodeAndDilate;
	erode(sourceBinary, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(3, 3)));
	dilate(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(3, 3)));
	imshow("After erode and dilate", afterErodeAndDilate);
	vector <vector<Point>> contours;
	findContours(afterErodeAndDilate, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat drawnContours(afterErodeAndDilate.rows, afterErodeAndDilate.cols, CV_8UC1, Scalar(0));
	polylines(drawnContours, contours, true, Scalar(255), 1);
	imshow("Drawn contours", drawnContours);
	vector<Point> centersOfMass;
	calcCentersOfMass(contours, centersOfMass);
	Mat result;
	result = sourceClone.clone();
	for (int i = 0; i < centersOfMass.size(); i++)
	{
		circle(result, centersOfMass[i], 4, Scalar(0, 0, 255), -1);
	}
	imshow("Result", result);
}

void findTarget2(int, void* inputImage)
{
	Mat* source = (Mat*)inputImage;
	Mat sourceClone = (*source).clone();
	imshow("Source", sourceClone);
	Mat sourceHSV;
	cvtColor(sourceClone, sourceHSV, COLOR_BGR2HSV);
	Mat sourceInRange;
	inRange(sourceHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), sourceInRange);
	imshow("In range", sourceInRange);
	Mat afterErodeAndDilate;
	dilate(sourceInRange, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(7, 7)));
	erode(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(7, 7)));
	erode(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(7, 7)));
	dilate(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(7, 7)));
	imshow("After erode and dilate", afterErodeAndDilate);
	vector <vector<Point>> contours;
	findContours(afterErodeAndDilate, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat drawnContours(afterErodeAndDilate.rows, afterErodeAndDilate.cols, CV_8UC1, Scalar(0));
	polylines(drawnContours, contours, true, Scalar(255), 1);
	imshow("Drawn contours", drawnContours);
	vector<Point> centersOfMass;
	calcCentersOfMass(contours, centersOfMass);
	Mat result;
	result = sourceClone.clone();
	for (int i = 0; i < centersOfMass.size(); i++)
	{
		circle(result, centersOfMass[i], 4, Scalar(0, 0, 0), -1);
	}
	imshow("Result", result);
}

int findObjects(Mat source, Scalar lowHSV, Scalar highHSV, Scalar color, Mat& result)
{
	Mat sourceHSV;
	cvtColor(source, sourceHSV, COLOR_BGR2HSV);
	Mat sourceInRange;
	inRange(sourceHSV, lowHSV, highHSV, sourceInRange);

	Mat afterErodeAndDilate;
	if (color == Scalar(0, 0, 255))
	{
		ellipse(sourceInRange, Point(640, 220), Size(34, 23), -17, 0, 360, Scalar(255, 255, 255), FILLED, 8, 0);
		dilate(sourceInRange, afterErodeAndDilate, getStructuringElement(MORPH_ELLIPSE, Size(12, 10)));
		erode(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_ELLIPSE, Size(14, 12)));
	}
	else if (color == Scalar(0, 0, 0))
	{
		dilate(sourceInRange, afterErodeAndDilate, getStructuringElement(MORPH_ELLIPSE, Size(12, 10)));
		erode(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_ELLIPSE, Size(14, 12)));
	}
	else
	{
		erode(sourceInRange, afterErodeAndDilate, getStructuringElement(MORPH_ELLIPSE, Size(13, 13)));
		dilate(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_ELLIPSE, Size(14, 14)));
	}
	//imshow("After erode and dilate", afterErodeAndDilate);

	Mat afterCanny;
	Canny(afterErodeAndDilate, afterCanny, 100, 1200);

	vector <vector<Point>> contours;
	findContours(afterCanny, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	if (color == Scalar(0, 0, 255))
	{
		numOfRed = contours.size();
	}
	else if (color == Scalar(0, 255, 0))
	{
		numOfGreen = contours.size();
	}
	else if (color == Scalar(255, 0, 0))
	{
		numOfBlue = contours.size();
	}
	
	if (color != Scalar(0, 0, 0))
	{
		polylines(result, contours, true, color, 3);
	}

	calcCentersOfMass(contours, centersOfMass);
	
	if (color == Scalar(0, 0, 0))
	{
		circle(result, centersOfMass[0], 5, color, -1);
	}
	
	return 0;
}

int findClosestRobot(Mat& result)
{
	Point lamp = centersOfMass[0];
	double minR = 100000, minG = 100000, minB = 100000;
	int iR = 0, iG = 0, iB = 0;
	for (int i = 1; i <= numOfRed; i++)
	{
		double distance = sqrt((lamp.x - centersOfMass[i].x) * (lamp.x - centersOfMass[i].x)
			+ (lamp.y - centersOfMass[i].y) * (lamp.y - centersOfMass[i].y));
		if (distance < minR)
		{
			minR = distance;
			iR = i;
		}
	}
	for (int i = numOfRed + 1; i <= numOfRed + numOfGreen; i++)
	{
		double distance = sqrt((lamp.x - centersOfMass[i].x) * (lamp.x - centersOfMass[i].x)
			+ (lamp.y - centersOfMass[i].y) * (lamp.y - centersOfMass[i].y));
		if (distance < minG)
		{
			minG = distance;
			iG = i;
		}
	}
	for (int i = numOfRed + numOfGreen + 1; i < centersOfMass.size(); i++)
	{
		double distance = sqrt((lamp.x - centersOfMass[i].x) * (lamp.x - centersOfMass[i].x)
			+ (lamp.y - centersOfMass[i].y) * (lamp.y - centersOfMass[i].y));
		if (distance < minB)
		{
			minB = distance;
			iB = i;
		}
	}
	line(result, lamp, centersOfMass[iR], Scalar(0, 0, 255));
	line(result, lamp, centersOfMass[iG], Scalar(0, 255, 0));
	line(result, lamp, centersOfMass[iB], Scalar(255, 0, 0));

	return 0;
}

void findTeams(int, void* inputImage)
{
	Mat* source = (Mat*)inputImage;
	Mat sourceClone = (*source).clone();
	imshow("Source", sourceClone);
	Mat sourceHSV;
	cvtColor(sourceClone, sourceHSV, COLOR_BGR2HSV);
	Mat result;
	result = sourceClone.clone();

	findObjects(sourceClone, Scalar(0, 0, 100), Scalar(0, 0, 255), Scalar(0, 0, 0), result);
	findObjects(sourceClone, Scalar(12, 0, 0), Scalar(134, 255, 255), Scalar(0, 0, 255), result);
	findObjects(sourceClone, Scalar(47, 43, 129), Scalar(77, 255, 255), Scalar(0, 255, 0), result);
	findObjects(sourceClone, Scalar(84, 0, 0), Scalar(105, 255, 255), Scalar(255, 0, 0), result);

	findClosestRobot(result);

	imshow("Result", result);
}

void findSpanners(int, void* inputImage)
{
	Mat* source = (Mat*)inputImage;
	Mat sourceClone = (*source).clone();
	imshow("Source", sourceClone);

	Mat sourceGray;
	cvtColor(sourceClone, sourceGray, COLOR_BGR2GRAY);

	Mat tmplt = imread("part4_template.jpg", IMREAD_GRAYSCALE);
	imshow("Template", tmplt);
	Mat tmpltBinary;
	threshold(tmplt, tmpltBinary, 50, 255, THRESH_BINARY);
	vector<vector<Point>> contourTmplt;
	findContours(tmpltBinary, contourTmplt, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat sourceBinary;
	threshold(sourceGray, sourceBinary, 210, 255, THRESH_BINARY_INV);
	imshow("Binary source", sourceBinary);

	Mat afterErodeAndDilate;
	dilate(sourceBinary, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(7, 7)));
	erode(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(7, 7)));
	imshow("After erode and dilate", afterErodeAndDilate);

	vector <vector<Point>> contours;
	findContours(afterErodeAndDilate, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat result = sourceClone.clone();
	for (int i = 0; i < contours.size(); i++)
	{
		double difference = matchShapes(contourTmplt[0], contours[i], CONTOURS_MATCH_I2, 0);
		cout << difference << endl;
		if (difference < 0.1)
		{
			polylines(result, contours[i], true, Scalar(0, 255, 0), 5);
		}
		else
		{
			polylines(result, contours[i], true, Scalar(0, 0, 255), 5);
		}
	}
	imshow("Result", result);
}

int main()
{
	Mat source1 = imread("part1_0.jpg");
	findTarget1(0, &source1);
	createTrackbar("Threshold", "Binary source", &th, 255, findTarget1, &source1);
	while (waitKey(1) != 27)
	{

	}
	destroyAllWindows();
	Mat source2 = imread("part2_3.png");
	findTarget2(0, &source2);
	createTrackbar("low_H", "In range", &lowH, 179, findTarget2, &source2);
	createTrackbar("low_H", "In range", &lowH, 179, findTarget2, &source2);
	createTrackbar("high_H", "In range", &highH, 179, findTarget2, &source2);
	createTrackbar("low_S", "In range", &lowS, 255, findTarget2, &source2);
	createTrackbar("high_S", "In range", &highS, 255, findTarget2, &source2);
	createTrackbar("low_V", "In range", &lowV, 255, findTarget2, &source2);
	createTrackbar("high_V", "In range", &highV, 255, findTarget2, &source2);
	while (waitKey(1) != 27)
	{

	}
	destroyAllWindows();
	Mat source3 = imread("part3_0.jpg");
	findTeams(0, &source3);
	while (waitKey(1) != 27)
	{

	}
	destroyAllWindows();
	Mat source4 = imread("part4.jpg");
	findSpanners(0, &source4);
	//createTrackbar("Threshold", "Binary source", &th, 255, findSpanners, &source4);
	while (waitKey(1) != 27)
	{

	}
	return 0;
}