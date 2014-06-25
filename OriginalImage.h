#ifndef _ORIGINALIMAGE_H_INCLUDED_
#define _ORIGINALIMAGE_H_INCLUDED_

#pragma once

#include "cv.h"
#include "highgui.h"
#include "ClassifiedRect.h"
#include "Image.h"

using namespace cv;
using namespace std;

class OriginalImage : public Image{

public:
	vector<Image> lower_images;
	OriginalImage(string path) : Image(imread(path, CV_LOAD_IMAGE_COLOR), path, 1.0) {}
	vector<ClassifiedRect> detectedBoxes;
	void addDetectedBox(ClassifiedRect &rect);
};

#endif