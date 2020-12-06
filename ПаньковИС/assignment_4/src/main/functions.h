#pragma once

#include <stdint.h>
#include "opencv2/core.hpp"

enum class Part
{
    PART_1 = 0,
    PART_2 = 1,
    PART_3 = 2,
    PART_4 = 3,
    PARTS  = 4
};

cv::Mat processImage(Part part, cv::Mat image, cv::Mat imageTemplate = cv::Mat());
cv::Mat findEnemyCover(cv::Mat image, cv::String imageName = "Detect Enemy Cover");
cv::Mat findEnemyTarget(cv::Mat image, cv::String imageName = "Detect Enemy Target");
cv::Mat findRobotTeams(cv::Mat image, cv::String imageName = "Detect Robot Teams");
cv::Mat findCorrectAndWrongKeys(cv::Mat image, cv::Mat imageTemplate,
                                cv::String imageName = "Detect Keys",
                                cv::String imageTemplateName = "Key Template");

void detectCovers(int32_t thresh, void *data);
void detectTargets(int32_t thresh, void *data);
void detectRobots(int32_t thresh, void* data);
void detectTemplates(int32_t thresh, void *data);

inline int32_t cvtHue(int32_t hue)
{
    return (int32_t)((float)hue / 360.0f * 179.0f);
}

inline int32_t cvtSat(int32_t sat)
{
    return (int32_t)((float)sat / 100.0f * 255.0f);
}

inline int32_t cvtVal(int32_t val)
{
    return (int32_t)((float)val / 100.0f * 255.0f);
}

void getLowerAndUpperHSVBounds(int32_t hueStart, int32_t hueEnd,
                               int32_t satStart, int32_t satEnd,
                               int32_t valStart, int32_t valEnd,
                               std::vector<cv::Scalar> &lowerb, 
                               std::vector<cv::Scalar> &upperb);