#include "Classification.h"

using namespace std;
using namespace cv;

namespace Classification {
	void classify(string modelFile, string testDir){
		vector<string> pos_examples;
		vector<string> neg_examples;
		Preprocessing::loadPathsByDirectory(TEST_POS_ORIGINAL, pos_examples);
		Preprocessing::loadPathsByDirectory(TEST_NEG_ORIGINAL, neg_examples);

		for(vector<string>::iterator posIt = pos_examples.begin(); posIt != pos_examples.end(); ++posIt)
		{
			OriginalImage img( *posIt );
			FeatureExtraction::computeHOGPyramid( img );
			FeatureExtraction::computeSlidingWindows( img );

			vector<Image>::iterator lower_img_it;
			for( lower_img_it = ( img ).lower_images.begin();  lower_img_it != ( img ).lower_images.end(); ++lower_img_it )
			{
				FeatureExtraction::computeSlidingWindows( *lower_img_it );
			}

			//TODO: Normally, we should iterate and scale image as we do in training... Doesn't make sense?!
		}

		for(vector<string>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt)
		{
			OriginalImage img( *negIt );
			FeatureExtraction::computeHOGPyramid( img );
			FeatureExtraction::computeSlidingWindows( img );

			vector<Image>::iterator lower_img_it;
			for( lower_img_it = ( img ).lower_images.begin();  lower_img_it != ( img ).lower_images.end(); ++lower_img_it )
			{
				FeatureExtraction::computeSlidingWindows( *lower_img_it );
			}


			//TODO: Normally, we should iterate and scale image as we do in training... Doesn't make sense?!
		}
	}
}