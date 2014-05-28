#ifndef _IMAGE_H_INCLUDED_
#define _IMAGE_H_INCLUDED_

#include "cv.h"
#include "highgui.h"
#include "WeightedRect.h"
#include "SlidingWindow.h"

using namespace cv;
using namespace std;

class Image{

public:
	Mat image;
	Mat hog_features;
	vector<SlidingWindow> slidingWindows;
	float scale_factor;
	string path;

	Image(Mat &image, string path, float scale_factor) {
		this->path = path;
		this->image = image;
		this->scale_factor = scale_factor;
	}
};

#endif