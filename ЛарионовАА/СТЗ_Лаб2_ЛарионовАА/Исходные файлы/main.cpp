
#include <iostream>
#include <chrono>
#include <ctime>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "my_filter\my_filter.h"

using namespace cv;
using namespace std;

float deviation(Mat& diff);

int main()
{
    Mat src, graySrc, dstOpCV, dstMyF, diff;

    src = imread("src.bmp", IMREAD_COLOR);
    cvtColor(src, graySrc, COLOR_BGRA2GRAY);
    imshow("Gray image", graySrc);
    imwrite("gray.bmp", graySrc);

    //auto start = chrono::steady_clock::now();
    //float start_time = (float)clock();

    adaptiveThreshold(graySrc, dstOpCV, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 7);

    //float end_time = (float)clock();
    //float search_time = end_time - start_time;
    //cout << "Time: "<< search_time / CLOCKS_PER_SEC << " seconds" << endl;
    //auto end = chrono::steady_clock::now();
    //auto elapsed = chrono::duration_cast<std::chrono::microseconds>(end - start);
    //cout << "Elapsed time: " << elapsed.count() << " microseconds" << endl;

    imshow("Processed image by OpenCV", dstOpCV);
    imwrite("dstOpCV.bmp", dstOpCV);

    MyFilter filter;
    filter.setImage(graySrc);
    filter.setParam(5, (float)1.1, 255, 7);
    filter.makeKernel();

    //auto start = chrono::steady_clock::now();

    filter.processImage();

    //auto end = chrono::steady_clock::now();
    //auto elapsed = chrono::duration_cast<std::chrono::microseconds>(end - start);
    //cout << "Elapsed time: " << elapsed.count() << " microseconds" << endl;

    dstMyF = filter.getImage();
    imshow("Processed image by MyFilter", dstMyF);
    imwrite("dstMyF.bmp", dstMyF);

    absdiff(dstOpCV, dstMyF, diff);
    imshow("Difference", diff);
    imwrite("difference.bmp", diff);

    float error = deviation(diff);
    cout << error;

    while (waitKey(33) != 27) {}

    return 0;
}

float deviation(Mat& diff)
{
    int sum = 0;
    int n = diff.rows * diff.cols;

    for (int i = 0; i < diff.rows; i++)
    {
        for (int j = 0; j < diff.cols; j++)
        {
            sum += (int)diff.at<uint8_t>(i, j);
        }
    }

    float middle = (float)sum / n;

    float s = 0;
    float numerator = 0;

    for (int i = 0; i < diff.rows; i++)
    {
        for (int j = 0; j < diff.cols; j++)
        {
            numerator += ((float)diff.at<uint8_t>(i, j) - middle) * ((float)diff.at<uint8_t>(i, j) - middle);
        }
    }

    s = sqrt((numerator / n) / (n - 1));

    return s;
}
