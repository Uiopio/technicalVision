//in the process		
//ready					-	need to check performance
//correctly

#define WAIT while (waitKey(100) != 27) {;}

#include "Otsu_method.h"

//correctly
//��������� ������������ �����������
static void DrawBrightnessHistogram(int total, int* value)
{
	int newValue[256];
	int Max = 0;
	//������ ������ ������� ������� �� ������������
	for (int i = 0; i < 256; i++)
	{
		newValue[i] = (int)((float)(value[i]) / (float)(total) * 512.0);
		if (Max < newValue[i])
		{
			Max = newValue[i];
		}
	}
	float factor = 1;
	while ((float)(Max) * factor < 600.0)
	{
		factor++;
	}
	Scalar white(255, 255, 255);
	Mat image((int)(factor * (float)(Max)) + 1, 512, CV_8UC3, white);
	//���������� ����������� ������������
	int thicknessLine = (int)(512.0 / 256.0);
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < factor * newValue[i]; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				for (int thickness = 0; thickness < thicknessLine; thickness++)
				{
					image.at<Vec3b>(image.rows - 1 - j, i * thicknessLine + thickness)[k] = 0;
				}
			}
		}
	}
	imshow("BrightnessHistogram", image);
	return;
}

//correctly
//���������� ������������ ����������� //�������������
static void BrightnessHistogram(const Mat & image, int* value)
{
	for (int i = 0; i < 256; i++)
	{
		value[i] = 0;
	}
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			//[0] - B; [1] - G; [2] - R
			int brightness = (int)(0.3 * (float)(image.at<Vec3b>(i, j)[2]) + 0.59 * (float)(image.at<Vec3b>(i, j)[1]) + 0.11 * (float)(image.at<Vec3b>(i, j)[0]));
			value[brightness]++;
		}
	}
	DrawBrightnessHistogram(image.rows * image.cols, value);
	return;
}

//correctly
//����� ���������� ��������� C1
static float CalculateAccumulationAmountP(float* probability, int threshold)
{
	float sum = 0;
	for (int i = 0; i <= threshold; i++)
	{
		sum += probability[i];
	}
	return sum;
}

//correctly
//����� ���������� ��������� C1
static float CalculateAccumulationAmountM(float* probability, int threshold)
{
	float sum = 0;
	for (int i = 0; i <= threshold; i++)
	{
		sum += i * probability[i];
	}
	return sum;
}

//correctly
//�������������� ����������� //������������
static int image�onversion(const Mat& src, Mat& dst, int threshold)
{
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			int brightness = (int)(0.3 * (float)(src.at<Vec3b>(i, j)[2]) + 0.59 * (float)(src.at<Vec3b>(i, j)[1]) + 0.11 * (float)(src.at<Vec3b>(i, j)[0]));
			if (brightness < threshold - 10)
			{
				dst.at<Vec3b>(i, j)[0] = 0;
				dst.at<Vec3b>(i, j)[1] = 0;
				dst.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	return 0;
}

//correctly
//�������� ������� ��������� //������������
int OtsuMethod(const Mat& src, Mat& dst)
{
	if (src.empty())
	{
		return -1;
	}
	if (dst.empty())
	{
		return -2;
	}
	int histogramm[256];
	BrightnessHistogram(src, histogramm);
	float probability[256];
	int totalPixel = src.cols * src.rows;
	//����������� ����������� ��������� ������ �������
	for (int i = 0; i < 256; i++)
	{
		probability[i] = (float)(histogramm[i]) / (float)(totalPixel);
	}
	//����������� ��������� ������� �� ������ k ������������
	float accumulationAmountP[256];
	//����������� ����� �� ������ ������� k
	float accumulationAmountM[256];
	for (int k = 0; k < 256; k++)
	{
		accumulationAmountP[k] = CalculateAccumulationAmountP(probability, k);
		accumulationAmountM[k] = CalculateAccumulationAmountM(probability, k);
	}
	float accumulationAmountMG = accumulationAmountM[255];
	//������������ ��������� ��� ������� k
	float interclassDispersion[256];
	//�������� ������������ ���������
	float maxInterclassDispersion = 0;
	for (int k = 0; k < 256; k++)
	{
		if (accumulationAmountP[k] != 0)
		{
			interclassDispersion[k] = (((float)accumulationAmountMG * (float)accumulationAmountP[k] - (float)accumulationAmountM[k]) *
				((float)accumulationAmountMG * (float)accumulationAmountP[k] - (float)accumulationAmountM[k])) /
				((float)accumulationAmountP[k] * (1.0 - (float)accumulationAmountP[k]));
		}
		else
		{
			interclassDispersion[k] = 0;
		}
		if (maxInterclassDispersion <= interclassDispersion[k])
		{
			maxInterclassDispersion = interclassDispersion[k];
		}
	}
	//���������� ���������� ������������ ��������� � ������� ������� ��������
	int finalThreshold = 0;
	{
		int tempSum = 0;
		int tempAmount = 0;
		for (int i = 0; i < 256; i++)
		{
			if ((int)(maxInterclassDispersion) <= (int)(interclassDispersion[i]))
			{
				tempSum += i;
				tempAmount++;
			}
		}
		finalThreshold = tempSum / tempAmount;
	}
	image�onversion(src, dst, finalThreshold);
	imshow("Before", src);
	imshow("After", dst);
	WAIT;
	return 0;
}

//correctly
//�������� ������� ��������� �����������
int OtsuMethod(const Mat& image)
{
	Scalar white(255, 255, 255);
	Mat newImage(image.rows, image.cols, CV_8UC3, white);
	return OtsuMethod(image, newImage);
}

//correctly
//���������� ������������ ����������� //��������������
static void com_histogramm(const Mat& image, int* value)
{
	for (int i = 0; i < 256; i++)
	{
		value[i] = 0;
	}
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			value[image.at<uchar>(i, j)]++;
		}
	}
	return;
}

//correctly
//�������������� ����������� //�������������
static void com_image�onversion(const Mat& src, Mat& dst, int threshold)
{
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<uchar>(i, j) < threshold)
			{
				dst.at<uchar>(i, j) = 0;
			}
			else
			{
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
	return;
}

//correctly
//����� ������� ����������� �������
static void searchTresholdStanFun(const Mat& image, const Mat& compared)
{
	Mat dst(image.rows, image.cols, CV_8UC1, Scalar(255));
	cout << "Threshold standart function: ";
	int delta[256];
	for (int i = 0; i < 256; i++)
	{
		delta[i] = 0;
		com_image�onversion(image, dst, i);
		for (int j = 0; j < image.rows; j++)
		{
			for (int k = 0; k < image.cols; k++)
			{
				if (compared.at<uchar>(j, k) != dst.at<uchar>(j, k))
				{
					delta[i]++;
				}
			}
		}
		if (delta[i] == 0)
		{
			cout << i << "; ";
		}
	}
	cout << endl;
	return;
}

//correctly
//��������� �� ����������� ��������
int Comparision(const Mat& originImage)
{
	if (originImage.empty())
	{
		return -1;
	}
	vector<Mat> v3;
	split(originImage, v3);
	//merge //���������� � ������������

	for (int i = 0; i < 3; i++)
	{
		//�������� ������ ����������� �� ������ �� ����� �����������
		Mat image = v3[i];
		int histogramm[256];
		com_histogramm(image, histogramm);
		float probability[256];
		int totalPixel = image.cols * image.rows;
		//����������� ����������� ��������� ������ �������
		for (int i = 0; i < 256; i++)
		{
			probability[i] = (float)(histogramm[i]) / (float)(totalPixel);
		}
		//����������� ��������� ������� �� ������ k ������������
		float accumulationAmountP[256];
		//����������� ����� �� ������ ������� k
		float accumulationAmountM[256];
		for (int k = 0; k < 256; k++)
		{
			accumulationAmountP[k] = CalculateAccumulationAmountP(probability, k);
			accumulationAmountM[k] = CalculateAccumulationAmountM(probability, k);
		}
		float accumulationAmountMG = accumulationAmountM[255];
		//������������ ��������� ��� ������� k
		float interclassDispersion[256];
		//�������� ������������ ���������
		float maxInterclassDispersion = 0;
		int treshold = 0;
		for (int k = 0; k < 256; k++)
		{
			if (accumulationAmountP[k] != 0)
			{
				interclassDispersion[k] = (((float)accumulationAmountMG * (float)accumulationAmountP[k] - (float)accumulationAmountM[k]) *
					((float)accumulationAmountMG * (float)accumulationAmountP[k] - (float)accumulationAmountM[k])) /
					((float)accumulationAmountP[k] * (1.0 - (float)accumulationAmountP[k]));
			}
			else
			{
				interclassDispersion[k] = 0;
			}
			if (maxInterclassDispersion <= interclassDispersion[k])
			{
				maxInterclassDispersion = interclassDispersion[k];
				treshold = k;
			}
		}
		//���������� ���������� ������������ ��������� � ������� ������� ��������
		int finalThreshold = treshold;
		{
			int tempSum = 0;
			int tempAmount = 0;
			for (int i = 0; i < 256; i++)
			{
				if ((maxInterclassDispersion) <= (interclassDispersion[i]))
				{
					tempSum += i;
					tempAmount++;
				}
			}
			finalThreshold = (int)((float)tempSum / (float)tempAmount);
		}
		Mat dst(image.rows, image.cols, CV_8UC1, Scalar(255));
		com_image�onversion(image, dst, finalThreshold);
		cout << "Threshold my function: " << finalThreshold << endl;
		threshold(image, dst, 0, 255.0, THRESH_BINARY | THRESH_OTSU); //�������� ������ � ������������� �������������
		searchTresholdStanFun(image, dst);
		imshow("Before", image);
		imshow("After", dst);
		imshow("Standart", dst);
		WAIT;
	}
	return 0;
}
