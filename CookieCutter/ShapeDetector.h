#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>

using namespace cv;
using namespace std;

class ShapeDetector
{
private:
	Mat img;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	vector<int> shapeColor = { 0,0,0,0,0,0 };	//black

	void setContours();
	bool validShape = false;	//initial state: the shape drawn is not valid
public:
	ShapeDetector(Mat img);
	bool pointIsValid(Point point);
	bool shapeIsValid(Mat frame);
	bool checkShape();
};

