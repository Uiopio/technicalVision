#include "iostream"
#include "openingFilter.h"
#include <chrono>

using namespace std;
using namespace cv;

void compareTime(Mat src, Mat kernel)
{
	//расчет времени для кастом фильтра
	auto begin = std::chrono::steady_clock::now();

	openingFilter custom;
	custom.openingCustom(src, kernel);

	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << "Custom filter time: " << elapsed_ms.count() << " ms\n" << endl;


	//расчет времени для openCV фильтра
	auto beginCV = std::chrono::steady_clock::now();

	openingFilter openCV;
	openCV.openingOpenCV(src, kernel);

	auto endCV = std::chrono::steady_clock::now();
	auto elapsed_msCV = std::chrono::duration_cast<std::chrono::milliseconds>(endCV - beginCV);
	std::cout << "OpenCV filter time: " << elapsed_msCV.count() << " ms\n" << endl;
}
void comapreSimilarity(Mat src1, Mat src2)  
{
	Mat result(src1.rows, src1.cols, src1.type());
	cv::absdiff(src1, src2, result);

	float res = 0;
	for (int i = 0; i < result.rows-1; i++)
		for (int j = 0; j < result.cols; j++)
		{
			if (result.at<uchar>(i, j) != 0) res++;
		}

	imshow("result", result);
	cout << "\nDifferent pixels: " << res << "\n" << endl;
}

int main() {

	//create core
	uint8_t kernelData[9] = { 255, 0, 255,
							 0, 255, 0,
						   255, 255, 255 };

	Mat kernel(3, 3, CV_8UC1, kernelData);

	//load "lena" image
	Mat lenaOriginal = imread("C:/Lena_test.jpg", IMREAD_GRAYSCALE);	  //считываем монохромно
	cv::threshold(lenaOriginal, lenaOriginal, 128, 255, THRESH_BINARY);	  //бинаризируем
	imshow("origin", lenaOriginal);

	openingFilter filter;
	//filter.setImage(lenaOriginal);
	//filter.addBoarder();
	filter.openingCustom(lenaOriginal, kernel);
	imshow("custom filter", filter.getImage());
	
	openingFilter filterCV;
	filterCV.openingOpenCV(lenaOriginal, kernel);
	imshow("openCV filter", filterCV.getImage());
	
	comapreSimilarity(filter.getImage(), filterCV.getImage());
	compareTime(lenaOriginal, kernel);
	
	while (waitKey(100) != 27);
	
	return 0;
}

