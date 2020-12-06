#include "functions.h"

#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

enum Team
{
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    TEAMS = 3
};

const auto black  = Scalar(0x00, 0x00, 0x00);
const auto red    = Scalar(0x00, 0x00, 0xFF);
const auto green  = Scalar(0x00, 0xFF, 0x00);
const auto blue   = Scalar(0xFF, 0x00, 0x00);
const auto white  = Scalar(0xFF, 0xFF, 0xFF);
const auto yellow = Scalar(0x00, 0xFF, 0xFF);

const auto thresh_min = 0x00;
const auto thresh_max = 0xFF;

const auto hue_min = 0;
const auto hue_max = 360;
const auto sat_min = 0;
const auto sat_max = 100;
const auto val_min = 0;
const auto val_max = 100;

typedef vector<vector<Point>> Contours;

Mat processImage(Part part, Mat image, Mat imageTemplate)
{
    auto result = Mat();

    switch (part)
    {
    case Part::PART_1:
        result = findEnemyCover(image);
        break;

    case Part::PART_2:
        result = findEnemyTarget(image);
        break;

    case Part::PART_3:
        result = findRobotTeams(image);
        break;

    case Part::PART_4:
        result = findCorrectAndWrongKeys(image, imageTemplate);
        break;

    default:
        result = image.clone();
        break;
    }

    return result;
}

Mat findEnemyCover(Mat image, String imageName)
{
    auto result = Mat();

    if (image.empty() == true)
    {
        cout << endl << "The image is empty!" << endl;
        return result;
    }

    auto thresh = thresh_max;
    void *data[] =
    {
        (void *)&imageName,
        (void *)&image,
        (void *)&result
    };

    detectCovers(thresh, data);
    createTrackbar("Thresh", imageName, &thresh, thresh_max, detectCovers, data);

    while (waitKey() != 27);

    destroyWindow(imageName);

    return result;
}

Mat findEnemyTarget(Mat image, String imageName)
{
    auto result = Mat();

    if (image.empty() == true)
    {
        cout << endl << "The image is empty!" << endl;
        return result;
    }

    auto hue = hue_min;
    auto sat = sat_min;
    auto val = val_min;

    auto hueWidth = hue_max - hue_min;
    auto satWidth = sat_max - sat_min;
    auto valWidth = val_max - val_min;

    void *data[] =
    {
        (void *)&hue,
        (void *)&sat,
        (void *)&val,
        (void *)&hueWidth,
        (void *)&satWidth,
        (void *)&valWidth,
        (void *)&imageName,
        (void *)&image,
        (void *)&result
    };        

    detectTargets(0, data);
    createTrackbar("Hue"       , imageName, &hue     , hue_max, detectTargets, data);
    createTrackbar("H Width"   , imageName, &hueWidth, hue_max, detectTargets, data);
    createTrackbar("Saturation", imageName, &sat     , sat_max, detectTargets, data);
    createTrackbar("S Width"   , imageName, &satWidth, sat_max, detectTargets, data);
    createTrackbar("Value"     , imageName, &val     , val_max, detectTargets, data);
    createTrackbar("V Width"   , imageName, &valWidth, val_max, detectTargets, data);

    while (waitKey() != 27);

    destroyWindow(imageName);

    return result;
}

Mat findRobotTeams(Mat image, String imageName)
{
    auto result = Mat();

    if (image.empty() == true)
    {
        cout << endl << "The image is empty!" << endl;
        return result;
    }

    auto hue = hue_min;
    auto sat = sat_min;
    auto val = val_min;

    auto hueWidth = hue_max - hue_min;
    auto satWidth = sat_max - sat_min;
    auto valWidth = val_max - val_min;

    int32_t currTeam = Team::RED;

    void *data[] =
    {
        (void *)&hue,
        (void *)&sat,
        (void *)&val,
        (void *)&hueWidth,
        (void *)&satWidth,
        (void *)&valWidth,
        (void *)&currTeam,
        (void *)&imageName,
        (void *)&image,
        (void *)&result
    };        

    detectRobots(0, data);
    createTrackbar("Hue"       , imageName, &hue      , hue_max, detectRobots, data);
    createTrackbar("H Width"   , imageName, &hueWidth , hue_max, detectRobots, data);
    createTrackbar("Saturation", imageName, &sat      , sat_max, detectRobots, data);
    createTrackbar("S Width"   , imageName, &satWidth , sat_max, detectRobots, data);
    createTrackbar("Value"     , imageName, &val      , val_max, detectRobots, data);
    createTrackbar("V Width"   , imageName, &valWidth , val_max, detectRobots, data);

    while (currTeam < Team::TEAMS)
    {
        while (waitKey() != 27);

        currTeam++;
        image = result.clone();
    }

    destroyWindow(imageName);

    return result;
}

Mat findCorrectAndWrongKeys(Mat image, Mat imageTemplate,
                            String imageName, String imageTemplateName)
{
    auto result = Mat();

    if (image.empty() == true)
    {
        cout << endl << "The image is empty!" << endl;
        return result;
    }

    if (imageTemplate.empty() == true)
    {
        cout << endl << "The image template is empty!" << endl;
        return result;
    }

    auto thresh = thresh_max;
    void *data[] =
    {
        (void *)&imageName,
        (void *)&image,
        (void *)&imageTemplateName,
        (void *)&imageTemplate,
        (void *)&result
    };

    detectTemplates(thresh, data);
    createTrackbar("Thresh", imageName, &thresh, thresh_max, detectTemplates, data);

    while (waitKey() != 27);

    destroyAllWindows();

    return result;
}

void detectCovers(int32_t thresh, void *data)
{
    auto imageName = **((String **)data + 0);
    auto image     = **((Mat **   )data + 1);

    auto buffer = image.clone();
    cvtColor(buffer, buffer, ColorConversionCodes::COLOR_BGR2GRAY);
    threshold(buffer, buffer, thresh, thresh_max, ThresholdTypes::THRESH_BINARY);
    imshow("Threshold Result", buffer);

    morphologyEx(buffer, buffer, MorphTypes::MORPH_OPEN,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(9, 9)));
    morphologyEx(buffer, buffer, MorphTypes::MORPH_GRADIENT,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3, 3)));
    imshow("Open Filter and Gradient Result", buffer);

    Contours contours;
    findContours(buffer, contours, RetrievalModes::RETR_EXTERNAL,
        ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    auto colour = red;
    auto result = image.clone();
    polylines(result, contours, true, colour);
    for (auto &contour : contours)
    {
        auto moment = moments(contour);
        auto center = Point(moment.m10 / moment.m00,
                            moment.m01 / moment.m00);
        circle(result, center, 0, colour, 2);
        line(result, Point(center.x - 15, center.y),
                     Point(center.x -  5, center.y), colour, 2);
        line(result, Point(center.x + 15, center.y),
                     Point(center.x +  5, center.y), colour, 2);
        line(result, Point(center.x, center.y - 15),
                     Point(center.x, center.y -  5), colour, 2);
        line(result, Point(center.x, center.y + 15),
                     Point(center.x, center.y +  5), colour, 2);
    }

    imshow(imageName, result);
    **((Mat **)data + 2) = result;
}

void detectTargets(int32_t thresh, void *data)
{
    auto hue       = **((int32_t **)data + 0);
    auto sat       = **((int32_t **)data + 1);
    auto val       = **((int32_t **)data + 2);
    auto hueWidth  = **((int32_t **)data + 3);
    auto satWidth  = **((int32_t **)data + 4);
    auto valWidth  = **((int32_t **)data + 5);
    auto imageName = **((String  **)data + 6);
    auto image     = **((Mat **    )data + 7);

    auto hueLow  = hue - hueWidth / 2;
    auto hueHigh = hue + (hueWidth + 1) / 2;
    auto satLow  = sat - satWidth / 2;
    auto satHigh = sat + (satWidth + 1) / 2;
    auto valLow  = val - valWidth / 2;
    auto valHigh = val + (valWidth + 1) / 2;

    vector<Scalar> lowerb;
    vector<Scalar> upperb;

    getLowerAndUpperHSVBounds(hueLow, hueHigh, 
                              satLow, satHigh, 
                              valLow, valHigh, 
                              lowerb, upperb);

    auto buffer = image.clone();
    cvtColor(buffer, buffer, ColorConversionCodes::COLOR_BGR2HSV);
    auto sum = Mat(image.size(), CV_8UC1, Scalar(0));
    for (auto index = 0; index < max(lowerb.size(), upperb.size()); index++)
    {
        auto elem = Mat();
        inRange(buffer, lowerb.at(index), upperb.at(index), elem);
        bitwise_or(sum, elem, sum);
    }
    buffer = sum;
    imshow("Threshold Result", buffer);

    morphologyEx(buffer, buffer, MorphTypes::MORPH_OPEN,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(9, 9)));
    morphologyEx(buffer, buffer, MorphTypes::MORPH_GRADIENT,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3, 3)));
    imshow("Open Filter and Gradient Result", buffer);

    Contours contours;
    findContours(buffer, contours, RetrievalModes::RETR_EXTERNAL,
        ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    auto colour = black;
    auto result = image.clone();
    polylines(result, contours, true, colour);
    for (auto &contour : contours)
    {
        auto moment = moments(contour);
        auto center = Point(moment.m10 / moment.m00,
                            moment.m01 / moment.m00);
        circle(result, center, 0, colour, 2);
        line(result, Point(center.x - 15, center.y),
                     Point(center.x -  5, center.y), colour, 2);
        line(result, Point(center.x + 15, center.y),
                     Point(center.x +  5, center.y), colour, 2);
        line(result, Point(center.x, center.y - 15),
                     Point(center.x, center.y -  5), colour, 2);
        line(result, Point(center.x, center.y + 15),
                     Point(center.x, center.y +  5), colour, 2);
    }

    imshow(imageName, result);
    **((Mat **)data + 8) = result;
}

void detectRobots(int32_t thresh, void *data)
{
    static const Scalar teamColour[Team::TEAMS] =
    {
        red,
        green,
        blue
    };

    auto hue       = **((int32_t **)data + 0);
    auto sat       = **((int32_t **)data + 1);
    auto val       = **((int32_t **)data + 2);
    auto hueWidth  = **((int32_t **)data + 3);
    auto satWidth  = **((int32_t **)data + 4);
    auto valWidth  = **((int32_t **)data + 5);
    auto currTeam  = **((int32_t **)data + 6);
    auto imageName = **((String  **)data + 7);
    auto image     = **((Mat **    )data + 8);

    auto hueLow  = hue - hueWidth / 2;
    auto hueHigh = hue + (hueWidth + 1) / 2;
    auto satLow  = sat - satWidth / 2;
    auto satHigh = sat + (satWidth + 1) / 2;
    auto valLow  = val - valWidth / 2;
    auto valHigh = val + (valWidth + 1) / 2;

    vector<Scalar> lowerb;
    vector<Scalar> upperb;

    getLowerAndUpperHSVBounds(hueLow, hueHigh,
                              satLow, satHigh,
                              valLow, valHigh,
                              lowerb, upperb);

    auto lamp = image.clone();
    cvtColor(lamp, lamp, ColorConversionCodes::COLOR_BGR2GRAY);
    threshold(lamp, lamp, 0.99 * thresh_max, thresh_max, ThresholdTypes::THRESH_BINARY);
    morphologyEx(lamp, lamp, MorphTypes::MORPH_OPEN,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(9, 9)));
    morphologyEx(lamp, lamp, MorphTypes::MORPH_GRADIENT,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3, 3)));

    Contours lampContours;
    findContours(lamp, lampContours, RetrievalModes::RETR_EXTERNAL,
        ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    auto buffer = image.clone();
    cvtColor(buffer, buffer, ColorConversionCodes::COLOR_BGR2HSV);
    auto sum = Mat(image.size(), CV_8UC1, Scalar(0));
    for (auto index = 0; index < max(lowerb.size(), upperb.size()); index++)
    {
        auto elem = Mat();
        inRange(buffer, lowerb.at(index), upperb.at(index), elem);
        bitwise_or(sum, elem, sum);
    }
    buffer = sum;
    imshow("Threshold Result", buffer);

    morphologyEx(buffer, buffer, MorphTypes::MORPH_OPEN,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(9, 9)));
    Canny(buffer, buffer, 0, 0);
    imshow("Open Filter and Canny Result", buffer);

    Contours teamContours;
    findContours(buffer, teamContours, RetrievalModes::RETR_EXTERNAL,
        ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    auto result = image.clone();
    auto lampColour = yellow;
    static auto lampCenter = Point();
    for (auto &contour : lampContours)
    {
        auto moment = moments(contour);
        lampCenter = Point(moment.m10 / moment.m00,
                           moment.m01 / moment.m00);
        
        circle(result, lampCenter, 10, lampColour, FILLED);
    }

    auto teamCenter = Point();
    auto minDistance = DBL_MAX;
    for (auto &contour : teamContours)
    {
        auto moment = moments(contour);
        auto center = Point(moment.m10 / moment.m00,
                            moment.m01 / moment.m00);
        auto distance = hypot(center.x - lampCenter.x,
                              center.y - lampCenter.y);
        if (distance < minDistance)
        {
            minDistance = distance;
            teamCenter = center;
        }
    }

    circle(result, teamCenter, 3, teamColour[currTeam], FILLED);
    polylines(result, teamContours, true, teamColour[currTeam], 1);
    
    imshow(imageName, result);
    **((Mat **)data + 9) = result;
}

void detectTemplates(int32_t thresh, void *data)
{
    auto imageName         = **((String **)data + 0);
    auto image             = **((Mat **   )data + 1);
    auto imageTemplateName = **((String **)data + 2);
    auto imageTemplate     = **((Mat **   )data + 3);

    auto buffer = image.clone();
    cvtColor(buffer, buffer, ColorConversionCodes::COLOR_BGR2GRAY);
    threshold(buffer, buffer, thresh, thresh_max, ThresholdTypes::THRESH_BINARY_INV);
    imshow("Treshold Result", buffer);

    morphologyEx(buffer, buffer, MorphTypes::MORPH_OPEN,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(9, 9)));
    morphologyEx(buffer, buffer, MorphTypes::MORPH_GRADIENT,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3, 3)));
    imshow("Open Filter and Gradient Result", buffer);

    Contours imageContours;
    findContours(buffer, imageContours, RetrievalModes::RETR_EXTERNAL,
        ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    auto templateBound = imageTemplate.clone();
    cvtColor(templateBound, templateBound, ColorConversionCodes::COLOR_BGR2GRAY);

    morphologyEx(templateBound, templateBound, MorphTypes::MORPH_OPEN,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(9, 9)));
    morphologyEx(templateBound, templateBound, MorphTypes::MORPH_GRADIENT,
        getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3, 3)));

    Contours templateContour;
    findContours(templateBound, templateContour, RetrievalModes::RETR_EXTERNAL,
        ContourApproximationModes::CHAIN_APPROX_SIMPLE);

    vector<double> weights;
    for (auto &contour : imageContours)
    {
        auto weight = matchShapes(contour, templateContour.front(),
            ShapeMatchModes::CONTOURS_MATCH_I2, 0);
        weights.push_back(weight);
    }

    auto correctColour = green;
    auto wrongColour = red;
    auto result = image.clone();

    for (auto index = 0; index < weights.size(); index++)
    {
        auto colour = correctColour;
        if (weights.at(index) > 0.25)
        {
            colour = wrongColour;
        }

        auto moment = moments(imageContours.at(index));
        polylines(result, imageContours.at(index), true, colour);
        auto center = Point(moment.m10 / moment.m00,
                            moment.m01 / moment.m00);
        circle(result, center, 0, colour, 2);
        line(result, Point(center.x - 15, center.y),
                     Point(center.x -  5, center.y), colour, 2);
        line(result, Point(center.x + 15, center.y),
                     Point(center.x +  5, center.y), colour, 2);
        line(result, Point(center.x, center.y - 15),
                     Point(center.x, center.y -  5), colour, 2);
        line(result, Point(center.x, center.y + 15),
                     Point(center.x, center.y +  5), colour, 2);
    }

    imshow(imageName, result);
    imshow(imageTemplateName, imageTemplate);

    **((Mat **)data + 4) = result;
}

void getLowerAndUpperHSVBounds(int32_t hueLow, int32_t hueHigh,
                               int32_t satLow, int32_t satHigh,
                               int32_t valLow, int32_t valHigh,
                               vector<Scalar> &lowerb, vector<Scalar> &upperb)
{
    for (auto hueCurr = hueLow, hueNext = hueHigh; hueCurr != hueNext;
         hueCurr = hueNext % (hue_max - hue_min),
         hueNext = hueHigh % (hue_max - hue_min))
    {
        if (hueCurr < hue_min)
        {
            hueCurr = hueCurr + hue_max;
            hueNext = hue_max;
        }
        if (hueNext > hue_max)
        {
            hueNext = hue_max;
        }

        for (auto satCurr = satLow, satNext = satHigh; satCurr != satNext;
             satCurr = satNext % (sat_max - sat_min),
             satNext = satHigh % (sat_max - sat_min))
        {
            if (satCurr < hue_min)
            {
                satCurr = satCurr + sat_max;
                satNext = sat_max;
            }
            if (satNext > sat_max)
            {
                satNext = sat_max;
            }

            for (auto valCurr = valLow, valNext = valHigh; valCurr != valNext;
                 valCurr = valNext % (val_max - val_min),
                 valNext = valHigh % (val_max - val_min))
            {
                if (valCurr < val_min)
                {
                    valCurr = valCurr + val_max;
                    valNext = val_max;
                }
                if (valNext > val_max)
                {
                    valNext = val_max;
                }

                lowerb.push_back(Scalar(cvtHue(hueCurr), cvtSat(satCurr), cvtVal(valCurr)));
                upperb.push_back(Scalar(cvtHue(hueNext), cvtSat(satNext), cvtVal(valNext)));
            }
        }
    }
}
