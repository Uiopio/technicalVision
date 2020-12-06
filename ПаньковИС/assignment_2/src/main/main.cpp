#include "morphological_filter.h"

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "stdint.h"

using namespace std;
using namespace cv;

int main()
{
    auto filter = MorphologicalFilter();
    auto image = imread("src/images/check.bmp");
    auto structuringElement = imread("src/structuring_elements/my_filter.bmp");
    imshow("Image", image);

    filter.setImage(image);
    filter.setStructuringElement(structuringElement);
    filter.open();

    auto myResult = filter.getImage();
    imshow("My result", myResult);

    auto kernel = Mat();
    auto src = Mat();
    auto openCVResult = Mat();

    cvtColor(image, src, COLOR_BGR2GRAY);
    cvtColor(structuringElement, kernel, COLOR_BGR2GRAY);
    threshold(src, src, 0x7F, 0xFF, THRESH_BINARY);
    threshold(kernel, kernel, 0x7F, 1, THRESH_BINARY_INV);
    morphologyEx(src, openCVResult, MORPH_OPEN, kernel, Point(-1, -1), 1, BORDER_REFLECT);
    imshow("OpenCV Result", openCVResult);

    auto diff = Mat();
    absdiff(myResult, openCVResult, diff);
    absdiff(diff, Mat(diff.rows, diff.cols, diff.type(), Scalar(0xFF)), diff);
    imshow("Difference", diff);

    while (waitKey() != 27);

    return 0;
}