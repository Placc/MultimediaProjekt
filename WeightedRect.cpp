#include "WeightedRect.h"

bool WeightedRect::checkOverlap(Rect &boundingBox, double overlap){
	return !(this->getOverlap(boundingBox) < overlap); 
}

double WeightedRect::getOverlap(Rect &boundingBox){
	double _intersection = (double)(boundingBox & this->rect).area();
	double _union = (double)(boundingBox.area() + this->rect.area() - _intersection);
	return (_intersection/_union);
}

