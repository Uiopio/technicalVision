#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class ClosingProcess	// Object is white
{
public:
	static const int OBJ;
	static const int BG;
	static const int CORE[9];
	static const int BORDER_WIDTH;

public:
	// 'src' without border, 'res' may be empty
	static int useClosingFilter(Mat& src, Mat& res);
	static int dilate(Mat& src, Mat& res);
	static int erode(Mat& src, Mat& res);

	// 'src' without border, 'res' may be empty
	static int borderReflect(Mat& src, Mat& res);

	// Without test of border
	static Mat cutBorder(Mat& img);
	static int cutBorder(Mat& img, Mat& res);

	// 'src' without border, 'dst' may be empty
	static int testOcvFilter(Mat& src, Mat& dst);

private:
	ClosingProcess() = default;
	~ClosingProcess() = default;

	// 'src' have border, 'res' may be empty
	static int prDilate(Mat& src, Mat& res);
	static int prErode(Mat& src, Mat& res);

	static int copyRow(int filledRow, int cleanRow, Mat & img);
	static int expandRow(int iImgRowNum, Mat& iImg, Mat& oImg);
	static void compareImg(Mat& firstImg, Mat& secondImg);
};


void pr(Mat& img);
