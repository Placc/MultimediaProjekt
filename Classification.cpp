#include "Classification.h"

using namespace std;
using namespace cv;

namespace Classification {
	void classify(string modelFile, string testDir){
		vector<OriginalImage> pos_examples;
		vector<OriginalImage> neg_examples;
		Preprocessing::loadImagesByDirectory(TEST_POS_ORIGINAL, pos_examples);
		Preprocessing::loadImagesByDirectory(TEST_NEG_ORIGINAL, neg_examples);
		for(vector<OriginalImage>::iterator posIt = pos_examples.begin(); posIt != pos_examples.end(); ++posIt){
			FeatureExtraction::extractHOGFeatures(*posIt);
			FeatureExtraction::computeSlidingWindows(*posIt);
		}
		for(vector<OriginalImage>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt){
			FeatureExtraction::extractHOGFeatures(*negIt);
			FeatureExtraction::computeSlidingWindows(*negIt);
		}
	}
}