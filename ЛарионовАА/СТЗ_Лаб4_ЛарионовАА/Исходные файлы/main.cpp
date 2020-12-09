
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
    thirdPicture,
    fourthPicture,
    fifthPicture,
};

enum Objects
{
    redRobots = 1,
    greenRobots,
    blueRobots,
    lamps,
};

int getAverage(Mat& image);
Point getCenter(Moments& set);
void drawCrossRed(Mat& image, Point& dot);
void doFirstTask(int capture); 

int findLarge(vector<vector<Point>>& contours);
void doSecondTask(int capture);

Mat findObject(Mat& imageHSV, int object);
void drawObject(Mat& image, Mat& objectPosition, int object);
float calculateDist(Point& dot, Point& center);
void drawCrossBlack(Mat& image, Point& center);
void findNear(Mat& image, Mat& lampPosition, vector<Mat>& robotPosition);
void doThirdTask(int capture);

void doFourthTask();


int main()
{
    doFirstTask(firstPicture);
    doFirstTask(secondPicture);
    doFirstTask(thirdPicture);

    destroyAllWindows();
    
    doSecondTask(firstPicture);
    doSecondTask(secondPicture);
    doSecondTask(thirdPicture);
    doSecondTask(fourthPicture);
    doSecondTask(fifthPicture);

    destroyAllWindows();

    doThirdTask(firstPicture);
    doThirdTask(secondPicture);

    destroyAllWindows();

    doFourthTask();

    return 0;
}


void doFirstTask(int capture)
{
    Mat image;

    switch (capture)
    {
    case 1:
        image = imread("Аллахбабах/ig_0.jpg", IMREAD_COLOR);
        break;
    case 2:
        image = imread("Аллахбабах/ig_1.jpg", IMREAD_COLOR);
        break;
    case 3:
        image = imread("Аллахбабах/ig_2.jpg", IMREAD_COLOR);
        break;
    default:
        return;
    }

    namedWindow("Original image", WINDOW_AUTOSIZE);
    imshow("Original image", image);

    Mat imageGray;
    cvtColor(image, imageGray, COLOR_BGRA2GRAY);

    Mat imageThresh;
    int initialThresh = getAverage(imageGray);
    bool exit = false;

    while (exit == false)
    {
        threshold(imageGray, imageThresh, initialThresh, 255, THRESH_BINARY);

        Mat kernel;
        kernel = getStructuringElement(MORPH_RECT, Size(9, 9));

        Mat morphology;
        erode(imageThresh, morphology, kernel);
        dilate(morphology, morphology, kernel);

        Mat canny;
        Canny(morphology, canny, 10, 255, 3, true);

        vector<vector<Point>> contours;
        findContours(canny, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

        Mat imageCopy = image.clone();
        drawContours(imageCopy, contours, -1, Scalar(220, 220, 40), 2);

        if (contours.size() == 1)
        {
            Moments set = moments(contours[0]);
            Point center = getCenter(set);
            drawCrossRed(imageCopy, center);

            namedWindow("Result image", WINDOW_AUTOSIZE);
            imshow("Result image", imageCopy);
            exit = true;
        }
        else
        {
            initialThresh++;

            if (initialThresh == 255)
            {
                break;
            }
        }
    }

    while (waitKey(33) != 27) {}

    return;
}

void doSecondTask(int capture)
{
    Mat image;

    switch (capture)
    {
    case 1:
        image = imread("Тепловизор/1.jpg", IMREAD_COLOR);
        break;
    case 2:
        image = imread("Тепловизор/2.jpg", IMREAD_COLOR);
        break;
    case 3:
        image = imread("Тепловизор/3.jpg", IMREAD_COLOR);
        break;
    case 4:
        image = imread("Тепловизор/4.png", IMREAD_COLOR);
        break;
    case 5:
        image = imread("Тепловизор/5.jpg", IMREAD_COLOR);
        break;
    default:
        return;
    }

    namedWindow("Original image", WINDOW_AUTOSIZE);
    imshow("Original image", image);

    Mat imageHSV;
    cvtColor(image, imageHSV, COLOR_BGR2HSV);

    Mat maskOne, maskTwo;
    inRange(imageHSV, Scalar(0, 70, 50), Scalar(10, 255, 255), maskOne);
    inRange(imageHSV, Scalar(170, 70, 50), Scalar(179, 255, 255), maskTwo);

    Mat imageThresh;
    imageThresh = maskOne | maskTwo;

    Mat kernel;
    kernel = getStructuringElement(MORPH_RECT, Size(15, 15));

    Mat morphology;
    dilate(imageThresh, morphology, kernel);

    Mat canny;
    Canny(morphology, canny, 10, 255, 3, true);

    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    int numberLarge = findLarge(contours);

    Mat imageCopy = image.clone();
    polylines(imageCopy, contours[numberLarge], false, Scalar(255, 255, 255), 2);

    Moments set = moments(contours[numberLarge]);
    Point center = getCenter(set);
    drawCrossBlack(imageCopy, center);

    putText(imageCopy, "Bang-Bang", Point2i(50, 50), FONT_HERSHEY_COMPLEX, 1, Scalar(20, 20, 240), 1);

    namedWindow("Result image", WINDOW_AUTOSIZE);
    imshow("Result image", imageCopy);

    while (waitKey(33) != 27) {}

    return;
}

void doThirdTask(int capture)
{
    Mat image;

    switch (capture)
    {
    case 1:
        image = imread("Роботы/roi_robotov (2).jpg", IMREAD_COLOR);
        break;
    case 2:
        image = imread("Роботы/roi_robotov_1 (2).jpg", IMREAD_COLOR);
        break;
    default:
        return;
    }

    namedWindow("Original image", WINDOW_NORMAL);
    resizeWindow("Original image", 640, 360);
    imshow("Original image", image);

    Mat imageHSV;
    cvtColor(image, imageHSV, COLOR_BGR2HSV);

    Mat redRobot, greenRobot, blueRobot, lamp;
    redRobot = findObject(imageHSV, redRobots);
    greenRobot = findObject(imageHSV, greenRobots);
    blueRobot = findObject(imageHSV, blueRobots);
    lamp = findObject(imageHSV, lamps);

    Mat imageCopy = image.clone();
    drawObject(imageCopy, redRobot, redRobots);
    drawObject(imageCopy, greenRobot, greenRobots);
    drawObject(imageCopy, blueRobot, blueRobots);
    drawObject(imageCopy, lamp, lamps);

    vector<Mat> objects;
    objects.push_back(redRobot);
    objects.push_back(greenRobot);
    objects.push_back(blueRobot);
    findNear(imageCopy, lamp, objects);

    namedWindow("Result image", WINDOW_NORMAL);
    resizeWindow("Result image", 640, 360);
    imshow("Result image", imageCopy);

    while (waitKey(33) != 27) {}

    return;
}

void doFourthTask()
{
    Mat image, imageTmp;
    image = imread("Гаечные ключи/gk.jpg", IMREAD_COLOR);
    imageTmp = imread("Гаечные ключи/gk_tmplt.jpg", IMREAD_COLOR);

    namedWindow("Original image", WINDOW_NORMAL);
    resizeWindow("Original image", 796, 438);
    imshow("Original image", image);

    namedWindow("Template image", WINDOW_AUTOSIZE);
    imshow("Template image", imageTmp);

    Mat imageGauss;
    GaussianBlur(image, imageGauss, Size (5, 5), 1);

    Mat imageGray;
    cvtColor(imageGauss, imageGray, COLOR_BGRA2GRAY);

    Mat tmpGray;
    cvtColor(imageTmp, tmpGray, COLOR_BGRA2GRAY);

    Mat imageThresh;
    threshold(imageGray, imageThresh, 200, 255, THRESH_BINARY_INV);

    Mat kernel;
    kernel = getStructuringElement(MORPH_RECT, Size(7, 7));

    Mat morphologyImage;
    erode(imageThresh, morphologyImage, kernel);
    dilate(morphologyImage, morphologyImage, kernel);

    Mat cannyImage;
    Canny(morphologyImage, cannyImage, 10, 255, 3, true);

    Mat cannyTmp;
    Canny(tmpGray, cannyTmp, 10, 255, 3, true);

    vector<vector<Point>> contoursImage;
    findContours(cannyImage, contoursImage, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    vector<vector<Point>> contoursTmp;
    findContours(cannyTmp, contoursTmp, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    Mat imageCopy = image.clone();

    for (int i = 0; i < contoursImage.size(); i++)
    {
        double valueSimilarity = matchShapes(contoursImage[i], contoursTmp[0], CONTOURS_MATCH_I2, 0);

        Moments set = moments(contoursImage[i]);
        Point center = getCenter(set);

        if ((valueSimilarity > 0) && (valueSimilarity < 0.5))
        {
            drawCrossRed(imageCopy, center);
            putText(imageCopy, to_string(valueSimilarity), Point2i(center.x + 20, center.y + 20), FONT_HERSHEY_COMPLEX, 1, Scalar(20, 20, 240), 2);
        }
        else
        {
            drawCrossBlack(imageCopy, center);
            putText(imageCopy, to_string(valueSimilarity), Point2i(center.x + 20, center.y + 20), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0), 2);
        }
    }

    namedWindow("Result image", WINDOW_NORMAL);
    resizeWindow("Result image", 796, 438);
    imshow("Result image", imageCopy);

    while (waitKey(33) != 27) {}

    return;
}


int getAverage(Mat& image)
{
    int value = 0;
    int average = 0;

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            value += image.at<uint8_t>(i, j);
        }
    }

    average = (int)((float)value / ((float)image.rows * (float)image.cols));

    if (average > 255)
    {
        average = 255;
    }

    return average;
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

void drawCrossRed(Mat& image, Point& dot)
{
    line(image, Point((dot.x - 10), dot.y), Point((dot.x + 10), dot.y), Scalar(20, 20, 240), 3);
    line(image, Point(dot.x, (dot.y - 10)), Point(dot.x, (dot.y + 10)), Scalar(20, 20, 240), 3);

    return;
}

int findLarge(vector<vector<Point>>& contours)
{
    int number = -1;
    double maxPerimeter = 0;

    for (int i = 0; i < contours.size(); i++)
    {
        Moments set = moments(contours[i]);

        if (maxPerimeter < set.m00)
        {
            maxPerimeter = set.m00;
            number = i;
        }
    }

    if (number == -1)
    {
        cout << endl << "Large contour is not found, return (-1)" << endl;
        waitKey(0);
    }

    return number;
}

Mat findObject(Mat& imageHSV, int object)
{
    Mat objectBorder;

    if (object == redRobots)
    {
        Mat maskOne, maskTwo;
        inRange(imageHSV, Scalar(0, 50, 50), Scalar(10, 255, 255), maskOne);
        inRange(imageHSV, Scalar(170, 50, 50), Scalar(179, 255, 255), maskTwo);

        Mat imageThresh;
        imageThresh = maskOne | maskTwo;

        Mat kernel;
        kernel = getStructuringElement(MORPH_RECT, Size(9, 9));

        Mat morphology;
        erode(imageThresh, morphology, kernel);
        dilate(morphology, morphology, kernel);

        Mat canny;
        Canny(morphology, canny, 10, 255, 3, true);

        objectBorder = canny;
    }

    if (object == greenRobots || object == blueRobots)
    {
        Mat imageThresh;

        if (object == greenRobots)
        {
            inRange(imageHSV, Scalar(70, 50, 50), Scalar(80, 255, 255), imageThresh);
        }

        if (object == blueRobots)
        {
            inRange(imageHSV, Scalar(90, 50, 50), Scalar(130, 255, 255), imageThresh);
        }

        Mat kernel;
        kernel = getStructuringElement(MORPH_RECT, Size(9, 9));

        Mat morphology;
        erode(imageThresh, morphology, kernel);
        dilate(morphology, morphology, kernel);

        Mat canny;
        Canny(morphology, canny, 10, 255, 3, true);

        objectBorder = canny;
    }

    if (object == lamps)
    {
        Mat imageThresh;
        inRange(imageHSV, Scalar(10, 0, 255), Scalar(30, 10, 255), imageThresh);

        Mat kernel;
        kernel = getStructuringElement(MORPH_RECT, Size(9, 9));

        Mat morphology;
        dilate(imageThresh, morphology, kernel);

        Mat canny;
        Canny(morphology, canny, 10, 255, 3, true);

        objectBorder = canny;
    }

    return objectBorder;
}

void drawObject(Mat& image, Mat& objectPosition, int object)
{
    vector<vector<Point>> contours;
    findContours(objectPosition, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    if (object == redRobots)
    {
        for (int i = 0; i < contours.size(); i++)
        {
            Moments set = moments(contours[i]);
            Point center = getCenter(set);
            ellipse(image, center, Size(20, 10), 0, 0, 360, Scalar(10, 10, 250), -1);
        }
    }

    if (object == greenRobots)
    {
        for (int i = 0; i < contours.size(); i++)
        {
            Moments set = moments(contours[i]);
            Point center = getCenter(set);
            ellipse(image, center, Size(20, 10), 0, 0, 360, Scalar(10, 250, 10), -1);
        }
    }

    if (object == blueRobots)
    {
        for (int i = 0; i < contours.size(); i++)
        {
            Moments set = moments(contours[i]);
            Point center = getCenter(set);
            ellipse(image, center, Size(20, 10), 0, 0, 360, Scalar(250, 10, 10), -1);
        }
    }

    if (object == lamps)
    {
        for (int i = 0; i < contours.size(); i++)
        {
            Moments set = moments(contours[i]);
            Point center = getCenter(set);
            ellipse(image, center, Size(20, 10), 0, 0, 360, Scalar(20, 220, 220), -1);

            // Mark
            ellipse(image, Point(center.x, (image.rows / 2)), Size(5, 5), 0, 0, 360, Scalar(0, 0, 0), -1);
        }
    }

    return;
}

float calculateDist(Point& dot, Point& center)
{
    float distance = 0;

    distance = sqrt(pow((float)(dot.x - center.x), 2) + pow((float)(dot.y - center.y), 2));

    return distance;
}

void drawCrossBlack(Mat& image, Point& dot)
{
    line(image, Point((dot.x - 10), dot.y), Point((dot.x + 10), dot.y), Scalar(0, 0, 0), 3);
    line(image, Point(dot.x, (dot.y - 10)), Point(dot.x, (dot.y + 10)), Scalar(0, 0, 0), 3);

    return;
}

void findNear(Mat& image, Mat& lampPosition, vector<Mat>& robotPosition)
{
    vector<vector<Point>> contoursLamp;
    findContours(lampPosition, contoursLamp, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    Moments setLamp = moments(contoursLamp[0]);
    Point centerLamp = getCenter(setLamp);
    Point dotMark(centerLamp.x, (image.rows / 2));

    for (int i = 0; i < robotPosition.size(); i++)
    {
        vector<vector<Point>> contours;
        findContours(robotPosition[i], contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

        Point centerWantedRobot;
        float distance = (float)10000;

        for (int j = 0; j < contours.size(); j++)
        {
            Moments setRobot = moments(contours[j]);
            Point centerRobot = getCenter(setRobot);

            float newDistance = calculateDist(dotMark, centerRobot);

            if (newDistance < distance)
            {
                distance = newDistance;
                centerWantedRobot = centerRobot;
            }
        }

        drawCrossBlack(image, centerWantedRobot);
    }

    return;
}
