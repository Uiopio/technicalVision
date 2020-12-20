#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"

#define EXP 2.7182818284

using namespace cv;
using namespace std;

class current_image
{
public:
	current_image() = default;

	current_image(float sigma);

	~current_image() = default;

	int setImage(string imageName);

	Mat getImage();

	int processImage();

	int expandImage(Mat& image);
	int cropImage(Mat& image);

	int corePass(Mat& image);

private:
	Mat m_processingImage;
	Mat m_defaultImage;
	float m_sigma;
};
