#pragma once

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

enum class MorphologicalOperation
{
    EROSION,
    DILATION
};

enum Border
{
    TOP,
    LEFT,
    BOTTOM,
    RIGHT
};

enum Colour
{
    BLUE,
    GREEN,
    RED
};

class MorphologicalFilter
{
public:
    MorphologicalFilter() = default;
    ~MorphologicalFilter() = default;

    void erode();
    void dilate();
    void open();
    void close();

    void setStructuringElement(const cv::Mat structuringElement);
    cv::Mat getStructuringElement() const;

    void setAnchor(const cv::Point2i anchor);
    cv::Point2i getAnchor() const;

    void setImage(const cv::Mat structure);
    cv::Mat getImage() const;

private:
    void fillBorders(cv::Mat& buffer);
    bool compare(const cv::Mat& box, MorphologicalOperation operation);
    cv::Mat binarize(const cv::Mat& image) const;
    cv::Mat normalize(const cv::Mat& image) const;

    cv::Mat m_structuringElement;
    cv::Point2i m_anchor;
    cv::Mat m_image;
    std::vector<int> m_border;
};
