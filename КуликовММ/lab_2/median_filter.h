#pragma once

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

class MedianFilter
{
public:
	MedianFilter();
	MedianFilter(Mat src);

	~MedianFilter();

	//���������� ������ (���� 3�3, ����� � ����� ������ ����)
	void filteringWithMyAnchor();

	//���������� ������ � ����� 3�3 � ������ �� ������ (��� ��������� �����������)
	void filteringWithMiddleAnchor();

	//���������� ������ (���� 3�3, ����� � ������ ������� ����)
	void filteringWithOtherAnchor();

	//����������� ������ OpenCV
	void filteringWithOpenCV();

	//������ ������
	void gaussFilter();

	//������� ����� ����������� � ���������� �������� (� ��������)
	void showDifference(Mat src1, Mat src2);

	//�������
	Mat getImageHW();
	Mat getImageHW2();
	Mat getImageCV();
	Mat getImageGauss();

private:
	Mat m_srcImage;
	Mat m_dstImageHW;
	Mat m_dstImageHW2;
	Mat m_dstImageCV;
	Mat m_dstImageGauss;
	int m_window[9];
};