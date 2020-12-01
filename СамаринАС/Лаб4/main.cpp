#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;


void part1(Mat inputImage)
{
    Mat temp;
    threshold(inputImage, temp, 220, 255, THRESH_BINARY);
    erode(temp, temp, getStructuringElement(MORPH_RECT, Size(3, 3)));
    dilate(temp, temp, getStructuringElement(MORPH_RECT, Size(3, 3)));

    Mat edges;
    Canny(temp, edges, 0, 0, 3, false);

    Mat image = edges.clone();
    vector<vector<Point>> cnts;
    findContours(image, cnts, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    Moments mnts = moments(cnts[0]);
    double m00 = mnts.m00;
    double m10 = mnts.m10;
    double m01 = mnts.m01;

    Point center(0, 0);
    center.x = m10 / m00;
    center.y = m01 / m00;

    Mat out;
    cvtColor(inputImage, out, COLOR_GRAY2BGR);

    rectangle(out, Rect(center.x - 2, center.y - 20, 4, 40), Scalar(0, 0, 255), FILLED, 8, 0);
    rectangle(out, Rect(center.x - 20, center.y - 2, 40, 4), Scalar(0, 0, 255), FILLED, 8, 0);

    imshow("output", out);
    waitKey();
    return;
}


void part2(Mat imputImage)
{
    Mat temp;
    inRange(imputImage, Scalar(34, 101, 0), Scalar(164, 255, 255), temp);

    dilate(temp, temp, getStructuringElement(MORPH_RECT, Size(3, 3)));
    erode(temp, temp, getStructuringElement(MORPH_RECT, Size(3, 3)));

    Mat edges;
    Canny(temp, edges, 0, 0, 3, false);

    Mat image = edges.clone();
    vector<vector<Point>> cnts;
    findContours(image, cnts, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    cout << cnts.size() << endl;

    //Рассчет моментов для найденных контуров
    vector<Moments> mnts;
    int i_m = cnts.size();
    for (int i = 0; i < i_m; i++)
    {
        mnts.push_back(moments(cnts[i]));
    }

    int i_max = 0; //Индекс наибольщего контура
    double tempArea = mnts[0].m00;
    for (int i = 0; i < cnts.size(); i++)
    {
        if (mnts[i].m00 >= tempArea)
        {
            i_max = i; 
            tempArea = mnts[i].m00;
        }
    }


    Moments mntss = moments(cnts[i_max]);
    double m00 = mntss.m00;
    double m10 = mntss.m10;
    double m01 = mntss.m01;

    Point center(0, 0);
    center.x = m10 / m00;
    center.y = m01 / m00;

    Mat out;
    cvtColor(imputImage, out, COLOR_HSV2BGR);

    rectangle(out, Rect(center.x - 2, center.y - 20, 4, 40), Scalar(0, 0, 0), FILLED, 8, 0);
    rectangle(out, Rect(center.x - 20, center.y - 2, 40, 4), Scalar(0, 0, 0), FILLED, 8, 0);

    imshow("output", out);
    waitKey();
    return;
}


Mat creationMask(int flagColor, Mat inputImage)
{
    Scalar low, high;
    int dilateSize = 1, erodeSize = 1;
    string color = "mask";

    if (flagColor == 0) //Лампа
    {
        Scalar lampLow(107, 0, 255);
        low = lampLow;
        Scalar lampHigh(180, 255, 255);
        high = lampHigh;
        dilateSize = 10;
        erodeSize = 1;
        color = "Lamp";
    }
    if (flagColor == 1) //Голубой
    {
        Scalar blueLow(82, 50, 0);
        Scalar blueHigh(116, 255, 255);
        low = blueLow;
        high = blueHigh;
        dilateSize = 13;
        erodeSize = 18;
        color = "Blue";
    }
    if (flagColor == 2) //Зеленый
    {
        Scalar greenLow(43, 52, 113);
        Scalar greenHigh(84, 255, 255);
        low = greenLow;
        high = greenHigh;
        dilateSize = 2;
        erodeSize = 7;
        color = "Green";
    }
    if (flagColor == 3) //Красный
    {
        Scalar redLow(10, 0, 0);
        Scalar redHigh(116, 255, 255);
        low = redLow;
        high = redHigh;
        dilateSize = 13;
        erodeSize = 16;
        color = "Red";
    }
    
    Mat tempMask;
    inRange(inputImage, low, high, tempMask);
    dilate(tempMask, tempMask, getStructuringElement(MORPH_RECT, Size(dilateSize, dilateSize)));
    erode(tempMask, tempMask, getStructuringElement(MORPH_RECT, Size(erodeSize, erodeSize)));
    imshow(color, tempMask);
    waitKey();
    //mask = tempMask.clone();
    return tempMask;

}

int searchMinDistance(vector<Point> centerRobot, Point centerLamp)
{
    vector<double> distance;
    for (int i = 0; i < centerRobot.size(); i++)
    {
        double a1 = abs(centerRobot[i].x - centerLamp.x);
        double a2 = abs(centerRobot[i].y - centerLamp.y);
        double temp = sqrt(a1 * a1 + a2 * a2);
        distance.push_back(temp);
    }

    double minDistance = distance[0];
    int indMin = 0;
    for (int i = 0; i < distance.size(); i++)
    {
        if (distance[i] <= minDistance)
        {
            minDistance = distance[i];
            indMin = i;
        }
    }
    return indMin;
}


void part3(Mat inputImage, Mat imageOut)
{
    Mat lampMask = creationMask(0, inputImage);

    Mat lampEdges;
    vector<vector<Point>> cntsLamp;
    Canny(lampMask, lampEdges, 0, 0, 3, false);
    Mat tempLamp = lampEdges.clone();
    findContours(tempLamp, cntsLamp, RETR_EXTERNAL, CHAIN_APPROX_NONE);


    Moments mntsLamp = moments(cntsLamp[0]);
    double m00Lamp = mntsLamp.m00;
    double m10Lamp = mntsLamp.m10;
    double m01Lamp = mntsLamp.m01;

    Point centerLamp(0, 0);
    centerLamp.x = m10Lamp / m00Lamp;
    centerLamp.y = m01Lamp / m00Lamp;
    circle(imageOut, centerLamp, 8, Scalar(0, 0, 0), FILLED, 8, 0);

    cvtColor(inputImage, inputImage, COLOR_HSV2BGR);

    Scalar color(inputImage.at<Vec3b>(10,10)[0], inputImage.at<Vec3b>(10, 10)[1], inputImage.at<Vec3b>(10, 10)[2]);
    ellipse(inputImage, Point(centerLamp.x, centerLamp.y - 28), Size(40,22), 17, 0, 360, color, FILLED, 8, 0);
    imshow("inputImage", inputImage);
    waitKey();
    cvtColor(inputImage, inputImage, COLOR_BGR2HSV);


    Mat blueMask = creationMask(1, inputImage);
    Mat blueEdges;
    vector<vector<Point>> cntsBlue;
    

    Canny(blueMask, blueEdges, 0, 0, 3, false);
    Mat tempBlue = blueEdges.clone();
    findContours(tempBlue, cntsBlue, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    Mat redMask = creationMask(3, inputImage);
    Mat redEdges;
    vector<vector<Point>> cntsRed;
    

    Canny(redMask, redEdges, 0, 0, 3, false);
    Mat tempRed = redEdges.clone();
    findContours(tempRed, cntsRed, RETR_EXTERNAL, CHAIN_APPROX_NONE);


    Mat greenMask = creationMask(2, inputImage);
    Mat greenEdges;
    vector<vector<Point>> cntsGreen;
    

    Canny(greenMask, greenEdges, 0, 0, 3, false);
    Mat tempGreen = greenEdges.clone();
    findContours(tempGreen, cntsGreen, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    
    vector<Moments> mntsRed;
    for (int i = 0; i < cntsRed.size(); i++)
    {
        mntsRed.push_back(moments(cntsRed[i]));
    }
    vector<Point> centerRed;
    for (int i = 0; i < cntsRed.size(); i++)
    {
        double m00 = mntsRed[i].m00;
        double m10 = mntsRed[i].m10;
        double m01 = mntsRed[i].m01;

        centerRed.push_back(Point((m10 / m00), (m01 / m00)) );
    }

    vector<Moments> mntsBlue;
    for (int i = 0; i < cntsBlue.size(); i++)
    {
        mntsBlue.push_back(moments(cntsBlue[i]));
    }
    vector<Point> centerBlue;
    for (int i = 0; i < cntsBlue.size(); i++)
    {
        double m00 = mntsBlue[i].m00;
        double m10 = mntsBlue[i].m10;
        double m01 = mntsBlue[i].m01;

        centerBlue.push_back(Point((m10 / m00), (m01 / m00)));
    }

    vector<Moments> mntsGreen;
    for (int i = 0; i < cntsGreen.size(); i++)
    {
        mntsGreen.push_back(moments(cntsGreen[i]));
    }
    vector<Point> centerGreen;
    for (int i = 0; i < cntsGreen.size(); i++)
    {
        double m00 = mntsGreen[i].m00;
        double m10 = mntsGreen[i].m10;
        double m01 = mntsGreen[i].m01;

        centerGreen.push_back(Point((m10 / m00), (m01 / m00)));
    }
    

    int indMinBlue = searchMinDistance(centerBlue, centerLamp);
    int indMinGreen = searchMinDistance(centerGreen, centerLamp);
    int indMinRed = searchMinDistance(centerRed, centerLamp);
   

    

    polylines(imageOut, cntsBlue, true, Scalar(255,0,0), 5, 8, 0);
    polylines(imageOut, cntsRed, true, Scalar(0, 0, 255), 5, 8, 0);
    polylines(imageOut, cntsGreen, true, Scalar(0, 255, 0), 5, 8, 0);

    line(imageOut, centerRed[indMinRed], centerLamp, Scalar(0, 0, 255), 4, 8, 0);
    line(imageOut, centerBlue[indMinBlue], centerLamp, Scalar(255, 0, 0), 4, 8, 0);
    line(imageOut, centerGreen[indMinGreen], centerLamp, Scalar(0, 255, 0), 4, 8, 0);

    imshow("imageOut", imageOut);
    waitKey();
}

void part4(Mat inputImage1, Mat inputImage2)
{
    Mat mask1;
    threshold(inputImage1, mask1, 229, 255, THRESH_BINARY);
    dilate(mask1, mask1, getStructuringElement(MORPH_RECT, Size(2, 2)));
    erode(mask1, mask1, getStructuringElement(MORPH_RECT, Size(3, 3)));

    Mat edges1;
    Canny(mask1, edges1, 0, 0, 3, false);
    Mat temp1 = edges1.clone();
    vector<vector<Point>> cnts1;
    findContours(temp1, cnts1, RETR_EXTERNAL, CHAIN_APPROX_NONE);


    Mat mask2;
    threshold(inputImage2, mask2, 100, 255, THRESH_BINARY);
    dilate(mask2, mask2, getStructuringElement(MORPH_RECT, Size(2, 2)));
    erode(mask2, mask2, getStructuringElement(MORPH_RECT, Size(3, 3)));

    Mat edges2;
    Canny(mask2, edges2, 0, 0, 3, false);
    Mat temp2 = edges2.clone();
    vector<vector<Point>> cnts2;
    findContours(temp2, cnts2, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    vector<double> likeness;
    cvtColor(inputImage1, inputImage1, COLOR_GRAY2BGR);

    for (int i = 0; i < cnts1.size(); i++)
    {
        likeness.push_back(matchShapes(cnts2[0], cnts1[i], ShapeMatchModes::CONTOURS_MATCH_I2, 0));
        cout << likeness[i] << endl;
        if (likeness[i] <= 1)
        {
            imshow("inputImage1", inputImage1);
            waitKey();
            polylines(inputImage1, cnts1[i], true, Scalar(0, 255, 0), 4, 8);
            imshow("inputImage1", inputImage1);
            waitKey();
        }
        if (likeness[i] > 1)
        {
            imshow("inputImage1", inputImage1);
            waitKey();
            polylines(inputImage1, cnts1[i], true, Scalar(0, 0, 255), 4, 8);
            imshow("inputImage1", inputImage1);
            waitKey();
        }
    }

    imshow("result", inputImage1);
    waitKey();
    return;
}

void partOne()
{
    string name;
    for (int i = 1; i < 4; i++)
    {
        name = "part1_";
        name = name + to_string(i);
        name += ".jpg";
        Mat part1_1 = imread(name, IMREAD_GRAYSCALE);
        imshow("name", part1_1);
        part1(part1_1);
    }
}

void partTwo()
{
    string name;
    for (int i = 1; i < 6; i++)
    {
        name = "part2_";
        name = name + to_string(i);
        name += ".jpg";
        Mat part2_1 = imread(name);
        imshow("part2_1", part2_1);
        Mat part2_1HSV;
        cvtColor(part2_1, part2_1HSV, COLOR_BGR2HSV);
        part2(part2_1HSV);
    }
}

void partThree()
{
    string name;
    for (int i = 1; i < 3; i++)
    {
        name = "part3_";
        name = name + to_string(i);
        name += ".jpg";

        Mat part3_1 = imread(name);
        imshow(name, part3_1);
        Mat part3_1HSV;
        cvtColor(part3_1, part3_1HSV, COLOR_BGR2HSV);
        part3(part3_1HSV, part3_1);
    }
}


void partFour()
{
    Mat part4_1 = imread("part4_1.jpg", IMREAD_GRAYSCALE);
    Mat part4_2 = imread("part4_2.jpg", IMREAD_GRAYSCALE);
    imshow("part4_1", part4_1);
    imshow("part4_2", part4_2);
    waitKey();

    part4(part4_1, part4_2);
}


int main()
{

    int button = 0;
    bool stop = false;
    while (stop == false)
    {
        cin >> button;

        switch (button)
        {
        case 1:
            partOne();
            break;

        case 2:
            partTwo();
            break;
        case 3:
            partThree();
            break;

        case 4:
            partFour();
            break;
        case 5:
            stop = true;
            break;
        }
        
    }
    
	return 0;
}

