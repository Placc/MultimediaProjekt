#ifndef _SLIDINGWINDOW_H_INCLUDED_
#define _SLIDINGWINDOW_H_INCLUDED_

#pragma once

#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

class SlidingWindow {
public:
	Mat hog_features;
	Rect slidingWindow;

	SlidingWindow(Mat &hog_features, Rect &slidingWindow){
		this->hog_features = hog_features;
		this->slidingWindow = slidingWindow;
	}
};

#endif