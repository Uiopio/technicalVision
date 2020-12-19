#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

#define CUURENT_DIR std::string("/home/den/QtProjects/opencv-3027/lab4/qt_prj/")
#define DEBUG 0
void task1(size_t img_count = 3, size_t crosshairLenth = 15, Scalar color = Scalar(255, 0, 0));
void task2(size_t img_count = 5, size_t crosshairLenth = 15, Scalar color = Scalar(0, 0, 0));
void task3(size_t img_count = 2);
void task4();



int main()
{
    char key = 0;
    Mat intro = imread(CUURENT_DIR + "media/presstowin.jpg");
    imshow("readkey", intro);
    while ((key = waitKey(30)) != 'q')
    {
        switch (key)
        {
            case '1':
                task1();
                break;
            case '2':
                task2();
                break;
            case '3':
                task3();
                break;
            case '4':
                task4();
                break;

            default:
                break;
        }
    };
    return 0;
}

void task1(size_t img_count, size_t crosshairLenth, Scalar color)
{
    Mat inputMatArray [img_count];
    Mat workingMatArray [img_count];
    Moments mom [img_count];
    Point cm [img_count];
    vector<vector<Point>> contours;
    Mat rangeOut [img_count];
    string imgPath("");

    for (size_t i = 0; i != img_count; i++)
    {
        imgPath = string(CUURENT_DIR + "media/task1/img_" + to_string(i) + ".jpg");
        inputMatArray[i] = imread(imgPath);
        cvtColor(inputMatArray[i], workingMatArray[i], COLOR_BGR2GRAY);
        threshold(workingMatArray[i], rangeOut[i], 255 * 0.92, 255, THRESH_BINARY);
        // Беру высокий порог фильтрации, поэтому добавлю диллатации
        dilate(rangeOut[i], rangeOut[i], Mat());
        findContours(rangeOut[i].clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        // Находим  только один контур
//        drawContours(inputMatArray[i], contours, 0, color);
        mom[i] = moments(contours.at(0));
        cm[i] = Point(int(mom[i].m10/mom[i].m00), int(mom[i].m01/mom[i].m00));
        circle(inputMatArray[i], cm[i], crosshairLenth, color, 2);
        line(inputMatArray[i], Point(cm[i].x + crosshairLenth, cm[i].y), Point(cm[i].x - crosshairLenth, cm[i].y), color, 2);
        line(inputMatArray[i], Point(cm[i].x, cm[i].y + crosshairLenth), Point(cm[i].x, cm[i].y - crosshairLenth), color, 2);
//        imshow(to_string(i), rangeOut[i]);
        imshow("out" + to_string(i), inputMatArray[i]);
    }

    while (waitKey(30) != 'q'){}
    for (size_t i = 0; i != img_count; i++)
        cvDestroyWindow(string("out" + to_string(i)).c_str());
    return;
}


void task2(size_t img_count, size_t crosshairLenth, Scalar color)
{
    Mat inputMatArray [img_count];
    Mat workingMatArray [img_count];
    Moments mom [img_count];
    Point cm [img_count];
    vector<vector<Point>> contours;
    string imgPath("");
//    static int lowerb_data[3] = {0, 154, 227};
    static int lowerb_data[3] = {0, 0, 0};
    static int upperb_data[3] = {19, 255, 255};
#if DEBUG==0
    Scalar lowerb(lowerb_data[0], lowerb_data[1], lowerb_data[2]);
    Scalar upperb(upperb_data[0], upperb_data[1], upperb_data[2]);
#endif
    for (size_t i = 0; i != img_count; i++)
    {
        imgPath = i == 4 ? string(CUURENT_DIR + "media/task2/img_" + to_string(i) + ".png")
                         : string(CUURENT_DIR + "media/task2/img_" + to_string(i) + ".jpg");
        inputMatArray[i] = imread(imgPath);
        cvtColor(inputMatArray[i], workingMatArray[i], COLOR_BGR2HSV);
        cvtColor(inputMatArray[i], inputMatArray[i], COLOR_BGR2HSV);
#if DEBUG==0
        inRange(inputMatArray[i], lowerb, upperb, workingMatArray[i]);
        morphologyEx(workingMatArray[i], workingMatArray[i], MORPH_CLOSE, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(30, 30)));
        morphologyEx(workingMatArray[i], workingMatArray[i], MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(3, 4)));
        findContours(workingMatArray[i].clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
        mom[i] = moments(contours.at(0));
        cm[i] = Point(int(mom[i].m10/mom[i].m00), int(mom[i].m01/mom[i].m00));
        cvtColor(inputMatArray[i], inputMatArray[i], COLOR_HSV2BGR);
        circle(inputMatArray[i], cm[i], crosshairLenth, color, 2);
        line(inputMatArray[i], Point(cm[i].x + crosshairLenth, cm[i].y), Point(cm[i].x - crosshairLenth, cm[i].y), color, 2);
        line(inputMatArray[i], Point(cm[i].x, cm[i].y + crosshairLenth), Point(cm[i].x, cm[i].y - crosshairLenth), color, 2);

//        drawContours(inputMatArray[i], contours, 0, color);
//        imshow("out" + to_string(i), workingMatArray[i]);

        imshow("out" + to_string(i), inputMatArray[i]);
#endif

    }

    while (waitKey(1) != 'q')
    {
#if DEBUG==1
        for (size_t i = 0; i != img_count; i++)
        {
            Scalar lowerb(lowerb_data[0], lowerb_data[1], lowerb_data[2]);
            Scalar upperb(upperb_data[0], upperb_data[1], upperb_data[2]);
            inRange(inputMatArray[i], lowerb, upperb, workingMatArray[i]);
            morphologyEx(workingMatArray[i], workingMatArray[i], MORPH_CLOSE, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(30, 30)));
            morphologyEx(workingMatArray[i], workingMatArray[i], MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(3, 4)));
            createTrackbar("upper" + string("_H"), "out" + to_string(0), &upperb_data[0], 179);
            createTrackbar("upper" + string("_S"), "out" + to_string(0), &upperb_data[1], 255);
            createTrackbar("upper" + string("_V"), "out" + to_string(0), &upperb_data[2], 255);
            createTrackbar("lower" + string("_H"), "out" + to_string(0), &lowerb_data[0], 179);
            createTrackbar("lower" + string("_S"), "out" + to_string(0), &lowerb_data[1], 255);
            createTrackbar("lower" + string("_V"), "out" + to_string(0), &lowerb_data[2], 255);
            imshow("out" + to_string(i), workingMatArray[i]);
        }
#endif
    }
    for (size_t i = 0; i != img_count; i++)
        cvDestroyWindow(string("out" + to_string(i)).c_str());
    return;
}


void findRobot (Mat& src, Mat& srcInv, Mat& mask, Scalar color, Point colorRange, vector<vector<Point>>& contours);
void findLamp (Mat& src, Mat& srcInv, Mat& mask, Scalar color, vector<vector<Point>>& contours);
void findObject (Mat& src, Scalar color, Scalar lowerb, Scalar upperb, vector<vector<Point>>& contours, int closeFilterCoef = 10, int openFilterCoef = 6);
vector<double> calculateDistance(const Point& root, const vector<Moments>& contoursMom);
Point findCMOfCloserObj(vector<double>& src, vector<Moments>& mom);


void task3(size_t img_count)
{
    Mat inputMatArray [img_count];
    Mat inputInverseMatArray [img_count];
    Mat workingMatArray [img_count];
    Mat lampMask [img_count];

    Scalar colorB = Scalar(255, 0, 0);
    Scalar colorG = Scalar(0, 255, 0);
    Scalar colorR = Scalar(0, 0, 255);

    vector<vector<Moments>> momR;
    vector<vector<Moments>> momG;
    vector<vector<Moments>> momB;
    vector<Moments> lampMom;
    Point lampCM [img_count];

    vector<vector<Point>> contoursR;
    vector<vector<Point>> contoursG;
    vector<vector<Point>> contoursB;
    vector<vector<Point>> contoursL;

    vector<double> distancesFromCM_R [img_count];
    vector<double> distancesFromCM_G [img_count];
    vector<double> distancesFromCM_B [img_count];

    string imgPath("");

// Неплохой вариант
    static int lowerb_data[3] = {120, 77, 48};
    static int upperb_data[3] = {179, 255, 255};
#if DEBUG==0
    Scalar lowerb(lowerb_data[0], lowerb_data[1], lowerb_data[2]);
    Scalar upperb(upperb_data[0], upperb_data[1], upperb_data[2]);
#endif

    for (size_t i = 0; i != img_count; i++)
    {
        imgPath = string(CUURENT_DIR + "media/task3/img_" + to_string(i) + ".jpg");
        inputMatArray[i] = imread(imgPath);

#if DEBUG==0
        findObject(inputMatArray[i], colorB, Scalar(0, 84, 102), Scalar(20, 255, 255), contoursB);
        findObject(inputMatArray[i], colorR, Scalar(80, 84, 102), Scalar(120, 255, 255), contoursR, 33, 8);
        findObject(inputMatArray[i], colorG, Scalar(120, 77, 48), Scalar(179, 255, 255), contoursG, 9, 8);
        findObject(inputMatArray[i], Scalar(0, 0, 0), Scalar(0, 0, 0), Scalar(179, 255, 10), contoursL);

        lampMom.push_back(moments(contoursL.back()));
        lampCM[i] = Point(int(lampMom.at(i).m10/lampMom.at(i).m00), int(lampMom.at(i).m01/lampMom.at(i).m00));
        momR.emplace_back();
        for (auto it:contoursR)
        {
            momR.at(i).push_back(moments(it));
        }
        momG.emplace_back();
        for (auto it:contoursG)
        {
            momG.at(i).push_back(moments(it));
        }
        momB.emplace_back();
        for (auto it:contoursB)
        {
            momB.at(i).push_back(moments(it));
        }
        distancesFromCM_R[i] = calculateDistance(lampCM[i], momR.at(i));
        distancesFromCM_G[i] = calculateDistance(lampCM[i], momG.at(i));
        distancesFromCM_B[i] = calculateDistance(lampCM[i], momB.at(i));

        Point redTeamCloserCM = findCMOfCloserObj(distancesFromCM_R[i], momR.at(i));
        Point blueTeamCloserCM = findCMOfCloserObj(distancesFromCM_B[i], momB.at(i));
        Point greenTeamCloserCM = findCMOfCloserObj(distancesFromCM_G[i], momG.at(i));

        circle(inputMatArray[i], redTeamCloserCM, 5, Scalar(255, 255, 255), -1);
        circle(inputMatArray[i], blueTeamCloserCM, 5, Scalar(255, 255, 255), -1);
        circle(inputMatArray[i], greenTeamCloserCM, 5, Scalar(255, 255, 255), -1);



        imshow("in" + to_string(i), inputMatArray[i]);
#endif



//        cvtColor(inputMatArray[i], inputMatArray[i], COLOR_HSV2BGR);
//        imshow("in" + to_string(i), workingMatArray[i]);
    }
    int closeFilterCoef = 9;
    int openFilterCoef = 8;
    while (waitKey(1) != 'q')
    {
#if DEBUG==1
        for (size_t i = 0; i != img_count; i++)
        {
            inputInverseMatArray[i] = ~inputMatArray[i].clone();
            cvtColor(inputInverseMatArray[i], inputInverseMatArray[i], COLOR_BGR2HSV);
            Scalar lowerb(lowerb_data[0], lowerb_data[1], lowerb_data[2]);
            Scalar upperb(upperb_data[0], upperb_data[1], upperb_data[2]);
            inRange(inputInverseMatArray[i], lowerb, upperb, workingMatArray[i]);
//            inRange(inputInverseMatArray[i], Scalar(90, 170, 107), Scalar (97, 198, 174), lampMask[i]);
//            inRange(inputInverseMatArray[i], Scalar(77, 71, 113), Scalar (121, 200, 174), lampMask[i]);
            inRange(inputInverseMatArray[i], Scalar(77, 87, 113), Scalar (121, 200, 174), lampMask[i]);
            morphologyEx(lampMask[i], lampMask[i], MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(6, 6)));
            morphologyEx(lampMask[i], lampMask[i], MORPH_CLOSE, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(33, 33)));
            workingMatArray[i] -= lampMask[i];
//            imshow("lampmask" + to_string(i), lampMask[i]);

            if (openFilterCoef != 0)
                morphologyEx(workingMatArray[i], workingMatArray[i], MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(openFilterCoef, openFilterCoef)));
            if (closeFilterCoef != 0)
                morphologyEx(workingMatArray[i], workingMatArray[i], MORPH_CLOSE, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(closeFilterCoef, closeFilterCoef)));
            findContours(workingMatArray[i].clone(), contoursG, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS);
//            drawContours(inputMatArray[i], contours, -1, colorBlue, 2);



            createTrackbar("upper" + string("_H"), "out" + to_string(0), &upperb_data[0], 179);
            createTrackbar("upper" + string("_S"), "out" + to_string(0), &upperb_data[1], 255);
            createTrackbar("upper" + string("_V"), "out" + to_string(0), &upperb_data[2], 255);
            createTrackbar("lower" + string("_H"), "out" + to_string(0), &lowerb_data[0], 179);
            createTrackbar("lower" + string("_S"), "out" + to_string(0), &lowerb_data[1], 255);
            createTrackbar("lower" + string("_V"), "out" + to_string(0), &lowerb_data[2], 255);
            createTrackbar("close", "out" + to_string(0), &closeFilterCoef, 50);
            createTrackbar("open", "out" + to_string(0), &openFilterCoef, 50);
            Mat srcClone = inputMatArray[i].clone();
//            findObject(inputMatArray[i], colorB, Scalar(0, 103, 76), Scalar(20, 255, 255), contoursB);
//            findObject(srcClone, colorR, Scalar(80, 103, 76), Scalar(120, 255, 255), contoursR);
//            findObject(inputMatArray[i], colorG, Scalar(120, 103, 76), Scalar(179, 255, 255), contoursG);
            findContours(workingMatArray[i].clone(), contoursG, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            drawContours(srcClone, contoursG, -1, colorR, 2);
//            drawContours(srcClone, contoursB, -1, colorB, 2);
//            drawContours(srcClone, contoursG, -1, colorG, 2);
//            imshow("out" + to_string(i), workingMatArray[i]);
            imshow("in" + to_string(i), srcClone);
            imshow("out" + to_string(i), workingMatArray[i]);
        }
#endif
    }
    for (size_t i = 0; i != img_count; i++)
        cvDestroyWindow(string("in" + to_string(i)).c_str());
    return;
}

void findObject (Mat& src, Scalar color, Scalar lowerb, Scalar upperb, vector<vector<Point>>& contours, int closeFilterCoef, int openFilterCoef)
{
//    static const int closeFilterCoef = 33;
//    static const int openFilterCoef = 8;

    Mat lampMask;
    Mat srcInv = ~src.clone();
    Mat tmp;
    cvtColor(srcInv, srcInv, COLOR_BGR2HSV);

    inRange(srcInv, lowerb, upperb, tmp);
//    inRange(srcInv, Scalar(90, 170, 107), Scalar (97, 198, 174), lampMask);
    inRange(srcInv, Scalar(77, 87, 113), Scalar (121, 200, 174), lampMask);
    morphologyEx(lampMask, lampMask, MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(6, 6)));
    morphologyEx(lampMask, lampMask, MORPH_CLOSE, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(33, 33)));
    tmp -= lampMask;
    morphologyEx(tmp, tmp, MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(openFilterCoef, openFilterCoef)));
    morphologyEx(tmp, tmp, MORPH_CLOSE, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(closeFilterCoef, closeFilterCoef)));
    findContours(tmp.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    drawContours(src, contours, -1, color, 2);
}


vector<double> calculateDistance(const Point& root, const vector<Moments>& contoursMom)
{
    vector<double> out;
    for (auto it:contoursMom)
    {
        Point cm = Point(int(it.m10/it.m00), int(it.m01/it.m00));
        double dx2 = pow((root.x - cm.x), 2);
        double dy2 = pow((root.y - cm.y), 2);
        out.push_back(sqrt(dx2 + dy2));
    }
    return out;
}

Point findCMOfCloserObj(vector<double>& src, vector<Moments>& mom)
{
    auto min_it = min_element(src.begin(), src.end());
    size_t indx = min_it - src.begin();
    return Point(int(mom.at(indx).m10/mom.at(indx).m00), int(mom.at(indx).m01/mom.at(indx).m00));
}


void task4()
{
    string imgPath = string(CUURENT_DIR + "media/task4/img_0" + ".jpg");

    Mat input = imread(imgPath);
    Mat inputProcessed = input.clone();
    Mat templOrig = imread(CUURENT_DIR + "media/task4/img_template.jpg");
    Mat templ = imread(CUURENT_DIR + "media/task4/img_template.jpg", IMREAD_GRAYSCALE);

    Mat inputGrey;
    Mat edges;
    vector<vector<Point>> contours;
    vector<vector<Point>> templContour;
    vector<double> matches;

    cvtColor(input, inputGrey, COLOR_BGR2GRAY);

    static int openCoef = 3;
    static int thresh1 = 235;
    static int thresh2 = 255;

    static double compareThreshold = 0.2;

#if DEBUG==0

    threshold(inputGrey, edges, thresh1, thresh2, THRESH_BINARY);
    threshold(templ, templ, thresh1, thresh2, THRESH_BINARY);

    morphologyEx(edges, edges, MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(openCoef, openCoef)));
    morphologyEx(templ, templ, MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(openCoef, openCoef)));

    edges = ~edges;

    findContours(edges.clone(), contours,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    findContours(templ.clone(), templContour,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    for (auto it: contours)
    {
        matches.push_back(matchShapes(templContour[0], it, CV_CONTOURS_MATCH_I2, 0));
    }

    for (auto it: contours)
    {
        static size_t i = 0;
        if (matches.at(i) < compareThreshold)
            drawContours(inputProcessed, contours, i, Scalar(0, 255, 0), -1);
        else
            drawContours(inputProcessed, contours, i, Scalar(0, 0, 255), -1);
        i++;
    }

#endif


    while (waitKey(30) != 'q')
    {
#if DEBUG==1
        Mat templCopy = templ.clone();
//
        threshold(inputGrey, edges, 235, 255, THRESH_BINARY);
        threshold(templ, templCopy, 235, 255, THRESH_BINARY);
        if (openCoef)
        {
            morphologyEx(edges, edges, MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(openCoef, openCoef)));
            morphologyEx(templCopy, templCopy, MORPH_OPEN, getStructuringElement(cv::MorphShapes::MORPH_ELLIPSE, Size(openCoef, openCoef)));
        }
        edges = ~edges;
//        Mat cannyOut;
//        Canny(edges, cannyOut, thresh1, thresh2);
        createTrackbar("cannyTrackbar1", "edges", &thresh1, 255);
        createTrackbar("cannyTrackbar2", "edges", &thresh2, 255);
        createTrackbar("morph", "edges", &openCoef, 20);
        findContours(edges.clone(), contours,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        findContours(templCopy.clone(), templContour,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        for (auto it: contours)
        {
            matches.push_back(matchShapes(templContour[0], it, CV_CONTOURS_MATCH_I2, 0));
        }
        drawContours(templOrig, templContour, -1, Scalar(255, 0, 255), 1);
        drawContours(input, contours, 6, Scalar(255, 0, 255), 1);
        drawContours(input, contours, 7, Scalar(255, 0, 255), 1);
        drawContours(input, contours, 8, Scalar(255, 0, 255), 1);
        imshow("edges", edges);
        imshow("templCopy", templCopy);
        imshow("template", templOrig);
#endif
        imshow("input", input);
        imshow("output", inputProcessed);
    }
    cvDestroyWindow("output");
    cvDestroyWindow("input");


}
