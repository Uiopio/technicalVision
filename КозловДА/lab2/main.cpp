#include <iostream>
#include "filter.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <time.h>

using namespace cv;
using namespace std;

int main()
{
	clock_t t;

	Mat img = imread("triangles.jpg", IMREAD_GRAYSCALE);
	Otsu otsu;
	otsu.setImage(img);
	t = clock();
	otsu.processImage();
	t = clock() - t;
	cout << (float)t / CLOCKS_PER_SEC << endl;

	Mat otsuOpenCV;
	t = clock();
	threshold(img, otsuOpenCV, 0, 255, THRESH_BINARY + THRESH_OTSU);
	t = clock() - t;
	cout << (float)t / CLOCKS_PER_SEC << endl;

	cout << compare(otsu.getImage(), otsuOpenCV) << endl;

	Mat diff;
	absdiff(otsu.getImage(), otsuOpenCV, diff);

	namedWindow("original", WINDOW_FREERATIO);
	namedWindow("My otsu", WINDOW_FREERATIO);
	namedWindow("OpenCV otsu", WINDOW_FREERATIO);
	namedWindow("Difference", WINDOW_FREERATIO);

	imshow("original", img);
	imshow("My otsu", otsu.getImage());
	imshow("OpenCV otsu", otsuOpenCV);
	imshow("Difference", diff);

	while (waitKey(1) != 27)
	{

	}

	return 0;
}