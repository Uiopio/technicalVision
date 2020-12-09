#pragma once

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

class openingFilter
{
public:
	void addBoarder(); 
	void deleteBoarder();
	void erodeCustom();
	void dilateCustom();
	void erodeOpencv();
	void dilateOpencv();
	void openingCustom(Mat src, Mat kernel);
	void openingOpenCV(Mat src, Mat kernel);

	void setImage(Mat src);
	void setKernel(Mat kernel);
	Mat getImage();
	Mat getKernel();
private:
	Mat m_image;
	Mat m_kernel;
};

