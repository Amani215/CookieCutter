#pragma once
#include <opencv2/highgui.hpp>
#include <iostream>       // std::cout, std::endl
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "Painter.h"

using namespace cv;

class GameLauncher
{
private :
	Mat cookie;
	bool gettingReady = true;
	string countdown;

	void timerFunction();
	void getReady(Painter painter, ShapeDetector sd, VideoCapture cap);
	void play(Painter painter, ShapeDetector sd, VideoCapture cap);
public:
	GameLauncher(Mat cookie, int countdown);
	void launch(Painter painter);
};

