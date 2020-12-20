#include <iostream>
#include <opencv2/opencv.hpp>

#include "my_filter/closing_filter.h"

using namespace std;
using namespace cv;

int main()
{
    //string basePath = ""
    Mat src = imread("../res/1.bmp", CV_8UC1);

    namedWindow("img");
    imshow("img", src);

    Mat buff;
    src.copyTo(buff);
    uint8_t data[9] = { 255, 255, 255, 0, 0, 0, 0, 0, 0 };
    Mat kernel(3, 3, CV_8UC1, data);
    while (true)
    {
        Mat temp;
        buff.copyTo(temp);

        switch ((char)waitKey(0))
        {
        case 'd':
            ClosingProcess::dilate(buff, temp);
            break;
        case 'e':
            ClosingProcess::erode(buff, temp);
            break;
        case 'b':
            ClosingProcess::borderReflect(buff, temp);
            imshow("img", temp);
            temp = ClosingProcess::cutBorder(temp);
            waitKey(0);
            break;
        case 'f':
            ClosingProcess::useClosingFilter(buff, temp);
            break;
        case 't':
            cout << endl;
            ClosingProcess::testOcvFilter(src, temp);
            break;
        case 'u':
            morphologyEx(buff, temp, MORPH_CLOSE, kernel, Point(1, 1), 1, BORDER_REFLECT);
            break;
        case 'i':
            dilate(buff, temp, kernel, Point(1, 1), 1, BORDER_REFLECT);
            break;
        case 'o':
            erode(buff, temp, kernel, Point(1, 1), 1, BORDER_REFLECT);
            break;
        case 'r':
            src.copyTo(temp);
            break;
        case '1':
            src = imread("../res/1.bmp", CV_8UC1);
            src.copyTo(temp);
            break;
        case '2':
            src = imread("../res/2.bmp", CV_8UC1);
            src.copyTo(temp);
            break;
        case '3':
            src = imread("../res/3.bmp", CV_8UC1);
            src.copyTo(temp);
            break;
        case '4':
            src = imread("../res/4.bmp", CV_8UC1);
            src.copyTo(temp);
            break;
        case (char)27:  // Key Esc
            return 0;
        default:
            break;
        }

        imshow("img", temp);
        temp.copyTo(buff);
    }
    return 0;
};
