#include "closing_filter.h"

const int ClosingProcess::OBJ = 255;
const int ClosingProcess::BG = 0;
const int ClosingProcess::CORE[9] = {OBJ, OBJ, OBJ, 
									BG, BG, BG, 
									BG, BG, BG };
const int ClosingProcess::BORDER_WIDTH = 1;


int ClosingProcess::borderReflect(Mat& src, Mat& res)
{	
	res = Mat(src.rows + (BORDER_WIDTH * 2), src.cols + (BORDER_WIDTH * 2), CV_8UC1);

	for (int row = 0; row < src.rows; row++)
	{
		expandRow(row, src, res);
	}

	for (int shift = 0; shift < BORDER_WIDTH; shift++)
	{
		int filledRow = BORDER_WIDTH + shift;
		int cleanRow = BORDER_WIDTH - 1 - shift;
		copyRow(filledRow, cleanRow, res);
	}

	for (int shift = 0; shift < BORDER_WIDTH; shift++)
	{
		int filledRow = res.rows - BORDER_WIDTH - 1 - shift;
		int cleanRow = res.rows - BORDER_WIDTH + shift;
		copyRow(filledRow, cleanRow, res);
	}
	return 0;
}

/*
bool ClosingFilter::testBorderReflect(Mat& img)
{
	for (int row = 0; row < BORDER_WIDTH; row++)
	{
		for (int col = 0; col < img.cols; col++)
		{
			if (img.at<uchar>(row, col) != img.at<uchar>(BORDER_WIDTH, col))
			{
				return false;
			}
		}
	}
	for (int row = img.rows - BORDER_WIDTH + 1; row < img.rows; row++)
	{
		for (int col = 0; col < img.cols; col++)
		{
			if (img.at<uchar>(row, col) != img.at<uchar>(img.rows - BORDER_WIDTH, col))
			{
				return false;
			}
		}
	}
	return true;
}
*/

Mat ClosingProcess::cutBorder(Mat& img)
{
	Mat res = Mat(img.rows - (BORDER_WIDTH * 2), img.cols - (BORDER_WIDTH * 2), CV_8U);
	for (int row = 0; row < res.rows; row++)
	{
		for (int col = 0; col < res.cols; col++)
		{
			res.at<uchar>(row, col) = img.at<uchar>(row + BORDER_WIDTH, col + BORDER_WIDTH);
		}
	}
	return res;
}

int ClosingProcess::cutBorder(Mat& img, Mat& res)
{
	res = Mat(img.rows - (BORDER_WIDTH * 2), img.cols - (BORDER_WIDTH * 2), CV_8U);
	for (int row = 0; row < res.rows; row++)
	{
		for (int col = 0; col < res.cols; col++)
		{
			res.at<uchar>(row, col) = img.at<uchar>(row + BORDER_WIDTH, col + BORDER_WIDTH);
		}
	}
	return 0;
}

int ClosingProcess::testOcvFilter(Mat& src, Mat& dst)
{
	// My
	Mat temp;
	int startMyTime = clock();
	useClosingFilter(src, temp);
	int endMyTime = clock();
	int resMyTime = endMyTime - startMyTime;

	// OpenCV
	uint8_t data[9] = { 255, 255, 255, 0, 0, 0, 0, 0, 0 };
	Mat kernel(3, 3, CV_8UC1, data);
	Mat ocvTemp;
	int startOcvTime = clock();
	morphologyEx(src, dst, MORPH_CLOSE, kernel, Point(1, 1), 1, BORDER_REFLECT);
	int endOcvTime = clock();
	int resOcvTime = endOcvTime - startOcvTime;

	compareImg(temp, dst);

	int res = resOcvTime - resMyTime;
	cout << "My time " << resMyTime << endl;
	cout << "OpenCV time " << resOcvTime << endl;
	cout << "DIFFERENT of time " << res << endl;
	//cout << "If time is >0 than my program better else it is worse." << endl;
	return res;
}

void ClosingProcess::compareImg(Mat& firstImg, Mat& secondImg)
{
	if (firstImg.rows != secondImg.rows
		|| firstImg.cols != secondImg.cols)
	{
		cout << "ERROR of size image in method compareImg()" << endl;
	}

	int errorsCounter = 0;
	for (int row = 0; row < firstImg.rows; row++)
	{
		for (int col = 0; col < firstImg.cols; col++)
		{
			if (firstImg.at<uchar>(row, col) != secondImg.at<uchar>(row, col))
			{
				errorsCounter++;
			}
		}
	}
	cout << "Count of errors " << errorsCounter << endl;
}

int ClosingProcess::prDilate(Mat& src, Mat& res)
{
	res = Mat(src.rows - BORDER_WIDTH * 2, src.cols - BORDER_WIDTH * 2, CV_8UC1);

	for (int row = BORDER_WIDTH; row < src.rows - BORDER_WIDTH; row++)
	{
		for (int col = BORDER_WIDTH; col < src.cols - BORDER_WIDTH; col++)
		{
			int resRow = row - BORDER_WIDTH;
			int resCol = col - BORDER_WIDTH;
			int iCore = 0;
			bool flag = false;
			for (int i = row - 1; i <= row + 1; i++) 
			{
				for (int j = col - 1; j <= col + 1; j++)
				{
					if (CORE[iCore] == OBJ && src.at<uchar>(i, j) == OBJ)
					{
						res.at<uchar>(resRow, resCol) = OBJ;
						flag = true;
						break;
					}
					iCore++;
				}
				if (flag == true) break;
			}

			if (flag == false)
			{
				res.at<uchar>(resRow, resCol) = BG;
			}
		}
	}

	return 0;
}

int ClosingProcess::prErode(Mat& src, Mat& res)
{
	res = Mat(src.rows - BORDER_WIDTH * 2, src.cols - BORDER_WIDTH * 2, CV_8UC1);

	for (int row = BORDER_WIDTH; row < src.rows - BORDER_WIDTH; row++)
	{
		for (int col = BORDER_WIDTH; col < src.cols - BORDER_WIDTH; col++)
		{
			int resRow = row - BORDER_WIDTH;
			int resCol = col - BORDER_WIDTH;
			int iCore = 0;
			bool flag = false;
			for (int i = row - 1; i <= row + 1; i++)
			{
				for (int j = col - 1; j <= col + 1; j++)
				{
					if (CORE[iCore] == OBJ && src.at<uchar>(i, j) != OBJ)
					{
						res.at<uchar>(resRow, resCol) = BG;
						flag = true;
						break;
					}
					iCore++;
				}
				if (flag == true) break;
			}

			if (flag == false)
			{
				res.at<uchar>(resRow, resCol) = OBJ;
			}
		}
	}

	return 0;
}

int ClosingProcess::useClosingFilter(Mat& src, Mat& res)
{
	Mat temp;
	dilate(src, temp);
	erode(temp, res);
	return 0;
}

int ClosingProcess::dilate(Mat& src, Mat& res)
{
	Mat temp;
	src.copyTo(temp);
	borderReflect(src, temp);
	prDilate(temp, res);
	return 0;
}

int ClosingProcess::erode(Mat& src, Mat& res)
{
	Mat temp;
	src.copyTo(temp);
	borderReflect(src, temp);
	prErode(temp, res);
	return 0;
}

int ClosingProcess::copyRow(int filledRow, int cleanRow, Mat & img)
{
	for (int col = 0; col < img.cols; col++)
	{
		img.at<uchar>(cleanRow, col) = img.at<uchar>(filledRow, col);
	}
	return 0;
}

int ClosingProcess::expandRow(int iImgRowNum, Mat& iImg, Mat& oImg)
{
	int oImgRowNum = iImgRowNum + BORDER_WIDTH;

	for (int shift = 0; shift < BORDER_WIDTH; shift++)
	{
		int cleanCol = BORDER_WIDTH - 1 - shift;
		oImg.at<uchar>(oImgRowNum, cleanCol) = iImg.at<uchar>(iImgRowNum, shift);
	}

	for (int col = 0; col < iImg.cols; col++)
	{
		oImg.at<uchar>(oImgRowNum, col + BORDER_WIDTH) = iImg.at<uchar>(iImgRowNum, col);
	}

	for (int shift = 0; shift < BORDER_WIDTH; shift++)
	{
		int cleanCol = oImg.cols - 1 - shift;
		int filledCol = iImg.cols - BORDER_WIDTH + shift;
		oImg.at<uchar>(oImgRowNum, cleanCol) = iImg.at<uchar>(iImgRowNum, filledCol);
	}
	return 0;
}

void pr(Mat& img)
{
	std::cout << "TEST" << std::endl;
	imshow("img", img);
	waitKey(0);
}
