#include "Training.h"

using namespace cv;
using namespace std;

namespace Training {
	
	void trainSVM(string modelFile){
		vector<OriginalImage> pos_examples;
		vector<OriginalImage> neg_examples;
		Rect roi(Point(16,16), Point(80, 144));
		Preprocessing::loadImagesByDirectory(TRAIN_POS_NORMALIZED, pos_examples);
		Preprocessing::loadImagesByDirectory(TRAIN_NEG_ORIGINAL, neg_examples);
		for(vector<OriginalImage>::iterator posIt = pos_examples.begin(); posIt != pos_examples.end(); ++posIt){
			(*posIt).image = (*posIt).image(roi);
			FeatureExtraction::computeHOGPyramid(*posIt);
			FeatureExtraction::computeSlidingWindows(*posIt);
			for(std::vector<Image>::iterator scaleIt = (*posIt).lower_images.begin(); scaleIt != (*posIt).lower_images.end(); ++scaleIt){
				FeatureExtraction::computeSlidingWindows(*scaleIt);
			}
		}
		for(vector<OriginalImage>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt){
			
			//TODO: Randomly select some hypothesis of all, s.t. not everything has to be computed...

			FeatureExtraction::computeHOGPyramid(*negIt);
			FeatureExtraction::computeSlidingWindows(*negIt);
			for(std::vector<Image>::iterator scaleIt = (*negIt).lower_images.begin(); scaleIt != (*negIt).lower_images.end(); ++scaleIt){
				FeatureExtraction::computeSlidingWindows(*scaleIt);
			}
		}

		//Sliding Window HOG-Features are in OriginalImage.slidingWindows and OriginalImage.lower_images[x].slidingWindows
	}
}