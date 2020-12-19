#pragma once

#include "my_filter.h"

MyFilter::MyFilter()
{
    m_ksize = 0;
    m_sigma = 0;
    m_maxValue = 0;
    m_constant = 0;
}

MyFilter::~MyFilter()
{
    ;
}

int MyFilter::setImage(Mat& image)
{
    if (image.empty())
    {
        return -1;
    }

    if (image.channels() != 1)
    {
        return -2;
    }

    m_src = image.clone();

    return 0;
}

int MyFilter::setParam(const int ksize, const float sigma, const int maxValue, const int constant)
{
    if (((ksize / 2) == 0) || (ksize < 3))
    {
        return -1;
    }

    if (sigma <= 0)
    {
        return -2;
    }

    if (maxValue < 0 || maxValue > 255)
    {
        return -3;
    }

    if (constant < 0 || constant > 255)
    {
        return -4;
    }

    m_ksize = ksize;
    m_sigma = sigma;
    m_maxValue = maxValue;
    m_constant = constant;

    return 0;
}

Mat MyFilter::getImage()
{
    return m_dst;
}

int MyFilter::makeKernel()
{
    if (m_ksize == 0 || m_sigma == 0)
    {
        return -1;
    }

    float value = 0;
    int x = -2;
    int y = 2;

    for (int i = 0; i < (m_ksize * m_ksize); i++)
    {
        if (((i % m_ksize) == 0) && (i != 0))
        {
            x = -2;
            y -= 1;
        }

        value = (1 / (2 * (float)CV_PI * m_sigma * m_sigma)) * exp(-((float)(x * x) + (float)(y * y)) / (2 * m_sigma * m_sigma));
        m_coef.push_back(value);

        x += 1;
    }

   /* for (int i = 0; i < (m_ksize * m_ksize); i++)
    {
        if (((i % m_ksize) == 0) && (i != 0))
        {
            cout << endl;
        }

        cout << m_coef[i] << " ";
    }*/

    return 0;
}

int MyFilter::processImage()
{
    if (m_src.empty())
    {
        return -1;
    }

    if (m_coef.empty())
    {
        return -2;
    }

    Mat tmpImage((m_src.rows + (m_ksize - 1)), (m_src.cols + (m_ksize - 1)), m_src.type(), Scalar(255));

    //Copy src
    for (int i = (m_ksize / 2); i < (tmpImage.rows - (m_ksize / 2)); i++)
    {
        for (int j = (m_ksize / 2); j < (tmpImage.cols - (m_ksize / 2)); j++)
        {
            tmpImage.at<uint8_t>(i, j) = m_src.at<uint8_t>((i - (m_ksize / 2)), (j - (m_ksize / 2)));
        }
    }

    imshow("First", tmpImage);

    //Border reflect
    //Left -> right -> top -> bottom
    int count = 0;

    for (int i = (m_ksize / 2); i < (tmpImage.rows - (m_ksize / 2)); i++)
    {
        for (int j = ((m_ksize / 2) - 1); j >= 0; j--)
        {
            tmpImage.at<uint8_t>(i, j) = tmpImage.at<uint8_t>(i, (j + ((2 * count) + 1)));
            count++;
        }
        count = 0;
    }

    for (int i = (m_ksize / 2); i < (tmpImage.rows - (m_ksize / 2)); i++)
    {
        for (int j = (tmpImage.cols - (m_ksize / 2)); j < tmpImage.cols; j++)
        {
            tmpImage.at<uint8_t>(i, j) = tmpImage.at<uint8_t>(i, (j - ((2 * count) + 1)));
            count++;
        }
        count = 0;
    }

    for (int j = 0; j < tmpImage.cols; j++)
    {
        for (int i = ((m_ksize / 2) - 1); i >= 0; i--)
        {
            tmpImage.at<uint8_t>(i, j) = tmpImage.at<uint8_t>((i + ((2 * count) + 1)), j);
            count++;
        }
        count = 0;
    }

    for (int j = 0; j < tmpImage.cols; j++)
    {
        for (int i = (tmpImage.rows - (m_ksize / 2)); i < tmpImage.rows; i++)
        {
            tmpImage.at<uint8_t>(i, j) = tmpImage.at<uint8_t>((i - ((2 * count) + 1)), j);
            count++;
        }
        count = 0;
    }

    imshow("Second", tmpImage);

    //Adaptive thresholding using Gaussian filter
    m_dst = m_src.clone();

    for (int i = (m_ksize / 2); i < (tmpImage.rows - (m_ksize / 2)); i++)
    {
        for (int j = (m_ksize / 2); j < (tmpImage.cols - (m_ksize / 2)); j++)
        {
            float sum = 0;
            float denominator = 0;
            int x = j - 2;
            int y = i - 2;

            for (int l = 0; l < (m_ksize * m_ksize); l++)
            {
                if (((l % m_ksize) == 0) && (l != 0))
                {
                    x = j - 2;
                    y += 1;
                }

                sum += (float)tmpImage.at<uint8_t>(y, x) * m_coef[l];
                denominator += m_coef[l];

                x += 1;
            }

            float T = sum / denominator - m_constant;

            if (tmpImage.at<uint8_t>(i, j) > (uint8_t)T)
            {
                m_dst.at<uint8_t>((i - (m_ksize / 2)), (j - (m_ksize / 2))) = m_maxValue;
            }
            else
            {
                m_dst.at<uint8_t>((i - (m_ksize / 2)), (j - (m_ksize / 2))) = 0;
            }
        }
    }

    return 0;
}
