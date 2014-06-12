#include "Training.h"

using namespace cv;
using namespace std;

namespace Training {
	
	void trainSVM(string modelFile){
		vector<string> pos_examples;
		vector<string> neg_examples;
		Classifier classifier = Classifier();
		RNG random_index = RNG( time (NULL) );

		Rect roi(Point(16,16), Point(80, 144));
		Preprocessing::loadPathsByDirectory(TRAIN_POS_NORMALIZED, pos_examples);
		Preprocessing::loadPathsByDirectory(TRAIN_NEG_ORIGINAL, neg_examples);
		for(vector<string>::iterator posIt = pos_examples.begin(); posIt != pos_examples.end(); ++posIt)
		{
			OriginalImage img( *posIt );
			( img ).image = ( img ).image(roi);
			FeatureExtraction::extractHOGFeatures( img );
			classifier.addPositiveExample(img.hog_features);
		}
		for(vector<string>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt)
		{
			
			//TODO: Randomly select some hypothesis of all, s.t. not everything has to be computed...

			OriginalImage img( *negIt );
			FeatureExtraction::computeHOGPyramid( img );
			FeatureExtraction::computeSlidingWindows( img );

			for(std::vector<Image>::iterator scaleIt = ( img ).lower_images.begin(); scaleIt != ( img ).lower_images.end(); ++scaleIt)
			{
				FeatureExtraction::computeSlidingWindows(*scaleIt);
				
			}

			int count = 0;
			double howMuch = ((double)(MAXRANDOMELEMENTS)/(double)(img.lower_images.size() + 1));
			for(int i = 0; i < (int)(howMuch); i++){
				classifier.addNegativeExample( img.slidingWindows[ random_index.uniform( 0, img.slidingWindows.size() ) ].hog_features );
				count++;
			}
			howMuch = howMuch + ((double)(MAXRANDOMELEMENTS)/(double)(img.lower_images.size() + 1)) - (int)howMuch;
			if(howMuch < MAXRANDOMELEMENTS)
				for(int current = 0; current < img.lower_images.size(); current++){
					for(int i = 0; i < (int)(howMuch); i++){
						int y = random_index.uniform( 0, img.lower_images[ current ].slidingWindows.size() );
						Mat hog_features = img.lower_images[ current ].slidingWindows[ y ].hog_features;
						classifier.addNegativeExample( hog_features );
						count++;
					}
					howMuch = howMuch + ((double)(MAXRANDOMELEMENTS)/(double)(img.lower_images.size() + 1)) - (int)howMuch;
				}
				while(count < MAXRANDOMELEMENTS){
					int x = random_index.uniform( 0, img.lower_images.size() );
					int y = random_index.uniform( 0, img.lower_images[ x ].slidingWindows.size() );
					Mat hog_features = img.lower_images[ x ].slidingWindows[ y ].hog_features;
					classifier.addNegativeExample( hog_features );
					count++;
				}
				
		}

		//Sliding Window HOG-Features are in OriginalImage.slidingWindows and OriginalImage.lower_images[x].slidingWindows
	}
}