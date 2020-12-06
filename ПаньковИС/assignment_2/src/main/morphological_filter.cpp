#include "morphological_filter.h"

using namespace std;
using namespace cv;

void MorphologicalFilter::setStructuringElement(Mat structuringElement)
{
    m_structuringElement = binarize(structuringElement);
    auto anchor = Point2i(structuringElement.cols / 2, structuringElement.rows / 2);
    setAnchor(anchor);
}

Mat MorphologicalFilter::getStructuringElement() const
{
    return normalize(m_structuringElement);
}

void MorphologicalFilter::setAnchor(const Point2i anchor)
{
    m_anchor = anchor;

    vector<int32_t> border =
    {
        m_anchor.y,
        m_anchor.x,
        m_structuringElement.rows - (m_anchor.y + 1),
        m_structuringElement.cols - (m_anchor.x + 1)
    };
    m_border = border;
}

Point2i MorphologicalFilter::getAnchor() const
{
    return m_anchor;
}

void MorphologicalFilter::setImage(Mat image)
{
    m_image = binarize(image);
}

Mat MorphologicalFilter::getImage() const
{
    return normalize(m_image);
}

void MorphologicalFilter::erode()
{
    if (m_image.empty() == true)
    {
        return;
    }

    if (m_structuringElement.empty() == true)
    {
        return;
    }

    if (m_anchor.x < 0 || m_anchor.x >= m_structuringElement.cols)
    {
        return;
    }

    if (m_anchor.y < 0 || m_anchor.y >= m_structuringElement.rows)
    {
        return;
    }

    auto cols = m_image.cols + m_border[LEFT] + m_border[RIGHT];
    auto rows = m_image.rows + m_border[TOP] + m_border[BOTTOM];

    auto buffer = Mat(rows, cols, CV_8UC1);
    m_image.copyTo(buffer(Rect(m_anchor.x, m_anchor.y, m_image.cols, m_image.rows)));

    fillBorders(buffer);

    for (auto col = 0; col < m_image.cols; col++)
    {
        for (auto row = 0; row < m_image.rows; row++)
        {
            auto box = Mat(buffer, Rect(col, row, m_structuringElement.cols, m_structuringElement.rows));
            if (compare(box, MorphologicalOperation::EROSION) == true)
            {
                m_image.at<uint8_t>(row, col) = 1;
            }
            else
            {
                m_image.at<uint8_t>(row, col) = 0;
            }
        }
    }
}

void MorphologicalFilter::dilate()
{
    if (m_image.empty() == true)
    {
        return;
    }

    if (m_structuringElement.empty() == true)
    {
        return;
    }

    if (m_anchor.x < 0 || m_anchor.x >= m_structuringElement.cols)
    {
        return;
    }

    if (m_anchor.y < 0 || m_anchor.y >= m_structuringElement.rows)
    {
        return;
    }

    auto cols = m_image.cols + m_border[LEFT] + m_border[RIGHT];
    auto rows = m_image.rows + m_border[TOP] + m_border[BOTTOM];

    auto buffer = Mat(rows, cols, CV_8UC1);
    m_image.copyTo(buffer(Rect(m_anchor.x, m_anchor.y, m_image.cols, m_image.rows)));

    fillBorders(buffer);

    for (auto col = 0; col < m_image.cols; col++)
    {
        for (auto row = 0; row < m_image.rows; row++)
        {
            auto box = Mat(buffer, Rect(col, row, m_structuringElement.cols, m_structuringElement.rows));
            if (compare(box, MorphologicalOperation::DILATION) == true)
            {
                m_image.at<uint8_t>(row, col) = 1;
            }
            else
            {
                m_image.at<uint8_t>(row, col) = 0;
            }
        }
    }
}

void MorphologicalFilter::open()
{
    erode();
    dilate();
}

void MorphologicalFilter::close()
{
    dilate();
    erode();
}

void MorphologicalFilter::fillBorders(Mat& buffer)
{
    for (auto col = 0; col < m_border[LEFT]; col++)
    {
        for (auto row = 0; row < buffer.rows; row++)
        {
            buffer.at<uint8_t>(row, col) = buffer.at<uint8_t>(row, 2 * m_border[LEFT] - (col + 1));
        }
    }

    for (auto col = 0; col < m_border[RIGHT]; col++)
    {
        for (auto row = 0; row < buffer.rows; row++)
        {
            buffer.at<uint8_t>(row, (buffer.cols - 1) - col) = buffer.at<uint8_t>(row, buffer.cols - 2 * m_border[RIGHT] + col);
        }
    }

    for (auto col = 0; col < buffer.cols; col++)
    {
        for (auto row = 0; row < m_border[TOP]; row++)
        {
            buffer.at<uint8_t>(row, col) = buffer.at<uint8_t>(2 * m_border[TOP] - (row + 1), col);
        }
    }

    for (auto col = 0; col < buffer.cols; col++)
    {
        for (auto row = 0; row < m_border[BOTTOM]; row++)
        {
            buffer.at<uint8_t>((buffer.rows - 1) - row, col) = buffer.at<uint8_t>(buffer.rows - 2 * m_border[BOTTOM] + row, col);
        }
    }
}

bool MorphologicalFilter::compare(const cv::Mat& box, MorphologicalOperation operation)
{
    switch (operation)
    {
    case MorphologicalOperation::EROSION:
        for (auto col = 0; col < box.cols; col++)
        {
            for (auto row = 0; row < box.rows; row++)
            {
                if (m_structuringElement.at<uint8_t>(row, col) != 1)
                {
                    continue;
                }

                if (m_structuringElement.at<uint8_t>(row, col) == box.at<uint8_t>(row, col))
                {
                    return true;
                }
            }
        }
        return false;

    case MorphologicalOperation::DILATION:
        for (auto col = 0; col < box.cols; col++)
        {
            for (auto row = 0; row < box.rows; row++)
            {
                if (m_structuringElement.at<uint8_t>(row, col) != 1)
                {
                    continue;
                }

                if (m_structuringElement.at<uint8_t>(row, col) != box.at<uint8_t>(row, col))
                {
                    return false;
                }
            }
        }
        return true;

    dafault:
        return false;
    }
    
    return false;
}

Mat MorphologicalFilter::binarize(const cv::Mat& image) const
{
    auto buffer = Mat();
    cvtColor(image, buffer, COLOR_BGR2GRAY);
    for (auto col = 0; col < image.cols; col++)
    {
        for (auto row = 0; row < image.rows; row++)
        {
            /*if (image.at<Vec3b>(row, col)[RED]   <= 0x7F &&
                image.at<Vec3b>(row, col)[GREEN] <= 0x7F &&
                image.at<Vec3b>(row, col)[BLUE]  <= 0x7F)*/
            if (buffer.at<uint8_t>(row, col) <= 0x7F)
            {
                buffer.at<uint8_t>(row, col) = 1;
            }
            else
            {
                buffer.at<uint8_t>(row, col) = 0;
            }
        }
    }
    return buffer;
}

Mat MorphologicalFilter::normalize(const cv::Mat& image) const
{
    auto buffer = Mat(image.rows, image.cols, CV_8UC1);
    for (auto col = 0; col < image.cols; col++)
    {
        for (auto row = 0; row < image.rows; row++)
        {
            if (image.at<uint8_t>(row, col) == 1)
            {
                buffer.at<uint8_t>(row, col) = 0x00;
            }
            else
            {
                buffer.at<uint8_t>(row, col) = 0xFF;
            }
        }
    }
    return buffer;
}
