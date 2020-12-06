#pragma once
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp";

using namespace cv;
using namespace std;

class Fourier
{
public:
    Fourier() = default;
    Fourier(Mat m_inputImage);
    ~Fourier() = default;

    void forwardTransform(Mat& inputImage, Mat& outputImage);
    void inverseTransform(Mat& inputImage, Mat& outputImage);
    void inverseFromSpectrumAndPhase(Mat& resultOfMultiply, Mat& outputImage);
    Mat optimalSize(Mat& image, int channels);
    void swapSpektr(Mat& magI);
    void spectrum(Mat& imageAfterDFT, Mat& result);
    void filterGaussian(Mat& inputImage, Mat& outputImage, float sigma);
    void laplace(Mat& inputImage, Mat& outputImage);
    void sobel(Mat& inputImage, Mat& outputImage, int flag); //horizontal  flag = 0, vertical flag = 1
    void boxFilter(Mat& inputImage, Mat& outputImage);
    void carNumber(Mat& number, Mat& symbol);
    void carNumber2(Mat& number, Mat& symbol);

private:
    Mat m_inputImage;
    Mat m_spectrum;
    Mat m_phase;
    Mat m_imageAfterDFT;

};