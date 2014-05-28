#ifndef _WEIGHTEDRECT_H_INCLUDED_
#define _WEIGHTEDRECT_H_INCLUDED_

#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

class WeightedRect{
public:	
	float weight;
	Rect rect;
	WeightedRect(Rect rect, float weight){
		this->rect = rect;
		this->weight = weight;
	}
	bool checkOverlap(Rect boundingBox, double overlap);
	double getOverlap(Rect boundingBox);
};

#endif