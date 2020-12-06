#pragma once
#include <iostream>
#include "opencv2/core.hpp";
#include "opencv2/highgui.hpp";

using namespace cv;
using namespace std;

class AdaptiveThreshold
{
public:
    AdaptiveThreshold() = default;
    AdaptiveThreshold(int m_maxValue, int m_constant, int m_positionOfAnchor);
    ~AdaptiveThreshold() = default;

    void adaptiveThreshold(Mat& inputImage, Mat& outputImage);
    double meanValue(Mat& image, int row, int col);

private:
    int m_maxValue;
    int m_constant;
    int m_positionOfAnchor;
};