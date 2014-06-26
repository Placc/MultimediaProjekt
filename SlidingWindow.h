#ifndef _SLIDINGWINDOW_H_INCLUDED_
#define _SLIDINGWINDOW_H_INCLUDED_

#pragma once

#include "cv.h"
#include "highgui.h"
#include "ClassifiedRect.h"

using namespace cv;
using namespace std;

class SlidingWindow {
public:
	Mat hog_features;
	ClassifiedRect slidingWindow;
	float scaleFactor;

	SlidingWindow(Mat &hog_features, Rect &slidingWindow, float scaleFactor){
		this->hog_features = hog_features;
		this->slidingWindow.rect = slidingWindow;
		this->scaleFactor = scaleFactor;
	}
};

#endif