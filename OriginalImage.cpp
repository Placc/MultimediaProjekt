#include "OriginalImage.h"

void OriginalImage::addDetectedBox(ClassifiedRect &rect){
	bool overlapping = false;
	for(int i = 0; i < detectedBoxes.size(); i++){
		if(rect.checkOverlap(detectedBoxes[i].rect, 0.2)){
			overlapping = true;
			//unnötiges If, da schon nach distanceFromHyperplane vorsortiert
			if(detectedBoxes[i].distanceFromHyperplane < rect.distanceFromHyperplane){
				detectedBoxes.erase(detectedBoxes.begin() + i);
				detectedBoxes.push_back(rect);
				return;
			}
		}
	}
	if(!overlapping)
		detectedBoxes.push_back(rect);
}