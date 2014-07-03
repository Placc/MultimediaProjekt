#include "Classifier.h"


using namespace cv;
using namespace std;


/**
*	converts the extracted hog features to a SparseVector format
*	@return
*/
SparseVector Classifier::convertToSparseVector(Mat hog_features)
{
	SparseVector vec(hog_features.rows * hog_features.cols);
	for(int y = 0; y < hog_features.rows; y++)
	{
		for(int x = 0; x < hog_features.cols; x++)
		{
			vec[y * hog_features.cols + x] = hog_features.at<float>(y,x);
		}
	}
	return vec;
}

/**
*	loads svm model from harddisk
*/
void Classifier::loadModel( string modelFile )
{
	model = SvmLightUtil::getModel( modelFile );
}

/**
*	adds a positive example to the training set
*	@param cv::Mat hog_features hog_features that are converted and then added to the exapmple vector
*/
void Classifier::addPositiveExample( cv::Mat hog_features )
{
	SparseVector vector = convertToSparseVector( hog_features );
	pos_examples.push_back( vector );
}

/**
*	adds a negative example to the training set
*	@param cv::Mat hog_features hog_features that are converted and then added to the exapmple vector
*/
void Classifier::addNegativeExample( cv::Mat hog_features )
{
	SparseVector vector = convertToSparseVector( hog_features );
	neg_examples.push_back( vector );
}


/**
*	converts the hog features to SparseVector format
*	classifies the hog featues and then returns the distance to hyperplane
*	@param cv::Mat hog_features: converts the hog features into SparseVector format and then classifies them
*	@return
*/
double Classifier::classify( Mat hog_features )
{
	SparseVector vec = convertToSparseVector( hog_features );
	double value = svm.classify( vec, DIMENSIONS, model );
	//joachims::MODEL model;

	return value;
}

/**
*	trains the svm model using the training dataset
*	@param string modelfile: directory where the svm model is stored
*/
void Classifier::train( string modelFile )
{
	svm.train( pos_examples, neg_examples, DIMENSIONS, modelFile, 1.0, params );
}

