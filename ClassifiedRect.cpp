#include "ClassifiedRect.h"


/**
*	checks overlap of the classifiedrect with a bounding box
*	@param cv::Rect boundingBox : box that needs to be checked
*	@param double overlap: overlap of the given rect
*	@return
*/
bool ClassifiedRect::checkOverlap(Rect &boundingBox, double overlap){
	return !(this->getOverlap(boundingBox) < overlap); 
}

/**
*	gets the overlap of a rect
*	@param cv::Rect boundingBox
*	@return
*/
double ClassifiedRect::getOverlap(Rect &boundingBox){
	double _intersection = (double)(boundingBox & this->rect).area();
	double _union = (double)(boundingBox.area() + this->rect.area() - _intersection);
	return (_intersection/_union);
}

