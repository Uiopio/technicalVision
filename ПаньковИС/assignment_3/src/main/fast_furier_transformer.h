#pragma once

#define _USE_MATH_DEFINES

#include "filter.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <stdint.h>
#include <math.h>

enum ComplexPart
{
    RE = 0,
    IM = 1,
    NUM_OF_COMPLEX_PARTS = 2
};

void shiftSpectrum(cv::Mat1f& spectrum, const int32_t shiftX, const int32_t shiftY);
void multiplySpectrums(const cv::Mat2f& first, const cv::Mat2f& second, cv::Mat2f& result, bool isCorr = false);

class FastFurierTransformer
{
public:
    FastFurierTransformer() = default;
    ~FastFurierTransformer() = default;

    void setImage(const cv::Mat1f& image);
    void setImage(const cv::Mat& image);
    cv::Mat1f getImage() const;
    void showImage(cv::String imageName = "Image");

    void setPattern(const cv::Mat1f& pattern);
    void setPattern(const cv::Mat& pattern);
    cv::Mat1f getPattern() const;
    void showPattern(cv::String patternName = "Pattern");

    void setSpectrum(const cv::Mat2f& spectrum);
    void setSpectrum(const cv::Mat& spectrum);
    cv::Mat2f getSpectrum() const;
    cv::Mat1f getSpectrumMagnitude();
    void showSpectrum(cv::String spectrumName = "Spectrum");

    void setSpectrumSize(cv::Size2i spectrumSize);
    cv::Size2i getSpectrumSize() const;

    void setImageShift(cv::Point2i imageShift);
    cv::Point2i getImageShift() const;

    void setFilter(Filter& filter);
    Filter& getFilter();

    void directFastFurierTransform();
    void inverseFastFurierTransform();
    void filtrateImage();
    void normalizedCrossCorrelation();

private:
    void normalizeSpectrum(cv::Mat1f& spectrumMagnitude, const float min, const float max);
    void shiftSpectrum(cv::Mat2f& spectrum, const int32_t shiftX, const int32_t shiftY);

    void filtrate(cv::Mat1f& image, cv::Mat2f& spectrum);
    cv::Mat1f getMagnitude(cv::Mat2f& spectrum);

    void directFFT(const cv::Mat1f& image, cv::Mat2f& spectrum);
    void inverseFFT(const cv::Mat2f& spectrum, cv::Mat1f& image);
    void fft(cv::Vec2f *ptr, const int32_t size, const bool isInvert);

    cv::Mat1f m_image;
    cv::Point2i m_imageShift;
    cv::Mat2f m_spectrum;
    cv::Size2i m_spectrumSize;
    cv::Mat1f m_pattern;
    Filter m_filter;
};
