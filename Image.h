#ifndef _IMAGE_H_INCLUDED_
#define _IMAGE_H_INCLUDED_

#include "cv.h"
#include "highgui.h"
#include "SlidingWindow.h"

using namespace cv;
using namespace std;

struct WeightedRect{
	float weight;
	Rect rect;
};

class Image{

private:
	
	bool checkOverlap(Rect boundingBox, Rect annotation, double overlap);

public:

	Mat image;
	Mat hog_features;
	vector<WeightedRect> detectedBoxes;
	vector<SlidingWindow> slidingWindows;
	float scale_factor;
	string path;
	Image(){}
	Image(Mat &image, string path, float scale_factor) {
		this->path = path;
		this->image = image;
		this->scale_factor = scale_factor;
	}

	void ereaseOverlappingBoxes();
};

#endif