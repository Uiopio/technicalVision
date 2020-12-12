#pragma once
#include "opencv2/core.hpp"
#include <opencv2/imgproc.hpp>

using namespace cv;

int compare(Mat img1, Mat img2);

class Otsu
{
public:
    Otsu();
    ~Otsu();
    int setImage(Mat img);
    Mat getImage();
    int processImage();
private:
    Mat m_image;
    int* m_hist;
    int m_minLuminity; 
    int m_maxLuminity;
    void makeHist();
    int otsuThreshold(Mat img);
    void binary(int treshold);
};