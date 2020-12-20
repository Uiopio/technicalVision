#pragma once
#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include <cmath>
#include <ctime>   

using namespace cv;
using namespace std;



class myGaussianBlur
{
public:
	myGaussianBlur();
	~myGaussianBlur(); 

//�������
	void setKernel(double sigma, int kernelRows, int kernelCols);
	void setSrcImg(Mat srcImage);

	void expandImage();
	void smooth();
	void calcDiff(Mat builtInImg);
	void squareDeviation();

//�������
	void getSrcImg();
	void getExpandedImg();
	void getResultImg();
	void getDiffImg();
	


private:
	Point2i m_anchor; //��������� �����
	
	double m_sigma;
	
	Mat m_kernel;
	Mat m_srcImage;
	Mat m_extImage;
	Mat m_resImage;
	Mat m_diffImg;

	Point2i m_Dot; //������� ����� ����� ������� � �������� ������������ �� �����������
	double m_kernelSum;
};

