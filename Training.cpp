#include "Training.h"

using namespace cv;
using namespace std;

namespace Training {
	
	/**
	*	trains an svm model using the InriaPerson training dataset
	*	choses a number of negative detections randomly chosen from each image
	*	saves the freshly trained model to the harddisk
	*	@param string modelFile: path to the model file
	*/
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

			OriginalImage img( *negIt );
			FeatureExtraction::computeHOGPyramid( img );
			FeatureExtraction::computeSlidingWindows( img );

			for(std::vector<Image>::iterator scaleIt = ( img ).lower_images.begin(); scaleIt != ( img ).lower_images.end(); ++scaleIt)
			{
				FeatureExtraction::computeSlidingWindows(*scaleIt);
				
			}

			int count = 0;
			double howMuch = ( (double)( MAXRANDOMELEMENTS ) / (double)( img.lower_images.size() + 1 ) );
			for( int i = 0; i < (int)( howMuch ); i++ )
			{
				classifier.addNegativeExample( img.slidingWindows[ random_index.uniform( 0, img.slidingWindows.size() ) ].hog_features );
				count++;
			}
			howMuch = howMuch + ( (double)( MAXRANDOMELEMENTS ) / (double)( img.lower_images.size() + 1) ) - (int)howMuch;
			if( howMuch < MAXRANDOMELEMENTS )
				for( int current = 0; current < img.lower_images.size(); current++ )
				{
					for( int i = 0; i < (int)( howMuch ); i++ )
					{
						int y = random_index.uniform( 0, img.lower_images[ current ].slidingWindows.size() );
						Mat hog_features = img.lower_images[ current ].slidingWindows[ y ].hog_features;
						classifier.addNegativeExample( hog_features );
						count++;
					}
					howMuch = howMuch + ( (double)( MAXRANDOMELEMENTS ) / (double)( img.lower_images.size() + 1) ) - (int)howMuch;
				}
				while( count < MAXRANDOMELEMENTS )
				{
					int x = random_index.uniform( 0, img.lower_images.size() );
					int y = random_index.uniform( 0, img.lower_images[ x ].slidingWindows.size() );
					Mat hog_features = img.lower_images[ x ].slidingWindows[ y ].hog_features;
					classifier.addNegativeExample( hog_features );
					count++;
				}
				
		}
		classifier.train( MODEL_STANDARD_FILE );

		classifier.loadModel( MODEL_STANDARD_FILE );

		for(vector<string>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt)
		{

			OriginalImage img( *negIt );
			FeatureExtraction::computeHOGPyramid( img );
			FeatureExtraction::computeSlidingWindows( img );

			
			for( vector<SlidingWindow>::iterator it= img.slidingWindows.begin(); it != img.slidingWindows.end(); ++it )
			{
				if( classifier.classify( ( *it ).hog_features ) > 0.0 )
				{
					classifier.addNegativeExample( (*it ).hog_features );
				}

			}


			for(std::vector<Image>::iterator scaleIt = ( img ).lower_images.begin(); scaleIt != ( img ).lower_images.end(); ++scaleIt)
			{
				FeatureExtraction::computeSlidingWindows(*scaleIt);

				for( vector<SlidingWindow>::iterator it= ( *scaleIt ).slidingWindows.begin(); it != ( *scaleIt ).slidingWindows.end(); ++it )
				{
					if( classifier.classify( ( *it ).hog_features ) > 0.0 )
					{
						classifier.addNegativeExample( (*it ).hog_features );
					}

				}
				
			}



		}

		classifier.train( MODEL_HARD_EX_FILE );
	}
}