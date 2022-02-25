#pragma once

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class ColorPicker
{
private:
	Scalar color;
	int hueMin = 0, satMin = 0, valMin = 0;
	int hueMax = 179, satMax = 255, valMax = 255;
public:
	void setColor();
	int getHueMin();
	int getHueMax();
	int getSatMin();
	int getSatMax();
	int getValMin();
	int getValMax();
};