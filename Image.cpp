#include "Image.h"

bool Image::checkOverlap(Rect boundingBox, Rect annotation, double overlap){
	double _intersection = (double)(boundingBox & annotation).area();
	double _union = (double)(boundingBox.area() + annotation.area() - _intersection);
	return !(_intersection/_union < overlap); 
}

void Image::ereaseOverlappingBoxes(){
	for(int i = 0; i < detectedBoxes.size(); i++)
		for(int j = i+1; j < detectedBoxes.size(); j++)
			if(checkOverlap(detectedBoxes[i].rect, detectedBoxes[j].rect, 0.2)){
				if(detectedBoxes[i].weight >= detectedBoxes[j].weight)
					detectedBoxes.erase(detectedBoxes.begin() + j);
				else{
					detectedBoxes.erase(detectedBoxes.begin() + i);
					i--;
				}
				j--;
			}
}