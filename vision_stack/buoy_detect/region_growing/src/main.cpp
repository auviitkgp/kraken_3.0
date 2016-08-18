#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <sstream>
#include <list>
#include "Color.cpp"
#include <chrono>
#include "grow.h"
//#include "chrono_io"

using namespace std;
using namespace cv;
using namespace std::chrono;

int main(int argc, char const **argv)
{
	Mat src, denoise;

	src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	fastNlMeansDenoisingColored(src, denoise, 2);
	medianBlur(denoise, denoise, 3);

	//denoise.copyTo(org);
	Mat edgeMap(denoise.rows, denoise.cols, CV_8UC3, Scalar(0, 0, 0));
	Mat filled(denoise.rows, denoise.cols, CV_8UC3, Scalar(0, 0, 0));

	grow M;
	M.setThresholds(15, 15);

	typedef std::chrono::high_resolution_clock clk;

	auto t1 = clk::now();

	//seed = denoise.at<Vec3b>(290, 290);
	M.start_grow(denoise, filled, edgeMap, 256, 130, 1);

	auto t2 = clk::now();

	//seed = denoise.at<Vec3b>(256, 294);
	M.start_grow(denoise, filled, edgeMap, 263, 276, 3);

	auto t3 = clk::now();

	//seed = denoise.at<Vec3b>(303, 515);

	//grow(denoise, edgeMap, 303, 515, 20, 3);

	//auto t4 = clk::now();

	cout << "First seed : " << duration_cast<microseconds>(t2 - t1).count() << ", Second seed : " << duration_cast<microseconds>(t3 - t2).count() << "\n";

	imshow("Original Image", denoise);
	imshow("filled Image", filled);
	imshow("EdgeMap Image", edgeMap);
	waitKey(0);
	destroyAllWindows();

	return 0;
}