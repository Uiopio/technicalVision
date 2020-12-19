
#include <iostream>
#include <vector>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"

using namespace cv;
using namespace std;

enum Pictures
{
    firstPicture = 1,
    secondPicture,
};

enum Objects
{
    redRobots = 1,
    greenRobots,
    blueRobots,
    lamps,
};

Point getCenter(Moments& set);
Point getNewCenter(vector<Point>& buffer);
bool checkRepeat(vector<int>& repeat, int index);
float calculateDist(Point& dot, Point& dotNext, float radius);
void findWhole(vector<Point>& src, float midRadius, vector<Point>& dst);
void testFunction(int, void*);

Mat image;
Mat imageHSV;
Mat redCircle;

int bottomOne = 0;
int topOne = 0;
int bottomTwo = 0;
int topTwo = 0;
int kernelSize = 0;
int SatBottom = 0;
int SatTop = 0;
int ValBottom = 0;
int ValTop = 0;

int main()
{
    // firstPicture, secondPicture
    switch (firstPicture)
    {
    case 1:
        image = imread("Роботы/roi_robotov (2).jpg", IMREAD_COLOR);
        break;
    case 2:
        image = imread("Роботы/roi_robotov_1 (2).jpg", IMREAD_COLOR);
        break;
    case 3:
        image = imread("testPicture.bmp", IMREAD_COLOR);
        break;
    default:
        return 0;
    }
    // redRobots, greenRobots, blueRobots
    switch (blueRobots)
    {
    case 1:
        bottomOne = 0; topOne = 5; bottomTwo = 175; topTwo = 179; kernelSize = 9;
        SatBottom = 100; SatTop = 255; ValBottom = 100; ValTop = 255;
        break;
    case 2:
        bottomOne = 70; topOne = 75; bottomTwo = 70; topTwo = 75; kernelSize = 9;
        SatBottom = 80; SatTop = 255; ValBottom = 150; ValTop = 255;
        break;
    case 3:
        bottomOne = 90; topOne = 110; bottomTwo = 90; topTwo = 110; kernelSize = 9;
        SatBottom = 50; SatTop = 255; ValBottom = 50; ValTop = 255;
        break;
    default:
        return 0;
    }

    Mat filter;
    GaussianBlur(image, filter, Size(3, 3), 1);

    cvtColor(filter, imageHSV, COLOR_BGR2HSV);

    namedWindow("Result image", WINDOW_NORMAL);
    resizeWindow("Result image", Size(800, 400));

    //createTrackbar("BottomOne", "Result image", &bottomOne, 179, testFunction);
    //createTrackbar("TopOne", "Result image", &topOne, 179, testFunction);
    //createTrackbar("BottomTwo", "Result image", &bottomTwo, 179, testFunction);
    //createTrackbar("TopTwo", "Result image", &topTwo, 179, testFunction);
    createTrackbar("Kernel", "Result image", &kernelSize, 15, testFunction);
    //createTrackbar("SatBottom", "Result image", &SatBottom, 255, testFunction);
    //createTrackbar("SatTop", "Result image", &SatTop, 255, testFunction);
    //createTrackbar("ValBottom", "Result image", &ValBottom, 255, testFunction);
    //createTrackbar("ValTop", "Result image", &ValTop, 255, testFunction);

    while (waitKey(33) != 27) {}

    return 0;
}

Point getCenter(Moments& set)
{
    double m00 = set.m00;
    double m10 = set.m10;
    double m01 = set.m01;

    int x = (int)(m10 / m00);
    int y = (int)(m01 / m00);

    Point center(x, y);

    return center;
}

Point getNewCenter(vector<Point>& buffer)
{
    Point dot;
    
    int x = 0;
    int y = 0;

    for (int i = 0; i < buffer.size(); i++)
    {
        x += buffer[i].x;
        y += buffer[i].y;
    }

    dot.x = (x / (int)buffer.size());
    dot.y = (y / (int)buffer.size());

    return dot;
}

bool checkRepeat(vector<int>& repeat, int index)
{
    bool result = false;

    for (int i = 0; i < repeat.size(); i++)
    {
        if (index == repeat[i])
        {
            result = true;
        }
    }
    
    return result;
}

float calculateDist(Point& dot, Point& dotNext, float radius)
{
    float distance = 0;

    if (abs((dot.y - dotNext.y)) < radius)
    {
        distance = sqrt(pow((float)(dot.x - dotNext.x), 2));
    }
    else
    {
        distance = 1000;
    }

    return distance;
}

void findWhole(vector<Point>& src, float radius, vector<Point>& dst)
{
    vector<int> repeat;
    vector<Point> buffer;

    for (int i = 0; i < src.size(); i++)
    {
        buffer.push_back(src[i]);

        for (int j = 0; j < src.size(); j++)
        {
            if (i != j)
            {
                float distance = calculateDist(src[i], src[j], radius);

                if (distance < (2.1 * radius))
                {
                    buffer.push_back(src[j]);
                    repeat.push_back(j);
                }
            }
        }

        if (checkRepeat(repeat, i) == false)
        {
            if (buffer.size() == 1)
            {
                dst.push_back(src[i]);
            }
            else
            {
                Point newCenter = getNewCenter(buffer);
                dst.push_back(newCenter);
            }
        }

        buffer.clear();
    }

    return;
}

void testFunction(int, void*)
{
    Mat maskOne, maskTwo;
    inRange(imageHSV, Scalar(bottomOne, SatBottom, ValBottom), Scalar(topOne, SatTop, ValTop), maskOne);
    inRange(imageHSV, Scalar(bottomTwo, SatBottom, ValBottom), Scalar(topTwo, SatTop, ValTop), maskTwo);

    Mat imageThresh;
    imageThresh = maskOne | maskTwo;

    if ((kernelSize % 2) != 1)
    {
        kernelSize += 1;
    }

    Mat kernel;
    kernel = getStructuringElement(MORPH_RECT, Size(kernelSize, kernelSize));

    Mat morphology;
    dilate(imageThresh, morphology, kernel);
    erode(morphology, morphology, kernel);
    
    Mat canny;
    Canny(morphology, canny, 10, 255, 3, true);

    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    Mat imageCopy = image.clone();

    float middleRadius = 0;
    float maxRadius = 0;
    vector<Point> centerContour;

    if (contours.empty() != true)
    {
        for (int i = 0; i < contours.size(); i++)
        {
            cout << "Contour " << (i + 1) << endl;
            Moments set = moments(contours[i]);

            Point center = getCenter(set);
            centerContour.push_back(center);
            cout << "X = " << center.x << " " << "Y = " << center.y << endl;

            cout << "Square (m00) = " << set.m00 << endl;
            
            float radius = sqrt((float)(set.m00 / CV_PI));
            cout << "Radius = " << radius << endl;
            middleRadius += radius;

            if (maxRadius < radius)
            {
                maxRadius = radius;
            }

            cout << endl;
        }

        middleRadius /= contours.size();
        cout << "Middle radius = " << middleRadius << endl << endl;
    }

    if (centerContour.empty() != true)
    {
        for (int i = 0; i < centerContour.size(); i++)
        {
            ellipse(imageCopy, Point(centerContour[i].x, centerContour[i].y), Size(int(middleRadius), int(middleRadius)), 0, 0, 360, Scalar(0, 0, 0), 3);
            putText(imageCopy, to_string((i + 1)), Point((centerContour[i].x + 20), (centerContour[i].y + 20)), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 170, 255), 2);
            line(imageCopy, Point(centerContour[i].x, centerContour[i].y), Point((centerContour[i].x + (int)middleRadius), centerContour[i].y), Scalar(0, 0, 0), 2);
        }
    }

    vector<Point> centerRobot;
    findWhole(centerContour, maxRadius, centerRobot);
    imageCopy = image.clone();

    if (centerRobot.empty() != true)
    {
        for (int i = 0; i < centerRobot.size(); i++)
        {
            ellipse(imageCopy, Point(centerRobot[i].x, centerRobot[i].y), Size(int(middleRadius), int(middleRadius)), 0, 0, 360, Scalar(0, 0, 0), 3);
            putText(imageCopy, to_string((i + 1)), Point((centerRobot[i].x + 20), (centerRobot[i].y + 20)), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 170, 255), 2);
            line(imageCopy, Point(centerRobot[i].x, centerRobot[i].y), Point((centerRobot[i].x + (int)middleRadius), centerRobot[i].y), Scalar(0, 0, 0), 2);
        }
    }

    imshow("Result image", imageCopy);
    
    return;
}
