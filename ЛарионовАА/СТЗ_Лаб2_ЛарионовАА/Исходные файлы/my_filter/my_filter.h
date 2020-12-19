#pragma once

#include <iostream>
#include <vector>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"

using namespace cv;
using namespace std;

class MyFilter
{
public:
    MyFilter();
    ~MyFilter();
private:
    Mat m_src;
    Mat m_dst;

    int m_ksize;
    float m_sigma;
    int m_maxValue;
    int m_constant;
    
    vector<float> m_coef;
public:
    int setImage(Mat& image);
    int setParam(const int ksize, const float sigma, const int maxValue, const int constant);

    Mat getImage();

    int makeKernel();
    int processImage();
};
