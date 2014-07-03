#include "Classification.h"

using namespace std;
using namespace cv;

namespace Classification 
{

	/**
	*	reads in all of the test images and stores them in pos/neg example vectors
	*	also loads the previously trained model
	*	classifies each image seperately and stores the possible detections in a vector
	*	@param string modelFile: path where the trained model is saved
	*	@param string testDir: directory where the test dataset is stored
	*/
	void classify(string modelFile, string testDir){

	

		vector<string> pos_examples;
		vector<string> neg_examples;
		Preprocessing::loadPathsByDirectory(TEST_POS_ORIGINAL, pos_examples);
		Preprocessing::loadPathsByDirectory(TEST_NEG_ORIGINAL, neg_examples);
		Classifier classifier = Classifier();
		classifier.loadModel(MODEL_HARD_EX_FILE);

		for(vector<string>::iterator posIt = pos_examples.begin(); posIt != pos_examples.end(); ++posIt)
		{
			OriginalImage img( *posIt );
			FeatureExtraction::computeHOGPyramid( img );
			FeatureExtraction::computeSlidingWindows( img );

			for( vector<SlidingWindow>::iterator it = img.slidingWindows.begin(); it != img.slidingWindows.end(); ++it )
			{
				double distance_hyperplane = classifier.classify( ( *it ).hog_features );

				if( distance_hyperplane > 0 )			
				{
					ClassifiedRect rect(( *it ).slidingWindow, distance_hyperplane);
					img.addDetectedBox( rect );
				}

				
			}


			for( vector<Image>::iterator lower_img_it = ( img ).lower_images.begin();  lower_img_it != ( img ).lower_images.end(); ++lower_img_it )
			{
				FeatureExtraction::computeSlidingWindows( *lower_img_it );

				for( vector<SlidingWindow>::iterator it = ( *lower_img_it ).slidingWindows.begin(); it != ( *lower_img_it ).slidingWindows.end(); ++it )
				{
					double distance_hyperplane = classifier.classify ( ( *it ).hog_features );

					if( distance_hyperplane > 0 )
					{
						
						ClassifiedRect rect( ( *it ).slidingWindow, distance_hyperplane );
						img.addDetectedBox( rect );
					}

				}

			}

			Evaluation::showImageWithDetections( img );
			
		}

		for(vector<string>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt)
		{
			OriginalImage img( *negIt );
			FeatureExtraction::computeHOGPyramid( img );
			FeatureExtraction::computeSlidingWindows( img );

			for( vector<SlidingWindow>::iterator it = img.slidingWindows.begin(); it != img.slidingWindows.end(); ++it )
			{
				double distance_hyperplane = classifier.classify( ( *it ).hog_features );

				if( distance_hyperplane > 0 )			
				{
					ClassifiedRect rect( ( *it ).slidingWindow, distance_hyperplane );
					img.addDetectedBox( rect );
				}

			}

			
			for( vector<Image>::iterator lower_img_it = ( img ).lower_images.begin();  lower_img_it != ( img ).lower_images.end(); ++lower_img_it )
			{
				FeatureExtraction::computeSlidingWindows( *lower_img_it );

				for( vector<SlidingWindow>::iterator it = ( *lower_img_it ).slidingWindows.begin(); it != ( *lower_img_it ).slidingWindows.end(); ++it )
				{
					double distance_hyperplane = classifier.classify ( ( *it ).hog_features );

					if( distance_hyperplane > 0 )
					{
						ClassifiedRect rect( ( *it ).slidingWindow, distance_hyperplane );
						img.addDetectedBox( rect );
					}

				}

				Evaluation::showImageWithDetections( img );
			}

		}


	}
}