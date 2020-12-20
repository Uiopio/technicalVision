// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once
#include "opencv.hpp"
using namespace std;
using namespace cv;

class Fourier2D
{
public:

	Fourier2D() = default;

	~Fourier2D() = default;

	int setSource(string sourceName);
	int getSource(Mat& source) const;

	int setFourierImage(Mat imageRe);
	int getFourierImage(Mat& image) const;

	int forwardTransform();
	int fastInverseTransform();
	int fastForwardTransform();

	static void swapSpektr(Mat& magI);
	int createGaussian(float sigmaX, float sigmaY);
	int spectrum();
	int spectrum(Mat reAndIm, Mat spec) const;
	int laplas(Mat ReAndIm) const;
	int box(Mat ReAndIm) const;
	int sobel(Mat ReAndIm, int flag) const;

	static Mat expansion(Mat inputImage);
	int createMinusGaussian(float sigmaX, float sigmaY);
	Mat doCorrelation(Mat whereSearch, Mat whatSearch) const;
	void carNumber2(Mat& number, Mat& symbol);
	int inverseFromSpectrumAndPhase(Mat& result);
	int inverseTransform();

	int setImageSpectre(Mat spectre);
	int getImageSpectre(Mat& spectre) const;

	int setImagePhase(Mat phase);
	int getImagePhase(Mat& phase) const;

private:

	Mat m_source;
	Mat m_outputImage;
	Mat m_spectrum;
	Mat m_phase;
	Size m_inputSize;
};

	int doSuperTask(string JacketName, string ForestName, float Sigma, Mat& result);