#include "Image.h"

void Image::addDetectedBox(WeightedRect rect){
	bool overlapping = false;
	for(int i = 0; i < detectedBoxes.size(); i++){
		if(rect.checkOverlap(detectedBoxes[i].rect, 0.2)){
			overlapping = true;
			if(detectedBoxes[i].weight < rect.weight){
				detectedBoxes.erase(detectedBoxes.begin() + i);
				detectedBoxes.push_back(rect);
				return;
			}
		}
	}
	if(!overlapping)
		detectedBoxes.push_back(rect);
}