#pragma once

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "ShapeDetector.h"

using namespace cv;
using namespace std;

class Painter
{
private:
	vector<int> colorToDetect;
	vector<vector<int>> newPoints;
	bool dead = false;	//initial state: the player is not dead
public:
	Point getTipOfPen(Mat imgDilate);
	void detectColor(Mat img);
	void drawOnCanvas(Mat img, ShapeDetector sd, bool gettingReady);
	void setColorToDetect(vector<int> colorToDetect);
	void refresh();
	bool getDead();
};