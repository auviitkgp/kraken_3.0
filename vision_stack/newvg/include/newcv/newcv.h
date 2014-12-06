#include <opencv2/opencv.hpp>
#include "iostream"

using namespace cv;
using namespace std;

class test
{	
public:
	Mat image;
	int x;
	int y;
	test(){
		y=6;
	}
	~test(){

	}
	void img(){
		x = 5;
		cout << x;
	}
};