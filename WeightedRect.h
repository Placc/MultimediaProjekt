#ifndef _WEIGHTEDRECT_H_INCLUDED_
#define _WEIGHTEDRECT_H_INCLUDED_

#pragma once

#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

class WeightedRect{
public:	
	double weight;
	Rect rect;
	
	WeightedRect(){
		weight = 0.0;
		rect = Rect(0,0,0,0);
	};

	WeightedRect(Rect &rect, double weight){
		this->rect = rect;
		this->weight = weight;
	};

	bool checkOverlap(Rect &boundingBox, double overlap);
	double getOverlap(Rect &boundingBox);
};

#endif