#ifndef _ClassifiedRect_H_INCLUDED_
#define _ClassifiedRect_H_INCLUDED_

#pragma once

#include "cv.h"
#include "highgui.h"
#include "highgui.h"

using namespace cv;
using namespace std;

class ClassifiedRect{
public:	
	double distanceFromHyperplane;
	Rect rect;
	bool groundTruth;
	
	ClassifiedRect(){
		distanceFromHyperplane = 0.0;
		rect = Rect(0,0,0,0);
		groundTruth=false;
	};

	ClassifiedRect(Rect &rect, double distanceFromHyperplane){
		this->rect = rect;
		this->distanceFromHyperplane = distanceFromHyperplane;
		groundTruth=false;
	};

	bool checkOverlap(Rect &boundingBox, double overlap);
	double getOverlap(Rect &boundingBox);
};

#endif