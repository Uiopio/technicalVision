#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

int th = 0;
int lowTh = 0;
int highTh = 0;
int low_H = 0; int low_S = 0; int low_V = 0;
int high_H = 179; int high_S = 255; int high_V = 255;

Mat img_1, img_2, img_3, img_4, img_4_tmplt;

vector <vector<Point>> contours_BLUE;
vector <vector<Point>> contours_GREEN;
vector <vector<Point>> contours_RED;

vector<Point> calcMoments(vector<vector<Point>> contours)
{
	vector<Point> centersOfMass;
	for (int i = 0; i < contours.size(); i++)
	{
		Moments mnts = moments(contours[i]);
		double m00 = mnts.m00;
		double m10 = mnts.m10;
		double m01 = mnts.m01;
		Point center;
		center.x = m10 / m00;
		center.y = m01 / m00;
		centersOfMass.push_back(center);
	}
	return centersOfMass;
}

int findMinIndex(Point lamp, vector<Point> var)
{
	vector<double> distances;
	for (int i = 0; i < var.size(); i++)
	{
		double x1 = lamp.x;
		double x2 = var[i].x;
		double y1 = lamp.y;
		double y2 = var[i].y;
		double result = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		distances.push_back(result);
		cout << distances[i] << endl;
	}

	int indexMin = 0;
	for (int i = 0; i < distances.size(); i++)
	{
		if (distances[i] <= distances[indexMin])
		{
			indexMin = i;
			cout << i << endl;
		}
	}
	return indexMin;
}

void func(Mat& inputImage, Mat& res, Scalar low, Scalar high, Size forErode, Size forDilate, string flag)
{
	if (flag == "BLUE")
	{
		Mat img_3HSV_BLUE, resultInRange_BLUE;
		cvtColor(inputImage, img_3HSV_BLUE, COLOR_BGR2HSV);
		inRange(img_3HSV_BLUE, low, high, resultInRange_BLUE);
		Mat afterErodeAndDilate_BLUE;
		erode(resultInRange_BLUE, afterErodeAndDilate_BLUE, getStructuringElement(MORPH_ELLIPSE, forErode));
		dilate(afterErodeAndDilate_BLUE, afterErodeAndDilate_BLUE, getStructuringElement(MORPH_ELLIPSE, forDilate));
		Mat afterCanny_BLUE;
		Canny(afterErodeAndDilate_BLUE, afterCanny_BLUE, lowTh, highTh, 3, false);
		Mat cloneAfterCanny_BLUE = afterCanny_BLUE.clone();
		findContours(cloneAfterCanny_BLUE, contours_BLUE, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		polylines(res, contours_BLUE, true, Scalar(255, 0, 0), 3);
	}	
	if (flag == "GREEN")
	{
		Mat img_3HSV_GREEN, resultInRange_GREEN;
		cvtColor(inputImage, img_3HSV_GREEN, COLOR_BGR2HSV);
		inRange(img_3HSV_GREEN, low, high, resultInRange_GREEN);
		Mat afterErodeAndDilate_GREEN;
		erode(resultInRange_GREEN, afterErodeAndDilate_GREEN, getStructuringElement(MORPH_ELLIPSE, forErode));
		dilate(afterErodeAndDilate_GREEN, afterErodeAndDilate_GREEN, getStructuringElement(MORPH_ELLIPSE, forDilate));
		Mat afterCanny_GREEN;
		Canny(afterErodeAndDilate_GREEN, afterCanny_GREEN, lowTh, highTh, 3, false);
		Mat cloneAfterCanny_GREEN = afterCanny_GREEN.clone();
		findContours(cloneAfterCanny_GREEN, contours_GREEN, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		polylines(res, contours_GREEN, true, Scalar(0, 255, 0), 3);
	}
	if (flag == "RED")
	{
		Mat img_3HSV_RED, resultInRange_RED;
		cvtColor(inputImage, img_3HSV_RED, COLOR_BGR2HSV);
		inRange(img_3HSV_RED, low, high, resultInRange_RED);
		Mat afterErodeAndDilate_RED;
		dilate(resultInRange_RED, afterErodeAndDilate_RED, getStructuringElement(MORPH_ELLIPSE, forDilate));
		erode(afterErodeAndDilate_RED, afterErodeAndDilate_RED, getStructuringElement(MORPH_ELLIPSE, forErode));
		Mat afterCanny_RED;
		Canny(afterErodeAndDilate_RED, afterCanny_RED, lowTh, highTh, 3, false);
		Mat cloneAfterCanny_RED = afterCanny_RED.clone();
		findContours(cloneAfterCanny_RED, contours_RED, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		polylines(res, contours_RED, true, Scalar(0, 0, 255), 3);
	}
}

void proc_img_1(int, void* inputImage)
{
	Mat* image = (Mat*)inputImage;
	Mat cloneImg_2BGR = (*image).clone();
	Mat thimg;
	threshold(cloneImg_2BGR, thimg, th, 255, THRESH_BINARY);
	imshow("thimg", thimg);
	Mat afterErodeAndDilate;
	erode(thimg, afterErodeAndDilate, Mat(), Point(1, 1), 3);
	dilate(afterErodeAndDilate, afterErodeAndDilate, Mat(), Point(1, 1), 3);
	imshow("afterErodeAndDilate", afterErodeAndDilate);
	Mat edges;
	Canny(afterErodeAndDilate, edges, lowTh, highTh, 3, false);
	//imshow("edges", edges);
	Mat cloneEdges = edges.clone();
	vector <vector<Point>> contours;
	findContours(cloneEdges, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat fff(Size(cloneEdges.cols, cloneEdges.rows), CV_8UC1, Scalar());
	polylines(fff, contours, true, Scalar(255), 1);
	imshow("fff", fff);
	int contours_size = contours.size();
	Mat cloneImg = img_1.clone();
	if (contours_size == 1)
	{
		Moments mnts = moments(contours[0]);
		double m00 = mnts.m00;
		double m10 = mnts.m10;
		double m01 = mnts.m01;
		Point centerOfMass;
		centerOfMass.x = m10 / m00;
		centerOfMass.y = m01 / m00;
		circle(cloneImg, centerOfMass, 2, Scalar(0), FILLED);
		imshow("Result", cloneImg);
	}
	else
		imshow("Result", cloneImg);
}

void proc_img_2(int, void* inputImage)
{
	Mat* image = (Mat*)inputImage;
	Mat cloneImg_2BGR = (*image).clone();
	Mat img_2HSV, resultInRange;
	cvtColor(cloneImg_2BGR, img_2HSV, COLOR_BGR2HSV);
	inRange(img_2HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), resultInRange);
	Mat afterErodeAndDilate;
	dilate(resultInRange, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(6, 6)), Point(-1, -1), 1);
	erode(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_RECT, Size(6, 6)), Point(-1, -1), 1);
	Mat afterCanny;
	Canny(afterErodeAndDilate, afterCanny, lowTh, highTh, 3, false);
	imshow("afterCanny", afterCanny);
	Mat cloneAfterCanny = afterCanny.clone();
	vector <vector<Point>> contours_2;
	findContours(cloneAfterCanny, contours_2, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat fff(Size(cloneAfterCanny.cols, cloneAfterCanny.rows), CV_8UC1, Scalar());
	polylines(fff, contours_2, true, Scalar(255), 1);
	imshow("contour", fff);
	int contours_size = contours_2.size();
	Mat cloneImg_2 = img_2.clone();
	if (contours_size > 0)
	{
		for (int i = 0; i < contours_size; i++)
		{
			Moments mnts = moments(contours_2[i]);
			double m00 = mnts.m00;
			double m10 = mnts.m10;
			double m01 = mnts.m01;
			Point centerOfMass;
			centerOfMass.x = m10 / m00;
			centerOfMass.y = m01 / m00;
			circle(cloneImg_2, centerOfMass, 2, Scalar(0), FILLED);
		}
		imshow("Result", cloneImg_2);
	}
	else
		imshow("Result", cloneImg_2);

	imshow("ResultInRange", resultInRange);
	imshow("afterErodeAndDilate", afterErodeAndDilate);
}

void proc_img_3(int, void* inputImage)
{
	Mat* image = (Mat*)inputImage;
	Mat cloneImg_3BGR = (*image).clone();
	Mat result = (cloneImg_3BGR).clone();

	// Поиск лампы-----------------------------
	Mat img_3HSV_Lamp, resultInRangeLamp;
	cvtColor(cloneImg_3BGR, img_3HSV_Lamp, COLOR_BGR2HSV);
	inRange(img_3HSV_Lamp, Scalar(0, 0, 250), Scalar(0, 0, 255), resultInRangeLamp);

	Mat afterErodeAndDilateLamp;
	erode(resultInRangeLamp, afterErodeAndDilateLamp, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 1);
	dilate(afterErodeAndDilateLamp, afterErodeAndDilateLamp, getStructuringElement(MORPH_RECT, Size(3, 3)), Point(-1, -1), 1);

	Mat afterCannyLamp;
	Canny(afterErodeAndDilateLamp, afterCannyLamp, lowTh, highTh, 3, false);
	vector <vector<Point>> contours_lamp;
	Mat cloneAfterCannyLamp = afterCannyLamp.clone();
	findContours(cloneAfterCannyLamp, contours_lamp, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Moments mnts = moments(contours_lamp[0]);
	double m00 = mnts.m00;
	double m10 = mnts.m10;
	double m01 = mnts.m01;
	Point centerOfMassLamp;
	centerOfMassLamp.x = m10 / m00;
	centerOfMassLamp.y = m01 / m00;
	ellipse(cloneImg_3BGR, Point(centerOfMassLamp.x, centerOfMassLamp.y - 28), Size(33, 21), -10, 0, 360, Scalar(102, 25, 65), FILLED);
	circle(cloneImg_3BGR, centerOfMassLamp, 10, Scalar(0, 255, 255), FILLED);
	circle(result, centerOfMassLamp, 10, Scalar(0, 255, 255), FILLED);

	//Поиск красных крышек--------------------
	Mat img_3HSV_RED, resultInRange_RED, afterErodeAndDilate_RED, afterCanny_RED;
	cvtColor(cloneImg_3BGR, img_3HSV_RED, COLOR_BGR2HSV);
	inRange(img_3HSV_RED, Scalar(12, 0, 0), Scalar(136, 255, 255), resultInRange_RED);

	dilate(resultInRange_RED, afterErodeAndDilate_RED, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));
	erode(afterErodeAndDilate_RED, afterErodeAndDilate_RED, getStructuringElement(MORPH_ELLIPSE, Size(12, 12)));
	
	Canny(afterErodeAndDilate_RED, afterCanny_RED, lowTh, highTh, 3, false);

	vector <vector<Point>> contours_RED;
	Mat cloneAfterCanny_RED = afterCanny_RED.clone();
	findContours(cloneAfterCanny_RED, contours_RED, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	polylines(result, contours_RED, true, Scalar(0, 0, 255), 3);

	//Поиск синих крышек--------------------
	Mat img_3HSV_BLUE, resultInRange_BLUE, afterErodeAndDilate_BLUE, afterCanny_BLUE;
	cvtColor(cloneImg_3BGR, img_3HSV_BLUE, COLOR_BGR2HSV);
	inRange(img_3HSV_BLUE, Scalar(84, 0, 0), Scalar(105, 255, 255), resultInRange_BLUE);

	erode(resultInRange_BLUE, afterErodeAndDilate_BLUE, getStructuringElement(MORPH_ELLIPSE, Size(13, 13)));
	dilate(afterErodeAndDilate_BLUE, afterErodeAndDilate_BLUE, getStructuringElement(MORPH_ELLIPSE, Size(14, 14)));

	Canny(afterErodeAndDilate_BLUE, afterCanny_BLUE, lowTh, highTh, 3, false);

	vector <vector<Point>> contours_BLUE;
	Mat cloneAfterCanny_BLUE = afterCanny_BLUE.clone();
	findContours(cloneAfterCanny_BLUE, contours_BLUE, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	polylines(result, contours_BLUE, true, Scalar(255, 0, 0), 3);
	
	//Поиск зеленых крышек--------------------
	Mat img_3HSV_GREEN, resultInRange_GREEN, afterErodeAndDilate_GREEN, afterCanny_GREEN;
	cvtColor(cloneImg_3BGR, img_3HSV_GREEN, COLOR_BGR2HSV);
	inRange(img_3HSV_GREEN, Scalar(47, 43, 129), Scalar(77, 255, 255), resultInRange_GREEN);

	erode(resultInRange_GREEN, afterErodeAndDilate_GREEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(afterErodeAndDilate_GREEN, afterErodeAndDilate_GREEN, getStructuringElement(MORPH_ELLIPSE, Size(7, 7)));

	Canny(afterErodeAndDilate_GREEN, afterCanny_GREEN, lowTh, highTh, 3, false);

	vector <vector<Point>> contours_GREEN;
	Mat cloneAfterCanny_GREEN = afterCanny_GREEN.clone();
	findContours(cloneAfterCanny_GREEN, contours_GREEN, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	polylines(result, contours_GREEN, true, Scalar(0, 255, 0), 3);
	
	//Поиск ближайшего до лампы
	vector<Point> centersOfMassRED;
	vector<Point> centersOfMassBLUE;
	vector<Point> centersOfMassGREEN;
	centersOfMassRED = calcMoments(contours_RED);
	centersOfMassBLUE = calcMoments(contours_BLUE);
	centersOfMassGREEN = calcMoments(contours_GREEN);
	Point centerResult(result.cols/2, result.rows/2);
	circle(result, centerResult, 5, Scalar(0, 0, 255), FILLED);
	int minElementIndexRED = findMinIndex(centerResult, centersOfMassRED);
	int minElementIndexBLUE = findMinIndex(centerResult, centersOfMassBLUE);
	int minElementIndexGREEN = findMinIndex(centerResult, centersOfMassGREEN);
	circle(result, centersOfMassRED[minElementIndexRED], 5, Scalar(0, 0, 255), FILLED);
	circle(result, centersOfMassBLUE[minElementIndexBLUE], 5, Scalar(255, 0, 0), FILLED);
	circle(result, centersOfMassGREEN[minElementIndexGREEN], 5, Scalar(0, 255, 0), FILLED);
	line(result, centerResult, centersOfMassRED[minElementIndexRED], Scalar(0, 0, 255), 1);
	line(result, centerResult, centersOfMassBLUE[minElementIndexBLUE], Scalar(255, 0, 0), 1);
	line(result, centerResult, centersOfMassGREEN[minElementIndexGREEN], Scalar(0, 255, 0), 1);
	imshow("result", result);
}

void proc_img_4(int, void* inputImage)
{
	Mat* image = (Mat*)inputImage;

	Mat tmplt(Size((*image).cols, (*image).rows), CV_8UC1, Scalar());
	Rect rectangle(0, 0, img_4_tmplt.cols, img_4_tmplt.rows);
	img_4_tmplt.copyTo(tmplt(rectangle));

	Mat forThresholdTmplt;
	threshold(tmplt, forThresholdTmplt, 89, 255, THRESH_BINARY);
	vector <vector<Point>> contours_tmplt;
	Mat afterCannyTmplt;
	Canny(forThresholdTmplt, afterCannyTmplt, lowTh, highTh, 3, false);
	Mat cloneAfterCannyTmplt = afterCannyTmplt.clone();
	findContours(cloneAfterCannyTmplt, contours_tmplt, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat contours1(Size(cloneAfterCannyTmplt.cols, cloneAfterCannyTmplt.rows), CV_8UC1, Scalar());
	polylines(contours1, contours_tmplt, true, Scalar(255), 1);
	imshow("contours1", contours1);

	Mat forThreshold;
	threshold((*image), forThreshold, 216, 255, THRESH_BINARY);
	Mat afterErodeAndDilate;
	erode(forThreshold, afterErodeAndDilate, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)), Point(-1, -1), 8);
	dilate(afterErodeAndDilate, afterErodeAndDilate, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)), Point(-1, -1), 8);
	vector <vector<Point>> contours_img;
	Mat afterCanny;
	Canny(forThreshold, afterCanny, lowTh, highTh, 3, false);
	Mat cloneAfterCanny = afterCanny.clone();
	findContours(cloneAfterCanny, contours_img, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Mat contours2(Size(cloneAfterCanny.cols, cloneAfterCanny.rows), CV_8UC1, Scalar());
	polylines(contours2, contours_img, true, Scalar(255), 1);
	imshow("contours2", contours2);

	Mat result = (*image).clone();
	cvtColor(result, result, COLOR_GRAY2BGR);
	int contours_img_size = contours_img.size();
	for (int i = 0; i < contours_img_size; i++)
	{
		double compare = matchShapes(contours_tmplt[0], contours_img[i], CONTOURS_MATCH_I2, 0);
		Moments mnts = moments(contours_img[i]);
		double m00 = mnts.m00;
		double m10 = mnts.m10;
		double m01 = mnts.m01;
		Point centerOfMass;
		centerOfMass.x = m10 / m00;
		centerOfMass.y = m01 / m00;
		if (compare < 0.1)
		{
			polylines(result, contours_img[i], true, Scalar(255, 0, 0), 3);
			circle(result, centerOfMass, 3, Scalar(255, 0, 0), FILLED);
		}
		else
		{
			polylines(result, contours_img[i], true, Scalar(0, 0, 255), 3);
			circle(result, centerOfMass, 3, Scalar(0, 0, 255), FILLED);
		}
	}
	imshow("result", result);
}

int main()
{
	while (waitKey() != 27)
	{
		int a;
		int b;
		cin >> b;
		a = b;
		switch (a)
		{
		case 1:
			img_1 = imread("ig_2.jpg", 0);
			proc_img_1(0, &img_1);
			createTrackbar("th", "thimg", &th, 255, proc_img_1, &img_1);
			createTrackbar("lowTh", "edges", &lowTh, 1500, proc_img_1, &img_1);
			createTrackbar("highTh", "edges", &highTh, 1500, proc_img_1, &img_1);
			break;
		case 2:
			img_2 = imread("task2_5.jpg");
			imshow("img_2", img_2);
			proc_img_2(0, &img_2);

			createTrackbar("low_H", "ResultInRange", &low_H, 179, proc_img_2, &img_2);
			createTrackbar("low_H", "ResultInRange", &low_H, 179, proc_img_2, &img_2);
			createTrackbar("high_H", "ResultInRange", &high_H, 179, proc_img_2, &img_2);
			createTrackbar("low_S", "ResultInRange", &low_S, 255, proc_img_2, &img_2);
			createTrackbar("high_S", "ResultInRange", &high_S, 255, proc_img_2, &img_2);
			createTrackbar("low_V", "ResultInRange", &low_V, 255, proc_img_2, &img_2);
			createTrackbar("high_V", "ResultInRange", &high_V, 255, proc_img_2, &img_2);
			break;
		case 3:
			img_3 = imread("task3_1.jpg");
			imshow("img_3", img_3);
			proc_img_3(0, &img_3);
			break;
		case 4:
			img_4 = imread("gk.jpg", 0);
			img_4_tmplt = imread("gk_tmplt.jpg", 0);
			imshow("img_4", img_4);
			proc_img_4(0, &img_4);
			break;
		}
	}

	return 0;
}
