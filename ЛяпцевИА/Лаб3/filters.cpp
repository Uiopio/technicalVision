// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Fourier.h"
using namespace std;

int Fourier2D::laplas(Mat ReAndIm) const
{
	Mat laplas(ReAndIm.size(), CV_32FC1, Scalar(0));
	laplas.at<float>(0, 1) = 1;
	laplas.at<float>(1, 0) = 1;
	laplas.at<float>(2, 1) = 1;
	laplas.at<float>(1, 2) = 1;
	laplas.at<float>(1, 1) = -4;

	Mat laplasFur(ReAndIm.size(), CV_32FC2, Scalar(0, 0));

	//forwardTransform(laplas, laplasFur);
	dft(laplas, laplasFur, DFT_COMPLEX_OUTPUT);

	Mat result(ReAndIm.size(), CV_32FC2, Scalar(0, 0));
	for (int x = 0; x < ReAndIm.rows; x++)
	{
		for (int y = 0; y < ReAndIm.cols; y++)
		{
			float a1 = ReAndIm.at<Vec2f>(x, y)[0];
			float b1 = ReAndIm.at<Vec2f>(x, y)[1];
			float a2 = laplasFur.at<Vec2f>(x, y)[0];
			float b2 = laplasFur.at<Vec2f>(x, y)[1];

			result.at<Vec2f>(x, y)[0] = a1 * a2 - b1 * b2;
			result.at<Vec2f>(x, y)[1] = a1 * b2 + a2 * b1;
		}
	}

	Mat spec(ReAndIm.size(), CV_32FC1, Scalar(0));
	spectrum(result, spec);

	Mat rezultInverse(ReAndIm.size(), CV_32FC1, Scalar(0));
	//inverseTransform(result, rezultInverse);
	dft(result, rezultInverse, DFT_INVERSE | DFT_REAL_OUTPUT);

	normalize(rezultInverse, rezultInverse, 0, 1, NORM_MINMAX);

	Mat temp;
	rezultInverse.convertTo(temp, CV_8UC1, 255);

	cout << "Laplas" << endl;
	imshow("Lap", temp);

	while (waitKey(25) != 27)
	{
	}
	return 0;
}

int Fourier2D::box(Mat ReAndIm) const
{
	Mat box(ReAndIm.size(), CV_32FC1, Scalar(0));
	box.at<float>(0, 0) = 1;
	box.at<float>(0, 1) = 1;
	box.at<float>(0, 2) = 1;
	box.at<float>(1, 0) = 1;
	box.at<float>(1, 1) = 1;
	box.at<float>(1, 2) = 1;
	box.at<float>(2, 0) = 1;
	box.at<float>(2, 1) = 1;
	box.at<float>(2, 2) = 1;

	Mat boxFur(ReAndIm.size(), CV_32FC2, Scalar(0, 0));

	//forwardTransform(laplas, laplasFur);
	dft(box, boxFur, DFT_COMPLEX_OUTPUT);

	Mat result(ReAndIm.size(), CV_32FC2, Scalar(0, 0));
	for (int x = 0; x < ReAndIm.rows; x++)
	{
		for (int y = 0; y < ReAndIm.cols; y++)
		{
			float a1 = ReAndIm.at<Vec2f>(x, y)[0];
			float b1 = ReAndIm.at<Vec2f>(x, y)[1];
			float a2 = boxFur.at<Vec2f>(x, y)[0];
			float b2 = boxFur.at<Vec2f>(x, y)[1];

			result.at<Vec2f>(x, y)[0] = a1 * a2 - b1 * b2;
			result.at<Vec2f>(x, y)[1] = a1 * b2 + a2 * b1;
		}
	}

	Mat spec(ReAndIm.size(), CV_32FC1, Scalar(0));
	spectrum(result, spec);

	Mat rezultInverse(ReAndIm.size(), CV_32FC1, Scalar(0));
	//inverseTransform(result, rezultInverse);
	dft(result, rezultInverse, DFT_INVERSE | DFT_REAL_OUTPUT);

	normalize(rezultInverse, rezultInverse, 0, 1, NORM_MINMAX);

	Mat temp;
	rezultInverse.convertTo(temp, CV_8UC1, 255);

	cout << "box" << endl;
	imshow("box", temp);

	while (waitKey(25) != 27)
	{
	}
	return 0;
}

int Fourier2D::sobel(Mat ReAndIm, int flag) const
{
	Mat box(ReAndIm.size(), CV_32FC1, Scalar(0));

	if (flag == 0)
	{
		box.at<float>(0, 0) = 1;
		box.at<float>(0, 1) = 2;
		box.at<float>(0, 2) = 1;
		box.at<float>(1, 0) = 0;
		box.at<float>(1, 1) = 0;
		box.at<float>(1, 2) = 0;
		box.at<float>(2, 0) = -1;
		box.at<float>(2, 1) = -2;
		box.at<float>(2, 2) = -1;
	}

	if (flag == 1)
	{
		box.at<float>(0, 0) = 1;
		box.at<float>(0, 1) = 0;
		box.at<float>(0, 2) = -1;
		box.at<float>(1, 0) = 2;
		box.at<float>(1, 1) = 0;
		box.at<float>(1, 2) = -2;
		box.at<float>(2, 0) = 1;
		box.at<float>(2, 1) = 0;
		box.at<float>(2, 2) = -1;
	}

	Mat boxFur(ReAndIm.size(), CV_32FC2, Scalar(0, 0));

	//forwardTransform(laplas, laplasFur);
	dft(box, boxFur, DFT_COMPLEX_OUTPUT);

	Mat result(ReAndIm.size(), CV_32FC2, Scalar(0, 0));
	for (int x = 0; x < ReAndIm.rows; x++)
	{
		for (int y = 0; y < ReAndIm.cols; y++)
		{
			float a1 = ReAndIm.at<Vec2f>(x, y)[0];
			float b1 = ReAndIm.at<Vec2f>(x, y)[1];
			float a2 = boxFur.at<Vec2f>(x, y)[0];
			float b2 = boxFur.at<Vec2f>(x, y)[1];

			result.at<Vec2f>(x, y)[0] = a1 * a2 - b1 * b2;
			result.at<Vec2f>(x, y)[1] = a1 * b2 + a2 * b1;
		}
	}

	Mat spec(ReAndIm.size(), CV_32FC1, Scalar(0));
	spectrum(result, spec);

	Mat resultInverse(ReAndIm.size(), CV_32FC1, Scalar(0));
	//inverseTransform(result, rezultInverse);
	dft(result, resultInverse, DFT_INVERSE | DFT_REAL_OUTPUT);

	normalize(resultInverse, resultInverse, 0, 1, NORM_MINMAX);

	Mat temp;
	resultInverse.convertTo(temp, CV_8UC1, 255);

	cout << "sobel" << endl;
	string sobelname = "sobel" + to_string(flag);
	imshow(sobelname, temp);

	while (waitKey(25) != 27)
	{
	}
	return 0;
}