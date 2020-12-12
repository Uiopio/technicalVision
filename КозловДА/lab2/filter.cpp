#include "filter.h"


Otsu::Otsu()
{
}

Otsu::~Otsu()
{
    delete[] m_hist;
}

int Otsu::setImage(Mat img)
{
    if (img.empty())
    {
        return -1;
    }
    m_image = img.clone();
    return 0;
}

Mat Otsu::getImage()
{
    return m_image;
}

int Otsu::processImage()
{
    if (m_image.empty())
    {
        return -1;
    }
    makeHist();
    binary(otsuThreshold(m_image));
    return 0;
}

void Otsu::makeHist()
{
    m_minLuminity = m_image.at<uchar>(0, 0);
    m_maxLuminity = m_image.at<uchar>(0, 0);
    for (int i = 0; i < m_image.rows; i++)
    {
        for (int j = 0; j < m_image.cols; j++)
        {
            int luminity = m_image.at<uchar>(i, j);
            if (luminity < m_minLuminity)
            {
                m_minLuminity = luminity;
            }
            if (luminity > m_maxLuminity)
            {
                m_maxLuminity = luminity;
            }
        }
    }
    int histSize = m_maxLuminity - m_minLuminity + 1;
    m_hist = new int[histSize];
    for (int t = 0; t < histSize; t++)
    {
        m_hist[t] = 0;
    }
    for (int i = 0; i < m_image.rows; i++)
    {
        for (int j = 0; j < m_image.cols; j++)
        {
            m_hist[m_image.at<uchar>(i, j) - m_minLuminity]++;
        }
    }
}

int Otsu::otsuThreshold(Mat img)
{
    int m = 0; // m - сумма высот всех бинов, домноженных на положение их середины
    int n = 0; // n - сумма высот всех бинов
    for (int t = 0; t <= m_maxLuminity - m_minLuminity; t++)
    {
        m += t * m_hist[t];
        n += m_hist[t];
    }
    float maxSigma = -1; // Максимальное значение межклассовой дисперсии
    int threshold = 0; // Порог, соответствующий maxSigma
    int alpha1 = 0; // Сумма высот всех бинов для класса 1
    int beta1 = 0; // Сумма высот всех бинов для класса 1, домноженных на положение их середины
    for (int t = 0; t < m_maxLuminity - m_minLuminity; t++)
    {
        alpha1 += t * m_hist[t];
        beta1 += m_hist[t];
        float w1 = (float)beta1 / n;
        // a = a1 - a2, где a1, a2 - средние арифметические для классов 1 и 2
        float a = (float)alpha1 / beta1 - (float)(m - alpha1) / (n - beta1);
        float sigma = w1 * (1 - w1) * a * a;
        // Если sigma больше текущей максимальной, то обновляем maxSigma и порог
        if (sigma > maxSigma)
        {
            maxSigma = sigma;
            threshold = t;
        }
    }
    threshold += m_minLuminity;
    return threshold;
}

void Otsu::binary(int threshold)
{
    for (int i = 0; i < m_image.rows; i++)
    {
        for (int j = 0; j < m_image.cols; j++)
        {
            if (m_image.at<uchar>(i, j) <= threshold)
            {
                m_image.at<uchar>(i, j) = 0;
            }
            else
            {
                m_image.at<uchar>(i, j) = 255;
            }
        }
    }
}

int compare(Mat img1, Mat img2)
{
    if (img1.total() != img2.total())
    {
        return -1;
    }
    int numOfDiffPix = 0;
    for (int i = 0; i < img1.rows; i++)
    {
        for (int j = 0; j < img2.cols; j++)
        {
            if (img1.at<uchar>(i, j) != img2.at<uchar>(i, j))
            {
                numOfDiffPix++;
            }
        }
    }
    return numOfDiffPix;
}
